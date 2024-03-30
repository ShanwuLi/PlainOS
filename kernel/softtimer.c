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
#include "softtimer.h"
#include "task.h"

static struct softtimer_ctrl softtimer_ctrl;

/*************************************************************************************
 * Function Name: softtimer_daemon_task
 *
 * Description:
 *   daemon task of soft timer.
 * 
 * Parameters:
 *  none.
 *
 * Return:
 *  none.
 ************************************************************************************/
static int softtimer_daemon_task(int argc, char **argv)
{
	USED(argc);
	USED(argv);
	struct softtimer *first;
	stimer_fun_t stimer_fun;

	while (true) {
		if (list_is_empty(&softtimer_ctrl.head)) {
			pl_task_pend(softtimer_ctrl.daemon);
			continue;
		}

		pl_enter_critical();
		first = list_first_entry(&softtimer_ctrl.head, struct softtimer, node);
		list_del_node(&first->node);
		list_init(&first->node);
		stimer_fun = first->fun;
		first->fun = NULL;
		pl_exit_critical();

		if (stimer_fun != NULL)
			stimer_fun(first);
	}

	return 0;
}

/*************************************************************************************
 * Function Name: pl_softtimer_request
 *
 * Description:
 *   request a soft timer.
 * 
 * Parameters:
 *  @name: timer name.
 *
 * Return:
 *  @stimer_handle_t: handle of soft timer requested.
 ************************************************************************************/
stimer_handle_t pl_softtimer_request(const char *name)
{
	struct softtimer *timer;

	timer = pl_mempool_malloc(g_pl_default_mempool, sizeof(struct softtimer));
	if (timer == NULL) {
		return NULL;
	}

	timer->name = name;
	list_init(&timer->node);

	return (stimer_handle_t)timer;
}

/*************************************************************************************
 * Function Name: pl_softtimer_get_private_data
 *
 * Description:
 *   get private data of soft timer.
 * 
 * Parameters:
 *  @timer: handle of soft timer requested.
 *  @data: private data addr.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_softtimer_get_private_data(stimer_handle_t timer, void **data)
{
	if (timer == NULL || data == NULL)
		return -EFAULT;

	pl_enter_critical();
	*data = ((struct softtimer *)timer)->priv_data;
	pl_exit_critical();

	return OK;
}

/*************************************************************************************
 * Function Name: pl_softtimer_start
 *
 * Description:
 *   start soft timer.
 * 
 * Parameters:
 *  @timer: handle of soft timer requested.
 *  @fun: callback function.
 *  @timing_cnt: the count of timing.
 *  @priv_data: private data.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_softtimer_start(stimer_handle_t timer, stimer_fun_t fun,
                       struct count *timing_cnt, void *priv_data)
{
	struct softtimer *pos;
	struct count syscount;
	struct softtimer *stimer = (struct softtimer *)timer;
	struct list_node *timer_list = pl_task_get_timer_list();

	if (stimer == NULL || fun == NULL || timing_cnt == NULL)
		return -EFAULT;

	if (!list_is_empty(&stimer->node))
		return -EBUSY;

	stimer->fun = fun;
	stimer->priv_data = priv_data;
	stimer->timing_cnt.hi32 = timing_cnt->hi32;
	stimer->timing_cnt.lo32 = timing_cnt->lo32;
	pl_task_get_syscount(&syscount);
	pl_count_add(&stimer->reach_cnt, &syscount, timing_cnt);

	pl_enter_critical();
	if (list_is_empty(timer_list)) {
		list_add_node_at_tail(timer_list, &stimer->node);
		goto out;
	}

	list_for_each_entry(pos, timer_list, struct softtimer, node) {
		if (pl_count_cmp(&stimer->reach_cnt, &pos->reach_cnt) < 0)
			break;
	}

	list_add_node_ahead(&pos->node, &stimer->node);
out:
	pl_exit_critical();
	return OK;
}

/*************************************************************************************
 * Function Name: pl_softtimer_cancel
 *
 * Description:
 *   cancel soft timer.
 * 
 * Parameters:
 *  @timer: handle of soft timer requested.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_softtimer_cancel(stimer_handle_t timer)
{
	struct softtimer *stimer = (struct softtimer *)timer;

	if (stimer == NULL || stimer->fun == NULL)
		return ERROR;

	if (list_is_empty(&stimer->node))
		return ERROR;

	pl_enter_critical();
	list_del_node(&stimer->node);
	stimer->fun = NULL;
	pl_exit_critical();
	list_init(&stimer->node);

	return OK;
}

/*************************************************************************************
 * Function Name: pl_softtimer_cancel
 *
 * Description:
 *   cancel soft timer.
 * 
 * Parameters:
 *  @timer: handle of soft timer requested.
 *
 * Return:
 *  void.
 ************************************************************************************/
void pl_softtimer_release(stimer_handle_t timer)
{
	if (timer == NULL)
		return;

	pl_mempool_free(g_pl_default_mempool, timer);
}

/*************************************************************************************
 * Function Name: pl_softtimer_get_ctrl
 *
 * Description:
 *   get timer controller.
 * 
 * Parameters:
 *  none.
 *
 * Return:
 *  @softtimer_ctrl: timer system controller.
 ************************************************************************************/
struct softtimer_ctrl *pl_softtimer_get_ctrl(void)
{
	return &softtimer_ctrl;
}

/*************************************************************************************
 * Function Name: pl_softtimer_core_init
 *
 * Description:
 *   init soft timer system.
 * 
 * Parameters:
 *  none.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_softtimer_core_init(void)
{
	list_init(&softtimer_ctrl.head);
	softtimer_ctrl.daemon = pl_task_create("softtimer_daemon", softtimer_daemon_task, 0,
	                        PL_CFG_SOFTTIMER_DAEMON_TASK_STACK_SIZE, 0, NULL);
	return 0;
}
