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

#ifndef __KERNEL_SEMAPHORE_H__
#define __KERNEL_SEMAPHORE_H__

#include <types.h>
#include <kernel/list.h>

struct pl_sem {
	struct list_node wait_list;
	int_t value;
	bool valid;
};

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************
 * 函数名称: pl_semaphore_init
 * 
 * 描述:
 *   初始化一个信号量。
 * 
 * 参数:
 *  @sem: 信号量实例。
 *  @val: 信号量的初始值。
 *
 * 返回值:
 *  成功时返回大于等于0的值，失败时返回小于0的值。
 ***********************************************************************************/
int pl_semaphore_init(struct pl_sem *sem, int val);

/**********************************************************************************
 * 函数名称: pl_semaphore_request
 * 
 * 描述:
 *   请求一个信号量。
 * 
 * 参数:
 *  @val: 信号量尝试减少到的值。
 *
 * 返回值:
 *  信号量的句柄。
 ***********************************************************************************/
struct pl_sem *pl_semaphore_request(int val);

/**********************************************************************************
 * 函数名称: pl_semaphore_wait
 * 
 * 描述:
 *   等待（获取）信号量。
 * 
 * 参数:
 *  @sem: 信号量的句柄。
 *
 * 返回值:
 *  成功时返回大于等于0的值，失败时返回小于0的值。
 ***********************************************************************************/
int pl_semaphore_wait(struct pl_sem *sem);

/**********************************************************************************
 * 函数名称: pl_semaphore_post
 * 
 * 描述:
 *   释放（增加）信号量。
 * 
 * 参数:
 *  @sem: 信号量的句柄。
 *
 * 返回值:
 *  成功时返回大于等于0的值，失败时返回小于0的值。
 ***********************************************************************************/
int pl_semaphore_post(struct pl_sem *sem);

/**********************************************************************************
 * 函数名称: pl_semaphore_release
 * 
 * 描述:
 *   释放一个信号量资源。
 * 
 * 参数:
 *   @sem: 信号量的句柄。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
void pl_semaphore_release(struct pl_sem *sem);

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_SEMAPHORE_H__ */