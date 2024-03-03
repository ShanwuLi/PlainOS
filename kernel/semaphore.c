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
#include <port.h>
#include <kernel/list.h>
#include <kernel/kernel.h>
#include <kernel/semaphore.h>
#include "task_private.h"
#include <kernel/syslog.h>

/*************************************************************************************
 * Function Name: pl_semaplore_init
 *
 * Description:
 *   initialize semaphore.
 * 
 * Parameters:
 *  @semap: semaphore pointer;
 *  @val: value of semaphore.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_semaplore_init(struct semaphore *semap, int val)
{
	if (semap == NULL)
		return -EFAULT;

	semap->value = val;
	list_init(&semap->wait_list);
	return OK;
}

/*************************************************************************************
 * Function Name: pl_semaplore_take
 *
 * Description:
 *    take semaphore.
 * 
 * Parameters:
 *  @semap: semaphore pointer;
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_semaplore_take(struct semaphore *semap)
{
	struct tcb *curr_tcb;

	if (semap == NULL)
		return -EFAULT;

	pl_enter_critical();
	--semap->value;
	if (semap->value < 0) {
		curr_tcb = pl_task_get_curr_tcb();
		curr_tcb->curr_state = PL_TASK_STATE_WAITING;
		curr_tcb->past_state = PL_TASK_STATE_READY;
		pl_task_remove_tcb_from_rdylist(curr_tcb);
		list_add_node_at_tail(&semap->wait_list, &curr_tcb->node);
		pl_exit_critical();
		pl_task_context_switch();
		return OK;
	}

	pl_exit_critical();
	return OK;
}

/*************************************************************************************
 * Function Name: pl_semaplore_give
 *
 * Description:
 *    give semaphore.
 * 
 * Parameters:
 *  @semap: semaphore pointer;
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_semaplore_give(struct semaphore *semap)
{
	struct tcb *front_tcb;
	struct list_node *front_node;
	
	if (semap == NULL)
		return -EFAULT;

	pl_enter_critical();
	++semap->value;
	if (semap->value <= 0) {
		front_node = list_del_front_node(&semap->wait_list);
		front_tcb = container_of(front_node, struct tcb, node);
		front_tcb->curr_state = PL_TASK_STATE_READY;
		front_tcb->past_state = PL_TASK_STATE_WAITING;
		pl_task_insert_tcb_to_rdylist(front_tcb);
		pl_exit_critical();
		pl_task_context_switch();
		return OK;
	}

	pl_exit_critical();
	return OK;
}
