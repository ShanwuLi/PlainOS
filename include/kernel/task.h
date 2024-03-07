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

typedef void *tid_t;

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
tid_t pl_task_create_with_stack(const char *name, main_t task, u16_t prio,
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
tid_t pl_task_create(const char *name, main_t task, u16_t prio,
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
void pl_task_delay_ticks(u32_t ticks);

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
int pl_task_join(tid_t tid, int *ret);

/*************************************************************************************
 * Function Name: pl_task_get_cpu_rate
 *
 * Description:
 *   The function is used to get cpu rate.
 *   on systick system.
 * 
 * Parameters:
 *  @cup_rate_base: base count of cpu rate.
 *  @cpu_rate_idle: idle count of cpu rate.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_task_get_cpu_rate(u32_t *cup_rate_base, u32_t *cpu_rate_idle);

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_TASK_H__ */
