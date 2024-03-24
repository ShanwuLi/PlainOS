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

#ifndef __KERNEL_INTERNAL_TASK_H__
#define __KERNEL_INTERNAL_TASK_H__

#include <types.h>
#include <kernel/kernel.h>
#include <kernel/list.h>

/*************************************************************************************
 * Type Name: task_state
 * Description: task state definition.
 *
 ************************************************************************************/
enum task_state {
	PL_TASK_STATE_READY = 0,
	PL_TASK_STATE_DELAY,
	PL_TASK_STATE_WAITING,
	PL_TASK_STATE_PENDING,
	PL_TASK_STATE_EXIT,
	PL_TASK_STATE_FATAL,
};

/*************************************************************************************
 * Structure Name: tcb
 * Description: task controller block.
 *
 * Members:
 *   @context_sp: task stack pointer.
 *   @name: task name.
 *   @parent: pointer to parent task.
 *   @task: task routine.
 *   @argc: count of argv.
 *   @argv: arguments vector.
 *   @node: list node of the same priority tcb.
 *   @past_state: past state of system.
 *   @curr_state: current state of system.
 *   @prio: priority of the task, support priority up to 4096.
 *   @signal: signal of the task received.
 *   @delay_ticks: high/low 32bit ticks of delay.
 *
 ************************************************************************************/
struct tcb {
	void *context_sp;
	const char *name;
	struct tcb *parent;
	main_t task;
	int argc;
	char **argv;
	int wait_for_task_ret;
	struct list_node wait_head;
	struct list_node node;
	u8_t past_state;
	u8_t curr_state;
	u16_t prio;
	uint_t signal;
	struct count delay_ticks;
};

typedef void (*task_entry_t)(struct tcb *tcb);

/*************************************************************************************
 * Function Name: pl_task_get_curr_tcb
 * Description: Get current tcb.
 *
 * Parameters:
 *   none.
 *
 * Return:
 *   @struct tcb: current tcb.
 ************************************************************************************/
struct tcb *pl_task_get_curr_tcb(void);

/*************************************************************************************
 * Function Name: pl_task_get_timer_list
 * Description: Get timer list tcb.
 *
 * Parameters:
 *   none.
 *
 * Return:
 *   @struct list_node: timer list.
 ************************************************************************************/
struct list_node *pl_task_get_timer_list(void);

/*************************************************************************************
 * Function Name: pl_task_context_switch
 * Description: switch task.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   none
 ************************************************************************************/
void pl_task_context_switch(void);

/*************************************************************************************
 * Function Name: pl_task_core_init
 * Description: initialize task component.
 *
 * Parameters:
 *   void
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_task_core_init(void);

/*************************************************************************************
 * Function Name: pl_task_insert_tcb_to_rdylist
 * Description: Insert a tcb to list of ready task.
 *
 * Param:
 *   @tcb: task control block.
 * Return:
 *   void
 ************************************************************************************/
void pl_task_insert_tcb_to_rdylist(struct tcb *tcb);

/*************************************************************************************
 * Function Name: pl_task_insert_tcb_to_delaylist
 * Description: Insert a tcb to list of ready task.
 *
 * Param:
 *   @tcb: task control block.
 * Return:
 *   void
 ************************************************************************************/
void pl_task_insert_tcb_to_delaylist(struct tcb *tcb);

/*************************************************************************************
 * Function Name: pl_task_remove_tcb_from_delaylist
 * Description: Insert a tcb to list of ready task.
 *
 * Param:
 *   @tcb: task control block.
 * Return:
 *   void
 ************************************************************************************/
void pl_task_remove_tcb_from_delaylist(struct tcb *tcb);

/*************************************************************************************
 * Function Name: pl_task_remove_tcb_from_rdylist
 * Description:  remove a tcb form list of ready task.
 *
 * Param:
 *   @tcb: task control block.
 * Return:
 *   void
 ************************************************************************************/
void pl_task_remove_tcb_from_rdylist(struct tcb *tcb);

#endif /* __KERNEL_INTERNAL_TASK_H__ */
