/* MIT License

Copyright (c) 2023 PlainOS

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <config.h>
#include <types.h>
#include <errno.h>
#include <kernel/kernel.h>
#include <kernel/mempool.h>
#include <kernel/assert.h>
#include <kernel/semaphore.h>
#include <kernel/initcall.h>
#include <kernel/syslog.h>

/*************************************************************************************
 * Description: mempool structure definition.
 ************************************************************************************/
struct mempool {
	ushrt_t id;
	uchar_t state;
	uchar_t grain_order;
	size_t blk_num;
	uintptr_t *blk_bitmaps;
	uchar_t *blk_first_bits;
	uchar_t *blk_max_bits;
	size_t data_pool_size;
	uchar_t *data_pool;
};

struct mempool_data {
	size_t bit_idx;
	size_t bit_num;
	uchar_t *data[0];
};

struct default_mempool {
	struct pl_sem sem;
	u8_t pool_data[CONFIG_PL_DEFAULT_MEMPOOL_SIZE];
};

/*************************************************************************************
 * Description: definitions.
 ************************************************************************************/
typedef bool (*find_bit_condition_t)(struct mempool* mp, size_t iter,
                                     size_t alloc_size, size_t* arg);

/*************************************************************************************
 * Description: default memory pool.
 ************************************************************************************/
static struct default_mempool pl_default_mempool;
pl_mempool_handle_t g_pl_default_mempool;

/*************************************************************************************
 * Function Name: calculate_min_pool_size
 *
 * Description:
 *    Before initialization, we need to calculate the minimum memory size.
 * If the given memory block is smaller than this value, it will cause
 * initialization failure.
 *
 * Param:
 *   @grain_order: the minimum granularity that memory allocators need to manage.
 *                 grain_order_size = (1 << grain_order).
 *
 * Return:
 *   minimum size of mempool we needed.
 ************************************************************************************/
static size_t calculate_min_pool_size(uchar_t grain_order)
{
	size_t min_pool_size = 0;

	/* add mempool_header */
	min_pool_size += sizeof(struct mempool);
	/* add ctrl_blk_size */
	min_pool_size += sizeof(uintptr_t) + (sizeof(uchar_t) << 1);
	/* add gap_size (4 * sizeof(uintptr_t)) */
	min_pool_size += sizeof(uintptr_t) << 2;
	/* add data_size */
	min_pool_size += (size_t)1 << grain_order;

	return min_pool_size;
}

/*************************************************************************************
 * Function Name: calculate_blk_num
 *
 * Description:
 *    Calculate the number of bitmaps based on the size of the memory pool
 *
 * Param:
 *   @grain_order: the minimum granularity that memory allocators need to manage.
 *                 grain_order_size = (1 << grain_order).
 *   @pool_size: size of memory pool.
 *
 * Return:
 *   The number of blocks that require a bitmap to manage.
 ************************************************************************************/
static size_t calculate_blk_num(uchar_t grain_order, size_t pool_size)
{
	size_t blk_num;
	size_t gap_size;
	size_t blk_size;

	/* calculate gap_size */
	gap_size = sizeof(uintptr_t) << 2;
	/* add ctrl_blk_size and data_size */
	blk_size = sizeof(uintptr_t) + (sizeof(uchar_t) << 1);
	blk_size += ((size_t)1 << grain_order) * UINTPTR_T_BITS;
	/* calculate body_size */
	pool_size -= sizeof(struct mempool);
	blk_num = ((pool_size - gap_size) + blk_size - 1) / blk_size;

	return blk_num;
}

/*************************************************************************************
 * Function Name: mempool_blk_init
 *
 * Description:
 *    Memory pool block initialization function.
 *
 * Param:
 *   @mp: memory pool.
 *
 * Return:
 *   Aligned address.
 ************************************************************************************/
static void mempool_blk_init(struct mempool* mp)
{
	size_t i;
	uchar_t grain_num;
	size_t rest_size = mp->data_pool_size;

	/* initialize mempool */
	for (i = 0; i < mp->blk_num - 1; i++) {
		mp->blk_first_bits[i] = UINTPTR_T_BITS;
		mp->blk_max_bits[i] = UINTPTR_T_BITS;
		mp->blk_bitmaps[i] = UINTPTR_T_MAX;
	}

	rest_size -= i * ((size_t)1 << mp->grain_order) * UINTPTR_T_BITS;
	grain_num = (uchar_t)(rest_size >> mp->grain_order);
	mp->blk_first_bits[i] = grain_num;
	mp->blk_max_bits[i] = grain_num;

	/* avoid overflow */
	if (grain_num == 0) {
		mp->blk_bitmaps[i] = 0;
		return;
	}

	mp->blk_bitmaps[i] = ((uintptr_t)1 << grain_num) - 1;
}

