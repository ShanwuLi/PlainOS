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

#ifndef __KERNEL_MEMPOOL_H__
#define __KERNEL_MEMPOOL_H__

#include <types.h>
#include <stddef.h>

typedef void *pl_mempool_handle_t;
extern pl_mempool_handle_t g_pl_default_mempool;

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************
 * 函数名称: pl_mempool_calloc
 *
 * 描述:
 *   数组方法的内存分配接口。
 *   分配的内存值将被设置为零。
 *
 * 参数:
 *   无
 *
 * 返回值:
 *   成功时返回大于等于0的值，失败时返回小于0的值。
 **********************************************************************************/
int pl_default_mempool_init(void);

/**********************************************************************************
 * 函数名称: pl_mempool_init
 * 
 * 描述:
 *   内存池初始化接口。
 *
 * 参数:
 *   @pool: 用户提供的必需内存块。
 *   @id: 内存池标识号。
 *   @pool_size: 用户提供的内存块所需大小。
 *   @grain_order: 内存分配器需要管理的最小粒度。
 *                粒度大小 = (1 << grain_order)，即粒度大小等于2的grain_order次方。
 * 
 * 返回值:
 *   内存池的句柄。
 ***********************************************************************************/
pl_mempool_handle_t pl_mempool_init(void *pool, ushrt_t id,
                                  size_t pool_size, uchar_t grain_order);

/**********************************************************************************
 * 函数名称: pl_mempool_malloc
 * 
 * 描述:
 *   在分配或释放内存时更新位图。
 *
 * 参数:
 *   @mempool: 内存池实例。
 *   @size: 所需的内存大小。
 * 
 * 返回值:
 *   内存地址。
 ***********************************************************************************/
void *pl_mempool_malloc(pl_mempool_handle_t mempool, size_t size);

/**********************************************************************************
 * 函数名称: pl_mempool_free
 * 
 * 描述:
 *   内存释放接口。
 *
 * 参数:
 *   @mempool: 内存池对象。
 *   @p: 待释放的内存地址。
 * 
 * 返回值:
 *   无。
 ***********************************************************************************/
void pl_mempool_free(pl_mempool_handle_t mempool, void *p);

/**********************************************************************************
 * 函数名称: pl_mempool_get_free_bytes
 * 
 * 描述:
 *   获取内存池剩余内存大小的接口。
 *
 * 参数:
 *   @mempool: 内存池实例。
 * 
 * 返回值:
 *   内存池的剩余容量。
 ***********************************************************************************/
size_t pl_mempool_get_free_bytes(pl_mempool_handle_t mempool);


/**********************************************************************************
 * 函数名称: pl_mempool_set
 * 
 * 描述:
 *   设置内存块的值。
 *
 * 参数:
 *   @mempool: 内存池对象。
 *   @p: 内存地址。
 *   @val: 需要设置的值。
 *   @size: 内存大小。
 * 
 * 返回值:
 *   设置值结束之后的地址，即`p + size`。
 ***********************************************************************************/
void *pl_mempool_set(pl_mempool_handle_t mempool, void *p, uint8_t val, size_t size);

/**********************************************************************************
 * 函数名称: pl_mempool_zalloc
 * 
 * 描述:
 *   零值内存分配接口。
 *   分配的内存将会被初始化为零。
 *
 * 参数:
 *   @mempool: 内存池实例。
 *   @size: 所需分配的内存大小。
 * 
 * 返回值:
 *   分配的内存地址。
 ***********************************************************************************/
void *pl_mempool_zalloc(pl_mempool_handle_t mempool, size_t size);

/**********************************************************************************
 * 函数名称: pl_mempool_calloc
 * 
 * 描述:
 *   数组方式的内存分配接口。
 *   分配的内存区域将被初始化为零。
 *
 * 参数:
 *   @mempool: 内存池对象。
 *   @num: 内存块的数量。
 *   @size: 每个内存块的大小。
 * 
 * 返回值:
 *   分配的内存起始地址。
 ***********************************************************************************/
void *pl_mempool_calloc(pl_mempool_handle_t mempool, size_t num, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_MEMPOOL_H__ */
