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
#include "softtimer.h"
#include "task.h"

static struct pl_stimer_ctrl pl_stimer_ctrl;

/*************************************************************************************
 * Function Name: softtimer_consume_node
 *
 * Description:
 *   consume a node of softtimer.
 * 
 * Parameters:
 *  @first: the first of softtimer list node.
 *
 * Return:
 *  callback function of softtimer.
 ************************************************************************************/
static pl_stimer_fun_t softtimer_consume_node(struct pl_stimer *first)
{
	pl_stimer_fun_t stimer_fun;

	list_del_node(&first->node);
	list_init(&first->node);
	stimer_fun = first->fun;
	first->fun = NULL;

	return stimer_fun;
}

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
	bool is_empty;
	struct pl_stimer *first;
	pl_stimer_fun_t stimer_fun;

	while (true) {
		/* get the timer_ctrl_list status */
		pl_port_enter_critical();
		is_empty = list_is_empty(&pl_stimer_ctrl.head);
		pl_port_exit_critical();

		/* if list of softtimer is empty, we need to suspend daemon task */
		if (is_empty) {
			pl_task_pend(NULL);
			continue;
		}

		/* get the first node from the softtimer list */
		pl_port_enter_critical();
		first = list_first_entry(&pl_stimer_ctrl.head, struct pl_stimer, node);
		stimer_fun = softtimer_consume_node(first);
		pl_port_exit_critical();

		/* call callback */
		if (stimer_fun != NULL)
			stimer_fun(first);

		/* reload softtimer */
		if (first->reload) {
			pl_softtimer_timer_init(first, first->fun, first->timing_cnt,
			                        first->priv_data);
			pl_softtimer_start(first);
		}
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
 *  @struct pl_stimer*: handle of soft timer requested.
 ************************************************************************************/
struct pl_stimer *pl_softtimer_request(const char *name)
{
	struct pl_stimer *timer;

	timer = pl_mempool_malloc(g_pl_default_mempool, sizeof(struct pl_stimer));
	if (timer == NULL) {
		return NULL;
	}

	timer->name = name;
	timer->reload = false;
	list_init(&timer->node);

	return timer;
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
int pl_softtimer_get_private_data(struct pl_stimer *timer, void **data)
{
	if (timer == NULL || data == NULL)
		return -EFAULT;

	pl_port_enter_critical();
	*data = timer->priv_data;
	pl_port_exit_critical();

	return OK;
}

/*************************************************************************************
 * Function Name: pl_softtimer_timer_init
 *
 * Description:
 *   init soft timer.
 * 
 * Parameters:
 *  @timer: handle of soft timer requested.
 *  @fun: callback function.
 *  @timing_cnt: the count of timing.
 *  @priv_data: private data.
 *
 * Return:
 *  void.
 ************************************************************************************/
void pl_softtimer_timer_init(struct pl_stimer *stimer, pl_stimer_fun_t fun,
                                         u64_t timing_cnt, void *priv_data)
{
	stimer->fun = fun;
	stimer->priv_data = priv_data;
	stimer->timing_cnt = timing_cnt;
	stimer->reach_cnt = 0;
}

/*************************************************************************************
 * Function Name: pl_softtimer_start
 *
 * Description:
 *   start soft timer.
 * 
 * Parameters:
 *  @timer: handle of soft timer requested.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_softtimer_start(struct pl_stimer *timer)
{
	u64_t syscount;
	struct pl_stimer *pos;
	struct list_node *timer_list;

	if (timer == NULL)
		return -EFAULT;

	if (!list_is_empty(&timer->node))
		return -EBUSY;

	pl_task_get_syscount(&syscount);
	timer->reach_cnt = syscount + timer->timing_cnt;
	timer_list = pl_task_get_timer_list();

	pl_port_enter_critical();
	if (list_is_empty(timer_list)) {
		list_add_node_at_tail(timer_list, &timer->node);
		goto out;
	}

	list_for_each_entry(pos, timer_list, struct pl_stimer, node) {
		if (timer->reach_cnt < pos->reach_cnt)
			break;
	}

	list_add_node_ahead(&pos->node, &timer->node);
out:
	pl_port_exit_critical();
	return OK;
}

/*************************************************************************************
 * Function Name: pl_softtimer_reload
 *
 * Description:
 *   reload soft timer.
 * 
 * Parameters:
 *  @timer: handle of soft timer requested.
 *  @reload: reload whether or not.
 *  @fun: callback function.
 *  @timing_cnt: the count of timing.
 *  @priv_data: private data.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_softtimer_reload(struct pl_stimer *timer, bool reload, pl_stimer_fun_t fun,
                                                u64_t timing_cnt, void *priv_data)
{
	if (timer == NULL || fun == NULL || timing_cnt == 0)
		return -EFAULT;

	pl_softtimer_timer_init(timer, fun, timing_cnt, priv_data);
	timer->reload = reload;

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
int pl_softtimer_cancel(struct pl_stimer *timer)
{
	if (timer == NULL || timer->fun == NULL)
		return -EFAULT;

	if (list_is_empty(&timer->node))
		return -EEMPTY;

	pl_port_enter_critical();
	list_del_node(&timer->node);
	pl_port_exit_critical();

	timer->fun = NULL;
	timer->reload = false;
	list_init(&timer->node);

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
void pl_softtimer_release(struct pl_stimer *timer)
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
struct pl_stimer_ctrl *pl_softtimer_get_ctrl(void)
{
	return &pl_stimer_ctrl;
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
	list_init(&pl_stimer_ctrl.head);
	pl_stimer_ctrl.daemon = pl_task_sys_create("softtimer_daemon",
	                        softtimer_daemon_task, 0,
	                        CONFIG_PL_CFG_SOFTTIMER_DAEMON_TASK_STACK_SIZE, 0, NULL);
	if (pl_stimer_ctrl.daemon == NULL) {
		pl_syslog_err("soft timer init failed\r\n");
		return -EFAULT;
	}

	pl_syslog_info("soft timer init done\r\n");
	return 0;
}