/*************************************************************************************
 * Function Name: pl_mempool_init
 *
 * Description:
 *    Memory pool initialization interface.
 *
 * Param:
 *   @pool: user provided memory block required.
 *   @id: memory pool identification number.
 *   @pool_size: user provided memory block size required.
 *   @grain_order: the minimum granularity that memory allocators need to manage.
 *                 grain_order_size = (1 << grain_order).
 *
 * Return:
 *   handle of memory pool.
 *
 * NOTE:
 * mempool:
 *  [//gap//][struct mempool][bitmaps][fbits][mbits][//gap//][data_pool]
 *  A        A               A        A      A               A
 *  |        |               |        |      |               |
 * pool      mp             body    fbits  mfits         data_pool
 *           \_________________________________________________________/
 *                                     |
 *                                pool_szie
 ************************************************************************************/
pl_mempool_handle_t pl_mempool_init(void* pool, ushrt_t id, size_t pool_size,
                                uchar_t grain_order)
{
	struct mempool* mp;
	size_t blk_num;
	size_t min_pool_size;

	if (pool == NULL)
		return NULL;

	mp = (struct mempool*)pl_align_address(pool, sizeof(uintptr_t));
	min_pool_size = calculate_min_pool_size(grain_order);
	if ((uintptr_t)pool + pool_size < (uintptr_t)mp + min_pool_size)
		return NULL;

	/* get actual size of pool */
	pool_size -= ((uintptr_t)mp - (uintptr_t)pool);
	blk_num = calculate_blk_num(grain_order, pool_size);
	if (blk_num == 0)
		return NULL;

	mp->id = id;
	mp->state = 0;
	mp->grain_order = grain_order;
	mp->blk_num = blk_num;
	mp->blk_bitmaps = (uintptr_t*)((uchar_t *)mp + sizeof(struct mempool));
	mp->blk_first_bits = (uchar_t*)(mp->blk_bitmaps + blk_num);
	mp->blk_max_bits = mp->blk_first_bits + blk_num;
	mp->data_pool = mp->blk_max_bits + blk_num;
	mp->data_pool = (uchar_t*)pl_align_address(mp->data_pool, sizeof(uintptr_t) << 2);
	mp->data_pool_size = pool_size - ((size_t)(mp->data_pool) - (size_t)mp);
	mp->data_pool_size &= (~(((size_t)1 << grain_order) - 1));

	mempool_blk_init(mp);
	return mp;
}

/*************************************************************************************
 * Function Name: find_blk_condition
 *
 * Description:
 *   Conditions for finding Memory index numbers by bitmap blocks.
 *
 * Param:
 *   @mp: Memory pool.
 *   @iter: idx of block.
 *   @alloc_size: memory size to be allocated.
 *   @arg: passed argument.
 *
 * Return:
 *   true: can find.
 *   false: can't find.
 *   can or can't we find it.
 ************************************************************************************/
static bool find_blk_condition(struct mempool* mp, size_t iter, size_t alloc_size,
                               size_t* arg)
{
	size_t first_blk_alloc = 0;

	if (iter < mp->blk_num - 1)
		first_blk_alloc = ((size_t)1 << mp->grain_order) *
		mp->blk_first_bits[iter + 1];

	*arg = (mp->blk_bitmaps[iter] == UINTPTR_T_MAX) ? *arg + 1 : 0;
	if ((((*arg << mp->grain_order) * UINTPTR_T_BITS) |
		first_blk_alloc) >= alloc_size) {
		*arg -= 1;
		return true;
	}

	return false;
}

/*************************************************************************************
 * Function Name: find_bit_condition
 *
 * Description: Conditions for finding Memory index numbers in bitmap block.
 *
 * Param:
 *   @mp: Memory pool.
 *   @iter: idx of block.
 *   @alloc_size: memory size to be allocated.
 *   @arg: passed argument.
 *
 * Return:
 *   true: can find.
 *   false: can't find.
 *   can or can't we find it.
 ************************************************************************************/
static bool find_bit_condition(struct mempool *mp, size_t iter, size_t alloc_size,
                               size_t* arg)
{
	if (mp->blk_max_bits[iter] * ((uintptr_t)1 << mp->grain_order)
		>= alloc_size)
		return true;

	USED(arg);
	return false;
}

/*************************************************************************************
 * Function Name: get_blk_idx
 *
 * Description:
 *   Get memory block index numbers based on required memory size and find condition.
 *
 * Param:
 *   @mp: Memory pool.
 *   @alloc_size: memory size to be allocated.
 *   @condition: find condition.
 *   @idx: got index of block.
 *
 * Return:
 *   true: got it.
 *   false: can't find.
 ************************************************************************************/
