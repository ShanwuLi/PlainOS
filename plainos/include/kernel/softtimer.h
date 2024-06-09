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

#ifndef __KERNEL_SOFTTIMER_H__
#define __KERNEL_SOFTTIMER_H__

#include <types.h>
#include <kernel/list.h>
#include <kernel/kernel.h>

struct pl_stimer;
typedef void (*pl_stimer_fun_t)(struct pl_stimer *timer);

struct pl_stimer {
	struct list_node node;
	const char *name;
	pl_stimer_fun_t fun;
	void *priv_data;
	u64_t timing_cnt;
	u64_t reach_cnt;
	bool reload;
};

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************
 * 函数名称: pl_softtimer_request
 * 
 * 描述:
 *   请求一个软定时器。
 * 
 * 参数:
 *  @name: 定时器名称。
 *
 * 返回值:
 *  软定时器的句柄（@struct pl_stimer*）。
 ***********************************************************************************/
struct pl_stimer *pl_softtimer_request(const char *name);

/**********************************************************************************
 * 函数名称: pl_softtimer_get_private_data
 * 
 * 描述:
 *   获取软定时器的私有数据。
 * 
 * 参数:
 *  @timer: 软定时器的句柄。
 *  @data: 私有数据的地址指针。
 *
 * 返回值:
 *  成功时返回大于等于0的值，失败时返回小于0的值。
 ***********************************************************************************/
int pl_softtimer_get_private_data(struct pl_stimer *timer, void **data);

/**********************************************************************************
 * 函数名称: pl_softtimer_timer_init
 * 
 * 描述:
 *   初始化软定时器。
 * 
 * 参数:
 *  @timer: 软定时器的句柄。
 *  @fun: 回调函数指针。
 *  @timing_cnt: 定时次数。
 *  @priv_data: 私有数据。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
void pl_softtimer_timer_init(struct pl_stimer *stimer, pl_stimer_fun_t fun,
                                         u64_t timing_cnt, void *priv_data);

/**********************************************************************************
 * 函数名称: pl_softtimer_start
 * 
 * 描述:
 *   启动软定时器。
 * 
 * 参数:
 *  @timer: 软定时器的句柄。
 *
 * 返回值:
 *  成功时返回大于等于0的值，失败时返回小于0的值。
 ***********************************************************************************/
int pl_softtimer_start(struct pl_stimer *timer);

/**********************************************************************************
 * 函数名称: pl_softtimer_reload
 * 
 * 描述:
 *   重载软定时器。
 * 
 * 参数:
 *  @timer: 软定时器的句柄。
 *  @reload: 是否重载标志。
 *  @fun: 回调函数指针。
 *  @timing_cnt: 定时次数。
 *  @priv_data: 私有数据。
 *
 * 返回值:
 *  成功时返回大于等于0的值，失败时返回小于0的值。
 ***********************************************************************************/
int pl_softtimer_reload(struct pl_stimer *timer, bool reload, pl_stimer_fun_t fun,
                                               u64_t timing_cnt, void *priv_data);

/**********************************************************************************
 * 函数名称: pl_softtimer_cancel
 * 
 * 描述:
 *   取消软定时器。
 * 
 * 参数:
 *  @timer: 软定时器的句柄。
 *
 * 返回值:
 *  成功时返回大于等于0的值，失败时返回小于0的值。
 ***********************************************************************************/
int pl_softtimer_cancel(struct pl_stimer *timer);

/**********************************************************************************
 * 函数名称: pl_softtimer_cancel
 * 
 * 描述:
 *   取消软定时器。
 * 
 * 参数:
 *  @timer: 软定时器的句柄。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
void pl_softtimer_release(struct pl_stimer *timer);

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_SOFTTIMER_H__ */
