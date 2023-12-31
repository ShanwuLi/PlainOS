#ifndef __KERNEL_MM_H__
#define __KERNEL_MM_H__
#include <stdint.h>

struct mm_pool {
	
};


struct mm_ctrl_blk {
	uint32_t *bitmap;
	uint32_t *tagmap;
	uint32_t rest_bytes;
	uint8_t data[0];
};

struct mm_sub_blk {
	struct mm_blk *blk;
	uint32_t max_alloc;
	uint32_t bitmap;
	uint8_t data[0];
};

struct mm_data {
	uint16_t bit_idx;
	uint16_t bit_size;
	struct mm_blk *blk;
	uint8_t data[0];
};



#endif /* __KERNEL_MM_H__ */
