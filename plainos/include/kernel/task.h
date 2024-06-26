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

#include <config.h>
#include <types.h>
#include "kernel.h"

typedef void *pl_tid_t;

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************
 * Function Name: pl_task_schedule_lock
 * Description: disable scheduler.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   none
 ************************************************************************************/
void pl_task_schedule_lock(void);

/*************************************************************************************
 * Function Name: pl_task_schedule_unlock
 * Description: enable scheduler.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   none
 ************************************************************************************/
void pl_task_schedule_unlock(void);

/*************************************************************************************
 * Function Name: pl_task_create_with_stack
 * Description: create a task with stack that must be provided.
 *
 * Parameters:
 *   @name: name of the task (optional).
 *   @task: task, prototype is "int task(int argc, char *argv[])"
 *   @prio: priority of the task, if is 0, it will be its parent's priority (optional).
 *   @stack: stack of the task (must provide).
 *   @stack_size: size of the stack (must specify).
 *   @argc: the count of argv (optional).
 *   @argv: argv[] (optional).
 *
 * Return:
 *   task id.
 ************************************************************************************/
pl_tid_t pl_task_create_with_stack(const char *name, main_t task, u16_t prio,
                                   void *stack, size_t stack_size,
                                   int argc, char *argv[]);

/*************************************************************************************
 * Function Name: pl_task_create
 * Description: create a task.
 *
 * Parameters:
 *   @name: name of the task (optional).
 *   @task: task, prototype is "int task(int argc, char *argv[])"
 *   @prio: priority of the task, if is 0, it will be its parent's priority (optional).
 *   @stack_size: size of the stack (must specify).
 *   @argc: the count of argv (optional).
 *   @argv: argv[] (optional).
 *
 * Return:
 *   task id.
 ************************************************************************************/
pl_tid_t pl_task_create(const char *name, main_t task, u16_t prio,
                        size_t stack_size, int argc, char *argv[]);

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
void pl_task_delay_ticks(u64_t ticks);

/*************************************************************************************
 * Function Name: pl_task_join
 *
 * Description:
 *   wait for task exit.
 * 
 * Parameters:
 *  @tid: task id;
 *  @ret: return value of waitting task.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_task_join(pl_tid_t tid, int *ret);

/*************************************************************************************
 * Function Name: pl_task_pend
 *
 * Description:
 *   pend task.
 *
 * Parameters:
 *  @tid: task id, if tid is NULL, it will pend himself.
 *
 * Return:
 *  void.
 ************************************************************************************/
void pl_task_pend(pl_tid_t tid);

/*************************************************************************************
 * Function Name: pl_task_resume
 *
 * Description:
 *   resume task.
 * 
 * Parameters:
 *  @tid: task id;
 *
 * Return:
 *  void.
 ************************************************************************************/
void pl_task_resume(pl_tid_t tid);

/*************************************************************************************
 * Function Name: pl_task_restart
 *
 * Description:
 *   restart task.
 * NOTE: task id must not be itself
 * 
 * Parameters:
 *  @tid: task id;
 *
 * Return:
 *  void.
 ************************************************************************************/
void pl_task_restart(pl_tid_t tid);

/*************************************************************************************
 * Function Name: pl_task_kill
 *
 * Description:
 *   kill a task.
 * 
 * Parameters:
 *  @tid: task id;
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_task_kill(pl_tid_t tid);

/*************************************************************************************
 * Function Name: pl_task_get_syscount
 *
 * Description:
 *   The function is used to get syscount.
 *   on systick system.
 * 
 * Parameters:
 *  @c: count wanted to get.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_task_get_syscount(u64_t *c);

/*************************************************************************************
 * Function Name: pl_task_get_cpu_rate_count
 *
 * Description:
 *   The function is used to get cpu utilization rate.
 *   on systick system.
 * 
 * Parameters:
 *  @cup_rate_base: base count of cpu utilization rate.
 *  @rate_useful: useful count of cpu utilization rate.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_task_get_cpu_rate_count(u32_t *rate_base, u32_t *rate_useful);

/*************************************************************************************
 * Function Name: pl_task_get_cpu_rate
 *
 * Description:
 *   The function is used to get cpu utilization rate.
 *   on systick system.
 * 
 * Parameters:
 *  @int_part: integer part of cpu utilization rate.
 *  @deci_part: decimal part of cpu utilization rate.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_task_get_cpu_rate(u32_t *int_part, u32_t *deci_part);

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_TASK_H__ */
