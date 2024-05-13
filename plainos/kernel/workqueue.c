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

#include <errno.h>
#include <port.h>
#include <types.h>
#include <config.h>
#include <kernel/mempool.h>
#include <kernel/syslog.h>
#include "workqueue.h"
#include "task.h"

static struct pl_workqueue pl_sys_hiwq;
static struct pl_workqueue pl_sys_lowq;
static struct pl_work *pl_sys_hiwq_fifo[PL_CFG_HI_WORKQUEUE_FIFO_CAPACITY];
static struct pl_work *pl_sys_lowq_fifo[PL_CFG_LO_WORKQUEUE_FIFO_CAPACITY];

struct pl_workqueue *g_pl_sys_hiwq_handle = &pl_sys_hiwq;
struct pl_workqueue *g_pl_sys_lowq_handle = &pl_sys_lowq;

static int workqueue_task(int argc, char **argv)
{
	USED(argc);
	USED(argv);
	struct pl_work *first;
	struct pl_workqueue *wq = (struct pl_workqueue *)argv;

	while (true) {
		/* if work fifo is empty, we need to suspend wq task */
		if (wq->fifo_out == wq->fifo_in) {
			pl_task_pend(NULL);
			continue;
		}

		/* get the first work */
		pl_port_enter_critical();
		first = wq->fifo[wq->fifo_out & (wq->fifo_cap - 1)];
		++wq->fifo_out;
		pl_port_exit_critical();

		/* call fun of callback */
		if (first->fun != NULL)
			first->fun(first);
	}

	return 0;
}

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
static int pl_workqueue_init(struct pl_workqueue *wq, const char *name, u16_t prio,
                   size_t wq_stack_sz, u32_t wq_fifo_cap, struct pl_work **wq_fifo)
{
	if (wq == NULL || wq_fifo == NULL)
		return -EFAULT;

	wq->fifo_in = 0;
	wq->fifo_out = 0;
	wq->fifo_cap = wq_fifo_cap;
	wq->fifo = wq_fifo;
	wq->name = (name == NULL) ? "anonymous wq" : name;

	wq->exec_thread = pl_task_sys_create(name, workqueue_task, prio,
	                                     wq_stack_sz, 1, (char **)wq);
	if (wq->exec_thread == NULL)
		return -EUNKNOWE;

	return OK;
}

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
                                         size_t wq_stack_sz, u32_t wq_fifo_cap)
{
	int ret;
	struct pl_workqueue *wq;

	if (!is_power_of_2(wq_fifo_cap) || wq_fifo_cap == 0)
		return NULL;

	wq = pl_mempool_malloc(g_pl_default_mempool, sizeof(struct pl_workqueue) +
	                       sizeof(struct pl_work *) * wq_fifo_cap);
	if (wq == NULL)
		return NULL;

	ret = pl_workqueue_init(wq, name, prio, wq_stack_sz, wq_fifo_cap,
	                        (struct pl_work **)(wq + 1));
	if (ret < 0)
		return NULL;

	return wq;
}

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
int pl_workqueue_destroy(struct pl_workqueue *wq)
{
	int ret;

	if (wq == NULL || wq == &pl_sys_hiwq || wq == &pl_sys_lowq)
		return -EFAULT;

	ret = pl_task_kill(wq->exec_thread);
	if (ret < 0)
		return -EINVAL;

	pl_mempool_free(g_pl_default_mempool, wq);
	return OK;
}

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
int pl_work_init(struct pl_work *wk, pl_work_fun_t fun, void *priv_data)
{
	if (wk == NULL || fun == NULL)
		return -EFAULT;

	wk->fun = fun;
	wk->priv_data = priv_data;

	return OK;
}

/*************************************************************************************
 * Function Name: pl_work_add
 *
 * Description:
 *   add a work to the workqueue.
 * 
 * Parameters:
 *  @wq: workqueue handle.
 *  @wk: work.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_work_add(struct pl_workqueue *wq, struct pl_work *wk)
{
	if (wk == NULL || wq == NULL)
		return -EFAULT;

	if (wq->fifo_cap - ((wq->fifo_in - wq->fifo_out) & (wq->fifo_cap - 1)) <= 1)
		return -EFULL;

	pl_port_enter_critical();
	wq->fifo[wq->fifo_in & (wq->fifo_cap - 1)] = wk;
	++wq->fifo_in;
	pl_port_exit_critical();

	pl_task_resume(wq->exec_thread);
	return OK;
}

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
void *pl_work_get_private_data(struct pl_work *wk)
{
	if (wk == NULL)
		return NULL;

	return wk->priv_data;
}

/*************************************************************************************
 * Function Name: pl_sys_wq_init
 *
 * Description:
 *   initialize a workqueue in the system.
 * 
 * Parameters:
 *   none.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_sys_wq_init(void)
{
	int ret;

	ret = pl_workqueue_init(&pl_sys_hiwq, "pl_sys_hiwq", 1,
	                        PL_CFG_HI_WORKQUEUE_TASK_STACK_SIZE,
	                        PL_CFG_HI_WORKQUEUE_FIFO_CAPACITY,
	                        pl_sys_hiwq_fifo);
	if (ret < 0) {
		g_pl_sys_hiwq_handle = NULL;
		pl_early_syslog_err("hi workqueue request failed, ret:%d\r\n", ret);
		return ret;
	}

	ret = pl_workqueue_init(&pl_sys_lowq, "pl_sys_lowq",
	                        PL_CFG_LO_WORKQUEUE_TASK_PRIORITY,
	                        PL_CFG_LO_WORKQUEUE_TASK_STACK_SIZE,
	                        PL_CFG_LO_WORKQUEUE_FIFO_CAPACITY,
	                        pl_sys_lowq_fifo);
	if (ret < 0) {
		g_pl_sys_lowq_handle = NULL;
		pl_early_syslog_err("hi workqueue request failed, ret:%d\r\n", ret);
		return ret;
	}

	pl_early_syslog_info("sys workqueue init done\r\n");
	return OK;
}
