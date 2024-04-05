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
#include <types.h>
#include <config.h>
#include <kernel/mempool.h>
#include <kernel/syslog.h>
#include "workqueue.h"
#include "task.h"

struct workqueue pl_sys_hiwq;
struct workqueue pl_sys_lowq;
pl_wq_handle g_pl_sys_hiwq_handle = (pl_wq_handle)&pl_sys_hiwq;
pl_wq_handle g_pl_sys_lowq_handle = (pl_wq_handle)&pl_sys_lowq;

static int workqueue_task(int argc, char **argv)
{
	USED(argc);
	USED(argv);
	pl_work_fun_t work_fun;
	struct work *first;
	struct workqueue *wq = (struct workqueue *)argv[0];

	while (true) {
		if (list_is_empty(&wq->work_list)) {
			pl_task_pend(NULL);
			continue;
		}

		pl_enter_critical();
		first = list_first_entry(&wq->work_list, struct work, node);
		list_del_node(&first->node);
		list_init(&first->node);
		work_fun = first->fun;
		first->fun = NULL;
		pl_exit_critical();

		if (work_fun != NULL)
			work_fun(first);
	}

	return 0;
}

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
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_workqueue_init(struct workqueue *wq, const char *name, u16_t prio, size_t wq_stack_sz)
{
	char *argv[1];

	if (wq == NULL)
		return -EFAULT;

	argv[0] = (char *)wq;
	wq->name = name;
	list_init(&wq->work_list);
	wq->exec_thread = pl_task_create(name, workqueue_task, prio, wq_stack_sz, 1, argv);
	if (wq->exec_thread == NULL)
		return ERROR;

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
 *
 * Return:
 *  @pl_wq_handle: handle of workqueue requested.
 ************************************************************************************/
pl_wq_handle pl_workqueue_create(const char *name, u16_t prio, size_t wq_stack_sz)
{
	struct workqueue *wq;
	char *argv[1];

	wq = pl_mempool_malloc(g_pl_default_mempool, sizeof(struct workqueue));
	if (wq == NULL)
		return NULL;

	argv[0] = (char *)wq;
	list_init(&wq->work_list);
	wq->name = name;
	wq->exec_thread = pl_task_create(name, workqueue_task, prio, wq_stack_sz, 1, argv);
	if (wq->exec_thread == NULL)
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
int pl_workqueue_destroy(pl_wq_handle workqueue)
{
	struct workqueue *wq = (struct workqueue *)workqueue;

	if (wq == NULL || wq == &pl_sys_hiwq || wq == &pl_sys_lowq)
		return -EFAULT;

	(void)pl_task_kill(wq->exec_thread);
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
int pl_work_init(pl_work_handle work, pl_work_fun_t fun, void *priv_data)
{
	struct work *wk = (struct work *)work;

	if (wk == NULL || fun == NULL)
		return -EFAULT;

	wk->fun = fun;
	wk->priv_data = priv_data;
	list_init(&wk->node);

	return OK;
}

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
int pl_work_add(pl_wq_handle workqueue, pl_work_handle work)
{
	struct work *wk = (struct work *)work;
	struct workqueue *wq = (struct workqueue *)workqueue;

	if (wk == NULL || wq == NULL)
		return -EFAULT;

	if (!list_is_empty(&wk->node) && wk->fun != NULL)
		return OK;

	pl_enter_critical();
	list_add_node_at_tail(&wq->work_list, &wk->node);
	pl_exit_critical();

	return OK;
}

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
int pl_work_cancel(pl_wq_handle workqueue, pl_work_handle work)
{
	struct work *wk = (struct work *)work;
	struct workqueue *wq = (struct workqueue *)workqueue;

	if (wk == NULL || wq == NULL)
		return -EFAULT;

	if (list_is_empty(&wk->node)) {
		if (wk->fun != NULL)
			return OK;

		return ERROR;
	}

	pl_enter_critical();
	list_del_node(&wk->node);
	pl_exit_critical();

	return OK;
}

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
int pl_sys_wq_init(void)
{
	int ret;

	ret = pl_workqueue_init(&pl_sys_hiwq, "pl_sys_hiwq",
	                        1, PL_CFG_HI_WORKQUEUE_TASK_STACK_SIZE);
	if (ret < 0) {
		g_pl_sys_hiwq_handle = NULL;
		pl_early_syslog_err("hi workqueue request failed, ret:%d\r\n", ret);
		return ret;
	}

	ret = pl_workqueue_init(&pl_sys_lowq, "pl_sys_lowq",
	                        PL_CFG_LO_WORKQUEUE_TASK_PRIORITY,
	                        PL_CFG_LO_WORKQUEUE_TASK_STACK_SIZE);
	if (ret < 0) {
		g_pl_sys_lowq_handle = NULL;
		pl_early_syslog_err("hi workqueue request failed, ret:%d\r\n", ret);
		return ret;
	}

	pl_early_syslog_info("sys workqueue init successfully\r\n");
	return OK;
}
