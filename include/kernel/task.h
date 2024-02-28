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

#ifndef __KERNEL_TASK_H__
#define __KERNEL_TASK_H__

#include <pl_cfg.h>
#include <kernel/kernel.h>
#include <kernel/list.h>
#include <types.h>

/*************************************************************************************
 * Type Name: task_schedule_t
 * Description: task schedule of plain os port.
 *
 ************************************************************************************/
enum task_state {
	PL_TASK_STATE_READY = 0,
	PL_TASK_STATE_DELAY,
	PL_TASK_STATE_WAIT,
	PL_TASK_STATE_PEND,
	PL_TASK_STATE_EXIT,
	PL_TASK_STATE_FATAL,
};

struct tcb;
typedef void (*task_entry_t)(struct tcb *tcb);
typedef int  (*task_t)(int argc, char *argv[]);

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
	task_t task;
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

typedef struct tcb* tid_t;

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************
 * Function Name: pl_context_switch
 * Description: switch task.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   none
 ************************************************************************************/
void pl_context_switch(void);

/*************************************************************************************
 * Function Name: pl_schedule_lock
 * Description: disable scheduler.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   none
 ************************************************************************************/
void pl_schedule_lock(void);

/*************************************************************************************
 * Function Name: pl_schedule_unlock
 * Description: enable scheduler.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   none
 ************************************************************************************/
void pl_schedule_unlock(void);

/*************************************************************************************
 * Function Name: pl_task_create_with_stack
 * Description: create a task with stack that must be provided.
 *
 * Parameters:
 *   @name: name of the task (optional).
 *   @task: task, prototype is "int task(int argc, char *argv[])"
 *   @prio: priority of the task, if is 0, it will be its parent's priority (optional).
 *   @tcb: tcb of the task (must provide).
 *   @stack: stack of the task (must provide).
 *   @stack_size: size of the stack (must specify).
 *   @argc: the count of argv (optional).
 *   @argv: argv[] (optional).
 *
 * Return:
 *   task handle.
 ************************************************************************************/
tid_t pl_task_create_with_stack(const char *name, task_t task, u16_t prio,
                                struct tcb *tcb, void *stack, size_t stack_size,
                                int argc, char *argv[]);

/*************************************************************************************
 * Function Name: pl_task_delay_ticks
 *
 * Description:
 *   Delay ticks function.
 * 
 * Parameters:
 *  @ticks: delay ticks;
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
void pl_task_delay_ticks(u32_t ticks);

/*************************************************************************************
 * Function Name: pl_task_join
 *
 * Description:
 *   wait for task exit.
 * 
 * Parameters:
 *  @tcb: task control block;
 *  @ret: return value of waitting task.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_task_join(struct tcb *tcb, int *ret);

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_TASK_H__ */
