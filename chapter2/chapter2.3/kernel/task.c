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
#include <early_setup/early_uart.h>
#include "task.h"
#include "port.h"


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
void pl_task_context_switch(void){
	pl_port_switch_context();
}

/*************************************************************************************
 * Function Name: pl_callee_save_curr_context_sp
 * Description: update context and return context_sp of the current task.
 *
 * Parameters:
 *  none
 *
 * Return:
 *    void *context sp;
 ************************************************************************************/
static volatile int8_t TaskID = -1;
extern uint8_t Task_Numbers;
extern TCB *TaskControlBlockArrayPointer;
void pl_callee_save_curr_context_sp(void *context_sp)
{
	USART1_PrintChar('T');
	(TaskControlBlockArrayPointer+TaskID)->TaskStackPointer = (uint32_t)context_sp;
}

/*************************************************************************************
 * Function Name: pl_callee_get_next_context
 * Description: update context and return context_sp of the current task.
 *
 * Parameters:
 *  none
 *
 * Return:
 *    void *context sp.
 ************************************************************************************/

void *pl_callee_get_next_context_sp(void)
{
	USART1_PrintChar('B');
	TaskID = (TaskID + 1) % Task_Numbers;
	return (uint32_t *)(TaskControlBlockArrayPointer+TaskID)->TaskStackPointer;
}

/*************************************************************************************
 * Function Name: pl_task_sys_create
 *
 * Description:
 *   The function is used to initialize stack of the task.
 *               _______________________________________________________
 * stack bottom |                   |                                   | stack top
 *              |___________________|___________________________________|
 *              A                   A                                   A
 *              |                   |                                   |
 *          task_stack       task_stack + regs                task_stack + stack_size
 
 * Parameters:
 *  @task: the task entry of initialization.
 *  @task_stack: task stack.
 *  @stack_size: stack size.
 *  @task_control_block: top stack pointer used to check stack overflow.
 *  @arg: parameter passed.
 *
 * Return:
 *  pointer to the task_stack + regs.
 ************************************************************************************/
void pl_task_sys_create(void (*task)(void *arg), uint32_t *task_stack, 
				uint32_t stack_size, TCB *task_control_block, void *arg){
	pl_port_task_stack_init(task, task_stack, stack_size, task_control_block, arg);

}

