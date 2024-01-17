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

#ifndef __KERNEL_MEM_POOL_H__
#define __KERNEL_MEM_POOL_H__
#include "types.h"
#include <stddef.h>

struct mem_pool {
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

struct mem_pool_data {
	struct mem_pool *mp;
	uintptr_t magic;
	size_t bit_idx;
	size_t bit_num;
	uchar_t *data[0];
};

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************
 * Function Name: pl_mem_pool_init
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
 ************************************************************************************/
struct mem_pool *pl_mem_pool_init(void *pool, ushrt_t id,
                      size_t pool_size, uchar_t grain_order);

/*************************************************************************************
 * Function Name: pl_mem_pool_alloc
 *
 * Description:
 *   Update bitmap when memory is allocated or free.
 *
 * Param:
 *   @mp: memory pool.
 *   @size: memory size to require.
 * 
 * Return:
 *   address of memory.
 ************************************************************************************/
void *pl_mem_pool_alloc(struct mem_pool *mp, size_t size);


/*************************************************************************************
 * Function Name: pl_mem_pool_free
 *
 * Description:
 *   Free memory interface.
 *
 * Param:
 *   @mp: memory pool.
 *   @p: memory address.
 * 
 * Return:
 *   void.
 ************************************************************************************/
void pl_mem_pool_free(struct mem_pool *mp, void *p);

/*************************************************************************************
 * Function Name: pl_mem_pool_get_free_bytes
 *
 * Description:
 *   Get the remaining memory of the memory block interface.
 *
 * Param:
 *   @mp: memory pool.
 * 
 * Return:
 *   remaining size of memory pool.
 ************************************************************************************/
size_t pl_mem_pool_get_free_bytes(struct mem_pool *mp);


/*************************************************************************************
 * Function Name: pl_mem_pool_set
 *
 * Description:
 *   set value of memory block.
 *
 * Param:
 *   @mp: memory pool.
 *   @p: address of memory
 *   @val: set value required.
 *   @size: the sizeof memory.
 * 
 * Return:
 *   the end of setting value address.
 ************************************************************************************/
void *pl_mem_pool_set(struct mem_pool *mp, void *p, uint8_t val, size_t size);


/*************************************************************************************
 * Function Name: pl_mem_pool_zalloc
 *
 * Description:
 *   Alloca memory interface of zero value.
 *   The value of memory will set to zero.
 *
 * Param:
 *   @mp: memory pool.
 *   @size: the size of alloacte required.
 * 
 * Return:
 *   memory address.
 ************************************************************************************/
void *pl_mem_pool_zalloc(struct mem_pool *mp, size_t size);

/*************************************************************************************
 * Function Name: pl_mem_pool_calloc
 *
 * Description:
 *   Alloca memory interface of array method.
 *   The value of memory will set to zero.
 *
 * Param:
 *   @mp: memory pool.
 *   @num: number of memory.
 *   @size: size of each memory block.
 * 
 * Return:
 *   memory address.
 ************************************************************************************/
void *pl_mem_pool_calloc(struct mem_pool *mp, size_t num, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_MEM_POOL_H__ */
