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

/*************************************************************************************
 * Function Name: pl_workqueue_create
 *  @g_pl_sys_hiwq_handle: handle of the hight priority workqueue.
 *  @g_pl_sys_lowq_handle: handle of the low priority workqueue.
 *
 * Description:
 *   create a workqueue.
 ************************************************************************************/
extern struct pl_workqueue *g_pl_sys_hiwq_handle;
extern struct pl_workqueue *g_pl_sys_lowq_handle;

/*************************************************************************************
 * Function Name: pl_workqueue_create
 *
 * Description:
 *   create a workqueue.
 * 
 * Parameters:
 *  @name: workqueue name.
 *  @proi: priority of workqueue.
 *  @wq_stack_sz: workqueue task stack size.
 *  @wq_fifo_cap: capacity of workqueue fifo.
 *
 * Return:
 *  @struct pl_workqueue*: handle of workqueue requested.
 ************************************************************************************/
struct pl_workqueue *pl_workqueue_create(const char *name, u16_t prio,
                               size_t wq_stack_sz, u32_t wq_fifo_cap);

/*************************************************************************************
 * Function Name: pl_workqueue_destroy
 *
 * Description:
 *   destroy a workqueue.
 * 
 * Parameters:
 *  @workqueue: workqueue handle.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_workqueue_destroy(struct pl_workqueue *workqueue);

/*************************************************************************************
 * Function Name: pl_work_init
 *
 * Description:
 *   initialize a work.
 * 
 * Parameters:
 *  @work: work.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_work_init(struct pl_work *work, pl_work_fun_t fun, void *priv_data);

/*************************************************************************************
 * Function Name: pl_work_add
 *
 * Description:
 *   add a work to the workqueue.
 * 
 * Parameters:
 *  @workqueue: workqueue handle.
 *  @work: work.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_work_add(struct pl_workqueue *workqueue, struct pl_work *work);

/*************************************************************************************
 * Function Name: pl_work_cancel
 *
 * Description:
 *   cancel a work int the workqueue.
 * 
 * Parameters:
 *  @workqueue: workqueue handle.
 *  @work: work.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_work_cancel(struct pl_workqueue *workqueue, struct pl_work *work);

/*************************************************************************************
 * Function Name: pl_work_get_private_data
 *
 * Description:
 *   get the private data of work.
 * 
 * Parameters:
 *  @work: work.
 *
 * Return:
 *   private data of the work.
 ************************************************************************************/
void *pl_work_get_private_data(struct pl_work *work);

#endif /* __KERNEL_WORKQUEUE_H__ */
