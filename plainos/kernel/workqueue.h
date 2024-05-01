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

#ifndef __KERNEL_WORKQUEUE_PRIVATE_H__
#define __KERNEL_WORKQUEUE_PRIVATE_H__

#include <kernel/kernel.h>
#include <kernel/task.h>
#include <kernel/kfifo.h>
#include <kernel/workqueue.h>

struct work {
	pl_work_fun_t fun;
	void *priv_data;
};

struct workqueue {
	tid_t exec_thread;
	u32_t fifo_cap;
	u32_t fifo_in;
	u32_t fifo_out;
	const char *name;
	struct work **fifo;
};

/*************************************************************************************
 * Function Name: pl_sys_wq_init
 *
 * Description:
 *   initialize a workqueue in the system.
 * 
 * Parameters:
 *  none
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_sys_wq_init(void);

/*************************************************************************************
 * Function Name: pl_workqueue_init
 *
 * Description:
 *   init a workqueue.
 * 
 * Parameters:
 *  @wq: workqueue struct address.
 *  @name: name of workqueue.
 *  @prio: priority of workqueue.
 *  @wq_stack_sz: workqueue task stack size.
 *  @wq_fifo_cap: capacity of workqueue fifo.
 *  @wq_fifo: fifo buffer of workqueue.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_workqueue_init(struct workqueue *wq, const char *name, u16_t prio,
            size_t wq_stack_sz, u32_t wq_fifo_cap, struct work **wq_fifo);

#endif /* __KERNEL_WORKQUEUE_PRIVATE_H__ */
