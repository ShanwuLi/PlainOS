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

#ifndef __KERNEL_SOFTTIMER_PRIVATE_H__
#define __KERNEL_SOFTTIMER_PRIVATE_H__

#include <kernel/kernel.h>
#include <kernel/task.h>
#include <kernel/list.h>
#include <kernel/semaphore.h>
#include <kernel/workqueue.h>
#include "semaphore.h"

struct workqueue {
	struct list_node work_list;
	tid_t exec_thread;
	const char *name;
};

struct work {
	struct list_node node;
	pl_work_fun_t fun;
	void *priv_data;
};

/*************************************************************************************
 * Function Name: pl_sys_wq_init
 *
 * Description:
 *   initialize a workqueue in the system.
 * 
 * Parameters:
 *  @workqueue: workqueue handle.
 *  @work: work.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_sys_wq_init(void);

/*************************************************************************************
 * Function Name: pl_workqueue_request
 *
 * Description:
 *   request a workqueue.
 * 
 * Parameters:
 *  @name: workqueue name.
 *  @proi: priority of workqueue.
 *  @wq_stack_sz: workqueue task stack size.
 *
 * Return:
 *  @pl_wq_handle: handle of workqueue requested.
 ************************************************************************************/
int pl_workqueue_init(struct workqueue *wq, const char *name, int prio, size_t wq_stack_sz);

#endif /* __KERNEL_SOFTTIMER_PRIVATE_H__ */
