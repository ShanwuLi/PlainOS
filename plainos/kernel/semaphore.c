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
#include <kernel/syslog.h>
#include <kernel/mempool.h>
#include "task.h"

/*************************************************************************************
 * Function Name: pl_semaphore_init
 *
 * Description:
 *   request a semaphore.
 * 
 * Parameters:
 *  @semap: semaphore.
 *  @val: value of semaphore.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_semaphore_init(struct pl_sem *sem, int val)
{
	if (sem == NULL) {
		pl_syslog_err("pl_sem alloc failed\r\n");
		return -EFAULT;
	}

	sem->valid = true;
	sem->value = val;
	list_init(&sem->wait_list);
	return OK;
}

/*************************************************************************************
 * Function Name: pl_semaphore_request
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
struct pl_sem *pl_semaphore_request(int val)
{
	struct pl_sem *sem;

	sem = pl_mempool_malloc(g_pl_default_mempool, sizeof(struct pl_sem));
	if (sem == NULL) {
		pl_syslog_err("pl_sem alloc failed\r\n");
		return NULL;
	}

	sem->valid = true;
	sem->value = val;
	list_init(&sem->wait_list);
	return sem;
}

/*************************************************************************************
 * Function Name: pl_semaphore_wait
 *
 * Description:
 *    take semaphore.
 * 
 * Parameters:
 *  @sem: semaphore handle.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_semaphore_wait(struct pl_sem *sem)
{
	struct tcb *curr_tcb;

	if (sem == NULL || (!sem->valid))
		return -EFAULT;

	pl_port_enter_critical();
	--sem->value;
	if (sem->value < 0) {
		curr_tcb = pl_task_get_curr_tcb();
		pl_task_remove_tcb_from_rdylist(curr_tcb);
		pl_task_insert_tcb_to_waitlist(&sem->wait_list, curr_tcb);
		pl_port_exit_critical();
		pl_task_context_switch();
		return OK;
	}

	pl_port_exit_critical();
	return OK;
}

/*************************************************************************************
 * Function Name: pl_semaphore_post
 *
 * Description:
 *    give semaphore.
 * 
 * Parameters:
 *  @sem: semaphore handle.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_semaphore_post(struct pl_sem *sem)
{
	struct tcb *front_tcb;
	struct list_node *front_node;

	if (sem == NULL || (!sem->valid))
		return -EFAULT;

	pl_port_enter_critical();
	++sem->value;
	if (sem->value <= 0) {
		front_node = list_del_front_node(&sem->wait_list);
		front_tcb = container_of(front_node, struct tcb, node);
		pl_task_insert_tcb_to_rdylist(front_tcb);
		pl_port_exit_critical();
		pl_task_context_switch();
		return OK;
	}

	pl_port_exit_critical();
	return OK;
}

/*************************************************************************************
 * Function Name: pl_semaphore_release
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
void pl_semaphore_release(struct pl_sem *sem)
{
	pl_port_enter_critical();
	if (sem == NULL || (!sem->valid)) {
		pl_port_exit_critical();
		return;
	}

	sem->valid = false;
	pl_port_exit_critical();
	pl_mempool_free(g_pl_default_mempool, sem);
}
