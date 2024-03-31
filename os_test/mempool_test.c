#include <kernel/initcall.h>
#include <kernel/mempool.h>
#include <kernel/syslog.h>

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

static void dump_mempool(void)
{
	struct mempool *mm_pl = (struct mempool *)g_pl_default_mempool;

	for (size_t i = 0; i < mm_pl->blk_num; i++) {
		pl_syslog_info("====================================================\r\n");
		pl_syslog_info("first_bits:%u\r\n", mm_pl->blk_first_bits[i]);
		pl_syslog_info("max_bits:%u\r\n", mm_pl->blk_max_bits[i]);
		pl_syslog_info("bitmap:0x%x\r\n", mm_pl->blk_bitmaps[i]);
		pl_syslog_info("----------------------------------------------------\r\n");
	}
}

static int mempool_test(void)
{
	void *p;
	size_t mempool_size;
	

	mempool_size = pl_mempool_get_free_bytes(g_pl_default_mempool);
	pl_syslog_info("mempool_size:%d\r\n", mempool_size);
	dump_mempool();

	p = pl_mempool_malloc(g_pl_default_mempool, 200);
	USED(p);
	mempool_size = pl_mempool_get_free_bytes(g_pl_default_mempool);
	pl_syslog_info("alloc 200 after - mempool_size:%d\r\n", mempool_size);
	dump_mempool();

	pl_mempool_free(g_pl_default_mempool, p);
	mempool_size = pl_mempool_get_free_bytes(g_pl_default_mempool);
	pl_syslog_info("free 200 after - mempool_size:%d\r\n", mempool_size);
	dump_mempool();

	return 0;
}
early_initcall(mempool_test);
