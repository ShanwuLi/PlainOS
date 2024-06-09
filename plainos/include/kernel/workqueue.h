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

#ifndef __KERNEL_WORKQUEUE_H__
#define __KERNEL_WORKQUEUE_H__

#include <types.h>
#include <kernel/task.h>
#include <kernel/kernel.h>

struct pl_work;
typedef void (*pl_work_fun_t)(struct pl_work *work);

struct pl_work {
	pl_work_fun_t fun;
	void *priv_data;
};

struct pl_workqueue {
	pl_tid_t exec_thread;
	u32_t fifo_cap;
	u32_t fifo_in;
	u32_t fifo_out;
	const char *name;
	struct pl_work **fifo;
};

uint_t wq_get_work_num(void);
/**********************************************************************************
 * 函数名称: pl_workqueue_create
 *  @g_pl_sys_hiwq_handle: 高优先级工作队列的句柄。
 *  @g_pl_sys_lowq_handle: 低优先级工作队列的句柄。
 *
 * 描述:
 *   创建一个工作队列，包含高优先级和低优先级两个队列。
 ***********************************************************************************/
extern struct pl_workqueue *g_pl_sys_hiwq_handle;
extern struct pl_workqueue *g_pl_sys_lowq_handle;

/**********************************************************************************
 * 函数名称: pl_workqueue_create
 * 描述: 创建一个具有指定属性的工作队列。
 *
 * 参数:
 *  @name: 工作队列的名称。
 *  @proi: 工作队列的优先级。
 *  @wq_stack_sz: 工作队列任务的栈大小。
 *  @wq_fifo_cap: 工作队列FIFO的容量。
 *
 * 返回值:
 *  @struct pl_workqueue*: 创建的工作队列的句柄。
 ***********************************************************************************/
struct pl_workqueue *pl_workqueue_create(const char *name, u16_t prio,
                                         size_t wq_stack_sz, u32_t wq_fifo_cap);

/**********************************************************************************
 * 函数名称: pl_workqueue_destroy
 * 描述: 销毁一个工作队列。
 *
 * 参数:
 *  @workqueue: 要销毁的工作队列的句柄。
 *
 * 返回值:
 *  成功时返回大于等于0的值，失败时返回小于0的值。
 ***********************************************************************************/
int pl_workqueue_destroy(struct pl_workqueue *workqueue);

/**********************************************************************************
 * 函数名称: pl_work_init
 * 描述: 初始化一个工作项（work）结构体。
 *
 * 参数:
 *  @work: 工作项的指针，需要被初始化。
 *
 * 返回值:
 *  成功时返回大于等于0的值，失败时返回小于0的值。
 ***********************************************************************************/
int pl_work_init(struct pl_work *work, pl_work_fun_t fun, void *priv_data);

/**********************************************************************************
 * 函数名称: pl_work_add
 * 描述: 将一个工作项添加到工作队列中。
 *
 * 参数:
 *  @workqueue: 工作队列的句柄。
 *  @work: 要添加的工作项。
 *
 * 返回值:
 *  成功时返回大于等于0的值，失败时返回小于0的值。
 ***********************************************************************************/
int pl_work_add(struct pl_workqueue *workqueue, struct pl_work *work);

/**********************************************************************************
 * 函数名称: pl_work_cancel
 * 描述: 取消工作队列中的一个工作项。
 *
 * 参数:
 *  @workqueue: 工作队列的句柄。
 *  @work: 要取消的工作项。
 *
 * 返回值:
 *  成功时返回大于等于0的值，失败时返回小于0的值。
 ***********************************************************************************/
int pl_work_cancel(struct pl_workqueue *workqueue, struct pl_work *work);

/**********************************************************************************
 * 函数名称: pl_work_get_private_data
 * 描述: 获取工作项的私有数据。
 *
 * 参数:
 *  @work: 工作项的指针。
 *
 * 返回值:
 *   工作项所关联的私有数据。
 ***********************************************************************************/
void *pl_work_get_private_data(struct pl_work *work);

#endif /* __KERNEL_WORKQUEUE_H__ */
