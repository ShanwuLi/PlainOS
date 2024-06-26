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
#include <kernel/semaphore.h>
#include "task.h"

/*************************************************************************************
 * Function Name: pl_semaphore_init
 *
 * Description:
 *   initialize a semaphore.
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
	if (sem == NULL)
		return -EFAULT;

	pl_port_enter_critical();
	sem->value = val;
	list_init(&sem->wait_list);
	pl_port_exit_critical();

	return OK;
}

/*************************************************************************************
 * Function Name: pl_semaphore_reset
 *
 * Description:
 *   reset a semaphore.
 * 
 * Parameters:
 *  @sem: semaphore.
 *  @val: value of semaphore.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure..
 ************************************************************************************/
int pl_semaphore_reset(struct pl_sem *sem, int val)
{
	if (sem == NULL)
		return -EFAULT;
	
	pl_port_enter_critical();
	sem->value = val;
	list_init(&sem->wait_list);
	pl_port_exit_critical();

	return OK;
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

	if (sem == NULL)
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

	if (sem == NULL)
		return -EFAULT;

	pl_port_enter_critical();
	++sem->value;
	if (sem->value <= 0) {
		front_tcb = list_first_entry(&sem->wait_list, struct tcb, node);
		pl_task_remove_tcb_from_waitlist(front_tcb);
		pl_task_insert_tcb_to_rdylist(front_tcb);
		pl_port_exit_critical();
		pl_task_context_switch();
		return OK;
	}

	pl_port_exit_critical();
	return OK;
}
