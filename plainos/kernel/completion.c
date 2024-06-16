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

#include <config.h>
#include <errno.h>
#include <port/port.h>
#include <kernel/list.h>
#include <kernel/kernel.h>
#include <kernel/completion.h>
#include "task.h"

/*************************************************************************************
 * Function Name: pl_completion_init
 *
 * Description:
 *   request a semaphore.
 *
 * Parameters:
 *  @comp: completion handle.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure..
 ************************************************************************************/
int pl_completion_init(struct pl_completion *comp)
{
	if (comp == NULL)
		return -EFAULT;
	
	pl_port_enter_critical();
	comp->done = 0;
	list_init(&comp->wait_list);
	pl_port_exit_critical();

	return OK;
}

/*************************************************************************************
 * Function Name: pl_completion_wait
 *
 * Description:
 *    wait a completion.
 * 
 * Parameters:
 *  @comp: completion handle.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_completion_wait(struct pl_completion *comp)
{
	struct tcb *curr_tcb;

	if (comp == NULL)
		return -EFAULT;

	pl_port_enter_critical();
	if (comp->done) {
		comp->done = 0;
		pl_port_exit_critical();
		return OK;
	}

	curr_tcb = pl_task_get_curr_tcb();
	pl_task_remove_tcb_from_rdylist(curr_tcb);
	pl_task_insert_tcb_to_waitlist(&comp->wait_list, curr_tcb);
	pl_port_exit_critical();
	pl_task_context_switch();

	return OK;
}

/*************************************************************************************
 * Function Name: pl_completion_post
 *
 * Description:
 *    post a completion.
 * 
 * Parameters:
 *  @comp: completion handle.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_completion_post(struct pl_completion *comp)
{
	struct tcb *front_tcb;

	if (comp == NULL)
		return -EFAULT;

	pl_port_enter_critical();
	if (list_is_empty(&comp->wait_list)) {
		comp->done = 1;
		pl_port_exit_critical();
		return OK;
	}

	front_tcb = list_first_entry(&comp->wait_list, struct tcb, node);
	pl_task_remove_tcb_from_waitlist(front_tcb);
	pl_task_insert_tcb_to_rdylist(front_tcb);
	pl_port_exit_critical();
	pl_task_context_switch();

	return OK;
}

/*************************************************************************************
 * Function Name: pl_completion_post_all
 *
 * Description:
 *    post all completion.
 * 
 * Parameters:
 *  @comp: completion handle.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_completion_post_all(struct pl_completion *comp)
{
	struct tcb *front;
	struct tcb *temp;

	if (comp == NULL)
		return -EFAULT;

	pl_port_enter_critical();
	if (list_is_empty(&comp->wait_list)) {
		comp->done = 1;
		pl_port_exit_critical();
		return OK;
	}

	/* post all waiters */
	list_for_each_entry_safe(front, temp, &comp->wait_list, struct tcb, node) {
		pl_task_remove_tcb_from_waitlist(front);
		pl_task_insert_tcb_to_rdylist(front);
	}

	pl_port_exit_critical();
	pl_task_context_switch();

	return OK;
}