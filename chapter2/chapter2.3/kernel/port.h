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

#ifndef __PLAINOS_PORT_H__
#define __PLAINOS_PORT_H__

#include <types.h>
#include "task.h"

//////////////////////////////////// extern  ports ///////////////////////////////////
/*************************************************************************************
 * Function Name: pl_port_putc_init
 *
 * Description:
 *   The function is used to initialize put char function.
 *
 * Parameters:
 *   none.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 with failure.
 ************************************************************************************/
int pl_port_putc_init(void);

/*************************************************************************************
 * Function Name: pl_port_putc
 *
 * Description:
 *   The function is used to put char.
 *
 * Parameters:
 *   @c: the char we want to put.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 with failure.
 ************************************************************************************/
int pl_port_putc(const char c);

/*************************************************************************************
 * Function Name: pl_callee_entry
 *
 * Description:
 *   PlainOS entry, We must call pl_entry in my own architecture machine. Then PlainOS
 *   will take over the machine.
 *
 * Parameters:
 *   none.
 * 
 * Return:
 *   none.
 ************************************************************************************/
void pl_callee_entry(void);

/*************************************************************************************
 * Function Name: pl_port_systick_init
 *
 * Description:
 *   The function is used to initialize systick on systick system.
 *
 * Parameters:
 *  none.
 *
 * Return:
 *  none.
 ************************************************************************************/
void pl_port_systick_init(void);

/*************************************************************************************
 * Function Name: pl_callee_save_curr_context_sp
 * Description: update context and return context_sp of the current task.
 *
 * Parameters:
 *  none
 *
 * Return:
 *    void *context sp.
 ************************************************************************************/
void pl_callee_save_curr_context_sp(void *context_sp);

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
void *pl_callee_get_next_context_sp(void);

/*************************************************************************************
 * Function Name: pl_port_switch_context
 *
 * Description:
 *   The function is used to switch context.
 *
 * Parameters:
 *   none.
 *
 * Return:
 *  none.
 ************************************************************************************/
void pl_port_switch_context(void);

/*************************************************************************************
 * Function Name: pl_port_task_stack_init
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
//创建任务控制块数组记录任务堆栈指针变化情况
void  pl_port_task_stack_init(void (*task)(void *arg), uint32_t *task_stack, 
				uint32_t stack_size, TCB *task_control_block, void *arg);

#endif /* __PLAINOS_PORT_H__ */