static bool get_blk_idx(struct mempool *mp, size_t alloc_size,
                        find_bit_condition_t condition, size_t* idx)
{
	size_t i;
	bool res;
	size_t max_continue_num = 0;

	for (i = 0; i < mp->blk_num; i++) {
		res = condition(mp, i, alloc_size, &max_continue_num);
		if (res) {
			*idx = i - max_continue_num;
			return true;
		}
	}

	return false;
}

/*************************************************************************************
 * Function Name: get_bit_offset
 *
 * Description:
 *   Get the offset inside the memory block based on blk_idx.
 *
 * Param:
 *   @mp: Memory pool.
 *   @blk_idx: index of block.
 *   @grain_order: the minimum granularity that memory allocators need to manage.
 *                 grain_order_size = (1 << grain_order).
 *   @idx: got index of block.
 *
 * Return:
 *   offset bit in blk_idx.
 ************************************************************************************/
static size_t get_bit_offset(struct mempool *mp, size_t blk_idx,
                             uchar_t grain_order, size_t alloc_size)
{
	size_t i;
	size_t max_continue_bits = 0;
	uintptr_t blk_bitmap = mp->blk_bitmaps[blk_idx];

	if (alloc_size >= (((size_t)1 << grain_order) * UINTPTR_T_BITS))
		return 0;

	for (i = 0; i < UINTPTR_T_BITS; i++) {
		max_continue_bits = blk_bitmap & ((uintptr_t)1 << i) ?
			max_continue_bits + 1 : 0;
		if ((max_continue_bits << grain_order) >= alloc_size)
			break;
	}

	return i + 1 - max_continue_bits;
}

/*************************************************************************************
 * Function Name: update_blk_bits
 *
 * Description:
 *   Update bits when memory is allocated or free.
 *
 * Param:
 *   @mp: Memory pool.
 *   @blk_idx: index of block.
 *
 * Return:
 *   void.
 ************************************************************************************/
static void update_blk_bits(struct mempool *mp, size_t blk_idx)
{
	size_t i;
	uchar_t fbits = 0;
	uchar_t bits = 0;
	uchar_t mbits = 0;
	uchar_t fbits_inc = 1;
	uintptr_t blk_bitmap = mp->blk_bitmaps[blk_idx];

	for (i = 0; i < UINTPTR_T_BITS; i++) {
		if (blk_bitmap & ((uintptr_t)1 << i)) {
			fbits += fbits_inc;
			++bits;
		} else {
			fbits_inc = 0;
			mbits = bits > mbits ? bits : mbits;
			bits = 0;
		}
	}

	mbits = bits > mbits ? bits : mbits;
	mp->blk_first_bits[blk_idx] = fbits;
	mp->blk_max_bits[blk_idx] = mbits;
}

/*************************************************************************************
 * Function Name: update_bit_map
 *
 * Description:
 *   Update bitmap when memory is allocated or free.
 *
 * Param:
 *   @mp: Memory pool.
 *   @blk_start_idx: start index of block.
 *   @bit_offset: bit offset.
 *   @bit_num: the number of bits.
 *   @set_bits: set or clear bits.
 *
 * Return:
 *   void.
 ************************************************************************************/
static void update_bit_map(struct mempool *mp, size_t blk_start_idx,
                           size_t bit_offset, size_t bit_num, bool set_bits)
{
	size_t i = 0;
	uintptr_t bitmap = 0;
	uintptr_t bitmap_mask;
	uchar_t first_bits = 0;
	uchar_t max_bits = 0;
	size_t blk_offset = blk_start_idx;

	if (set_bits) {
		bitmap = UINTPTR_T_MAX;
		first_bits = UINTPTR_T_BITS;
		max_bits = UINTPTR_T_BITS;
	}

	if (bit_num >= UINTPTR_T_BITS) {
		for (i = 0; i < bit_num / UINTPTR_T_BITS; i++) {
			mp->blk_first_bits[blk_offset] = first_bits;
			mp->blk_max_bits[blk_offset] = max_bits;
			mp->blk_bitmaps[blk_offset] = bitmap;
			++blk_offset;
		}
	}

	/* bit_num -= i * UINTPTR_T_BITS */
	bit_num -= i * UINTPTR_T_BITS;
	bitmap = mp->blk_bitmaps[blk_offset];
	bitmap_mask = ((((uintptr_t)1 << bit_num) - 1) << bit_offset);
	bitmap = set_bits ? (bitmap | bitmap_mask) : (bitmap & (~bitmap_mask));
	mp->blk_bitmaps[blk_offset] = bitmap;
	/* get rest bit_map */
	update_blk_bits(mp, blk_offset);
}

