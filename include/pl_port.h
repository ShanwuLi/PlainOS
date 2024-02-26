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
#include <stddef.h>
#include <kernel/kernel.h>
#include <kernel/task.h>

/////////////////////////////////// system callee ////////////////////////////////////
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
 *   The function is used to initialize systick.
 *   on systick system.
 * 
 * Parameters:
 *  none
 *
 * Return:
 *  none
 ************************************************************************************/
int pl_port_systick_init(void);

/*************************************************************************************
 * Function Name: pl_callee_systick_expiration
 *
 * Description:
 *   The function is used to switch task, we must call it in systick handler
 *   on systick system.
 * 
 * Parameters:
 *  none
 *
 * Return:
 *  none
 ************************************************************************************/
void pl_callee_systick_expiration(void);

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
void pl_callee_save_curr_context_sp(void *context_sp);

/*************************************************************************************
 * Function Name: pl_callee_get_next_context
 * Description: update context and return context_sp of the current task.
 *
 * Parameters:
 *  none
 *
 * Return:
 *    void *context sp;
 ************************************************************************************/
void *pl_callee_get_next_context(void);


/////////////////////////////////// rodata ports /////////////////////////////////////
/*************************************************************************************
 * Function Name: pl_port_rodata_read8
 *
 * Description:
 *   The function is used to read only data of 8bits.
 *
 * Parameters:
 *   @addr: the address of data we want to read.
 * 
 * Return:
 *   data of 8bits.
 ************************************************************************************/
u8_t pl_port_rodata_read8(void *addr);

/*************************************************************************************
 * Function Name: pl_port_rodata_read16
 *
 * Description:
 *   The function is used to read only data of 16bits.
 *
 * Param:
 *   @addr: the address of data we want to read.
 * 
 * Return:
 *   data of 16bits.
 ************************************************************************************/
u16_t pl_port_rodata_read16(void *addr);

/*************************************************************************************
 * Function Name: pl_port_rodata_read32
 *
 * Description:
 *   The function is used to read only data of 32bits.
 *
 * Parameters:
 *   @addr: the address of data we want to read.
 * 
 * Return:
 *   data of 32bits.
 ************************************************************************************/
u32_t pl_port_rodata_read32(void *addr);

/*************************************************************************************
 * Function Name: pl_port_rodata_read32
 *
 * Description:
 *   The function is used to read only data of uintptr_t.
 *
 * Parameters:
 *   @addr: the address of data we want to read.
 * 
 * Return:
 *   data of uintptr_t.
 ************************************************************************************/
uintptr_t pl_port_rodata_read(void *addr);


///////////////////////////////////// early ports ////////////////////////////////////
/*************************************************************************************
 * Function Name: pl_early_port_putc
 *
 * Description:
 *   The function is used to put char in early stage before the first task setup.
 *
 * Parameters:
 *   @c: the char we want to put.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 with failure.
 ************************************************************************************/
int pl_early_port_putc(const char c);



////////////////////////////////////  task ports /////////////////////////////////////
typedef uintptr_t irqstate_t;

/*************************************************************************************
 * Function Name: pl_port_irq_save
 * for example in atmega128:
 *  irqstate_t pl_port_irq_save(void)
 *  {
 *     return SREG;
 *  }
 *
 * Description:
 *   The function is used to save interrupt mask status.
 *
 * Parameters:
 *   none.
 *
 * Return:
 *  The interrupt mask status we want to save.
 ************************************************************************************/
irqstate_t pl_port_irq_save(void);

/*************************************************************************************
 * Function Name: pl_port_irq_restore
 *
 * Description:
 *   The function is used to restore interrupt mask status.
 *   void pl_port_irq_restore(irqstate_t irqstate)
 *   {
 *      SREG = (u8_t)irqstate;
 *   }
 *
 * Parameters:
 *   none.
 *
 * Return:
 *  The interrupt status we want to restore.
 ************************************************************************************/
void pl_port_irq_restore(irqstate_t irqstate);

/*************************************************************************************
 * Function Name: pl_port_first_context_switch
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
void pl_port_first_context_switch(void);

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
 *            _______________________________________________________
 * stack top |                   |                                   | stack bottom
 *           |___________________|___________________________________|
 *           A                   A                                   A
 *           |                   |                                   |
 *       task_stack       task_stack + regs                task_stack + stack_size
 
 * Parameters:
 *  @task: the task of initialization, prototype is "int task(int argc, char *argv[])"
 *  @task_stack: task stack.
 *  @stack_size: stack size.
 *  @argc: the count of argv.
 *  @argv: argv[].
 *  @task_end: routine of task ending.
 *
 * Return:
 *  pointer to the task_stack + regs.
 ************************************************************************************/
void *pl_port_task_stack_init(task_t task, void *task_stack, size_t stack_size,
                              int argc, char *argv[], task_end_t task_end);


#endif /* __PLAINOS_PORT_H__ */
