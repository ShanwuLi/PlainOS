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
#include <kernel/mempool.h>
#include "semaphore_private.h"

/*************************************************************************************
 * Function Name: pl_semaplore_init
 *
 * Description:
 *   request a semaphore.
 * 
 * Parameters:
 *  @semap: semaphore.
 *  @val: value of semaphore.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure..
 ************************************************************************************/
int pl_semaplore_init(struct semaphore *semap, int val)
{
	if (semap == NULL) {
		pl_syslog_err("pl_sem alloc failed\r\n");
		return -EFAULT;
	}

	semap->value = val;
	list_init(&semap->wait_list);
	return OK;
}

/*************************************************************************************
 * Function Name: pl_semaplore_request
 *
 * Description:
 *   request a semaphore.
 * 
 * Parameters:
 *  @val: value of semaphore.
 *
 * Return:
 *  @semaphore_handle.
 ************************************************************************************/
pl_semaphore_handle_t pl_semaplore_request(int val)
{
	struct semaphore *semap;

	semap = pl_mempool_malloc(g_pl_default_mempool, sizeof(struct semaphore));
	if (semap == NULL) {
		pl_syslog_err("pl_sem alloc failed\r\n");
		return ERR_TO_PTR(-EFAULT);
	}

	semap->value = val;
	list_init(&semap->wait_list);
	return semap;
}

/*************************************************************************************
 * Function Name: pl_semaplore_take
 *
 * Description:
 *    take semaphore.
 * 
 * Parameters:
 *  @semap: semaphore handle.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_semaplore_take(pl_semaphore_handle_t semap)
{
	struct tcb *curr_tcb;
	struct semaphore *sem = (struct semaphore *)semap;

	if (semap == NULL)
		return -EFAULT;

	pl_enter_critical();
	--sem->value;
	if (sem->value < 0) {
		curr_tcb = pl_task_get_curr_tcb();
		curr_tcb->curr_state = PL_TASK_STATE_WAITING;
		curr_tcb->past_state = PL_TASK_STATE_READY;
		pl_task_remove_tcb_from_rdylist(curr_tcb);
		list_add_node_at_tail(&sem->wait_list, &curr_tcb->node);
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
 *  @semap: semaphore handle.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_semaplore_give(pl_semaphore_handle_t semap)
{
	struct tcb *front_tcb;
	struct list_node *front_node;
	struct semaphore *sem = (struct semaphore *)semap;

	if (semap == NULL)
		return -EFAULT;

	pl_enter_critical();
	++sem->value;
	if (sem->value <= 0) {
		front_node = list_del_front_node(&sem->wait_list);
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

/*************************************************************************************
 * Function Name: pl_semaplore_release
 *
 * Description:
 *   release a semaphore.
 * 
 * Parameters:
 *   @semap: semaphore handle;
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_semaplore_release(pl_semaphore_handle_t semap)
{
	struct semaphore *sem = (struct semaphore *)semap;

	if (sem == NULL)
		return;

	sem->value = 0;
	pl_mempool_free(g_pl_default_mempool, sem);
}