/*************************************************************************************
 * Function Name: pl_mempool_malloc
 *
 * Description:
 *   Update bitmap when memory is allocated or free.
 *
 * Param:
 *   @mempool: memory pool.
 *   @size: memory size to require.
 *
 * Return:
 *   address of memory.
 ************************************************************************************/
void* pl_mempool_malloc(pl_mempool_handle_t mempool, size_t size)
{
	bool found;
	size_t blk_idx;
	size_t bit_offset;
	struct mempool_data* data_addr;
	struct mempool *mp = (struct mempool *)mempool;
	size_t grain_size = (uintptr_t)1 << mp->grain_order;
	size_t alloc_size = size + sizeof(struct mempool_data);
	size_t bit_num = (alloc_size + grain_size - 1) / grain_size;

	/* check mp if is NULL */
	if (mp == NULL)
		return NULL;

	pl_semaphore_wait(&pl_default_mempool.sem);
	/* get block index */
	if (bit_num < UINTPTR_T_BITS)
		found = get_blk_idx(mp, alloc_size, find_bit_condition, &blk_idx);
	else
		found = get_blk_idx(mp, alloc_size, find_blk_condition, &blk_idx);

	/* check block index */
	if (!found || blk_idx >= mp->blk_num) {
		pl_semaphore_post(&pl_default_mempool.sem);
		return NULL;
	}

	/* get bit offset */
	bit_offset = get_bit_offset(mp, blk_idx, mp->grain_order, alloc_size);
	data_addr = (struct mempool_data*)(mp->data_pool +
	             (((blk_idx << mp->grain_order) * UINTPTR_T_BITS) |
	               (bit_offset << mp->grain_order)));

	/* set data structure */
	data_addr->bit_idx = (blk_idx * UINTPTR_T_BITS) | bit_offset;
	data_addr->bit_num = bit_num;

	/* update bitmap */
	update_bit_map(mp, blk_idx, bit_offset, bit_num, false);
	pl_semaphore_post(&pl_default_mempool.sem);

	return (void*)data_addr->data;
}

/*************************************************************************************
 * Function Name: get_mempool_data
 *
 * Description:
 *   Get the data head of memory based on memory address p.
 *
 * Param:
 *   @mp: memory pool.
 *   @p: memory address.
 *
 * Return:
 *   data in the memory pool.
 ************************************************************************************/
static struct mempool_data* get_mempool_data(struct mempool *mp, void* p)
{
	size_t blk_idx;
	struct mempool_data* data_addr;

	/* check mp and p */
	if (mp == NULL || p == NULL)
		return NULL;

	/* get address of data and check it */
	data_addr = container_of(p, struct mempool_data, data);
	if ((uchar_t*)data_addr >= (mp->data_pool + mp->data_pool_size) ||
		(uchar_t*)data_addr < mp->data_pool)
		return NULL;

	/* get magic of data and block index */
	blk_idx = data_addr->bit_idx / UINTPTR_T_BITS;

	/* check mp and magic of data */
	if (blk_idx >= mp->blk_num)
		return NULL;

	return data_addr;
}

/*************************************************************************************
 * Function Name: pl_mempool_free
 *
 * Description:
 *   Free memory interface.
 *
 * Param:
 *   @mempool: memory pool.
 *   @p: memory address.
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_mempool_free(pl_mempool_handle_t mempool, void* p)
{
	size_t blk_idx;
	size_t bit_offset;
	struct mempool_data* data_addr;
	struct mempool *mp = (struct mempool *)mempool;

	data_addr = get_mempool_data(mp, p);
	if (data_addr == NULL)
		return;

	pl_semaphore_wait(&pl_default_mempool.sem);
	blk_idx = data_addr->bit_idx / UINTPTR_T_BITS;
	bit_offset = data_addr->bit_idx & (UINTPTR_T_BITS - 1);
	update_bit_map(mp, blk_idx, bit_offset, data_addr->bit_num, true);
	pl_semaphore_post(&pl_default_mempool.sem);
}

/*************************************************************************************
 * Function Name: mempool_blk_get_free_bytes
 *
 * Description:
 *   Get the remaining memory of the memory block.
 *
 * Param:
 *   @mp: memory pool.
 *   @blk_idx: index of block.
 *   @grain_order: the minimum granularity that memory allocators need to manage.
 *                 grain_order_size = (1 << grain_order).
 *
 * Return:
 *   remaining size of memory block.
 ************************************************************************************/
static size_t mempool_blk_get_free_bytes(struct mempool *mp, size_t blk_idx,
                                          uchar_t grain_order)
{
	size_t i;
	size_t rest_bit_num = 0;

	for (i = 0; i < UINTPTR_T_BITS; i++) {
		if (mp->blk_bitmaps[blk_idx] & ((uintptr_t)1 << i))
			++rest_bit_num;
	}

	return (rest_bit_num << grain_order);
}

/*************************************************************************************
 * Function Name: pl_mempool_get_free_bytes
 *
 * Description:
 *   Get the remaining memory of the memory block interface.
 *
 * Param:
 *   @mempool: memory pool.
 *
 * Return:
 *   remaining size of memory pool.
 ************************************************************************************/
size_t pl_mempool_get_free_bytes(pl_mempool_handle_t mempool)
{
	size_t i;
	size_t rest_bytes = 0;
	struct mempool *mp = (struct mempool *)mempool;

	if (mp == NULL)
		return 0;

	pl_semaphore_wait(&pl_default_mempool.sem);
	for (i = 0; i < mp->blk_num; i++)
		rest_bytes += mempool_blk_get_free_bytes(mp, i, mp->grain_order);
	pl_semaphore_post(&pl_default_mempool.sem);

	return rest_bytes;
}

/*************************************************************************************
 * Function Name: pl_mempool_set
 *
 * Description:
 *   set value of memory block.
 *
 * Param:
 *   @mempool: memory pool.
 *   @p: address of memory
 *   @val: set value required.
 *   @size: the sizeof memory.
 *
 * Return:
 *   the end of setting value address.
 ************************************************************************************/
void* pl_mempool_set(pl_mempool_handle_t mempool, void* p, uint8_t val, size_t size)
{
	size_t i;
	char* char_p;
	uintptr_t uintptr_val = val;
	uintptr_t* set_p = (uintptr_t*)p;
	size_t words = size / sizeof(uintptr_t);
	size_t slice = size & (sizeof(uintptr_t) - 1);
	struct mempool *mp = (struct mempool *)mempool;

	if (mp == NULL || p == NULL || size == 0)
		return p;

	for (i = 0; i < sizeof(uintptr_t); i++)
		uintptr_val |= ((uintptr_t)val << (i << 3));

	for (i = 0; i < words; i++)
		*(set_p++) = uintptr_val;

	char_p = (char*)set_p;
	for (i = 0; i < slice; i++)
		*(char_p++) = val;

	return char_p - 1;
}

/*************************************************************************************
 * Function Name: pl_mempool_zalloc
 *
 * Description:
 *   Alloca memory interface of zero value.
 *   The value of memory will set to zero.
 *
 * Param:
 *   @mempool: memory pool.
 *   @size: the size of alloacte required.
 *
 * Return:
 *   memory address.
 ************************************************************************************/
void* pl_mempool_zalloc(pl_mempool_handle_t mempool, size_t size)
{
	void* data;

	data = pl_mempool_malloc(mempool, size);
	if (data != NULL)
		pl_mempool_set(mempool, data, 0, size);

	return data;
}

/*************************************************************************************
 * Function Name: pl_mempool_calloc
 *
 * Description:
 *   Alloca memory interface of array method.
 *   The value of memory will set to zero.
 *
 * Param:
 *   @mempool: memory pool.
 *   @num: number of memory.
 *   @size: size of each memory block.
 *
 * Return:
 *   memory address.
 ************************************************************************************/
void *pl_mempool_calloc(pl_mempool_handle_t mempool, size_t num, size_t size)
{
	void* data;
	size_t alloc_size = num * size;

	data = pl_mempool_malloc(mempool, alloc_size);
	if (data != NULL)
		pl_mempool_set(mempool, data, 0, alloc_size);

	return data;
}

/*************************************************************************************
 * Function Name: pl_mempool_calloc
 *
 * Description:
 *   Alloca memory interface of array method.
 *   The value of memory will set to zero.
 *
 * Param:
 *   none
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
static int pl_default_mempool_init(void)
{
	int ret = pl_semaphore_init(&pl_default_mempool.sem, 1);
	pl_assert(ret == OK);

	g_pl_default_mempool = pl_mempool_init(pl_default_mempool.pool_data,
										   0, CONFIG_PL_DEFAULT_MEMPOOL_SIZE,
										   CONFIG_PL_DEFAULT_MEMPOOL_GRAIN_ORDER);
	pl_assert(g_pl_default_mempool != NULL);

	pl_early_syslog_info("mempool init done\r\n");
	return OK;
}
pl_early_initcall(pl_default_mempool_init);
