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

/*************************************************************************************
 * Description:
 *   type defitions.
 ************************************************************************************/

////////////////////////////// macros need to implement //////////////////////////////
/*************************************************************************************
 * Function Name: pl_port_cpu_barrier
 * 
 * Description:
 *   compiler barrier.
 *
 * Parameters:
 *   none.
 *
 * Return:
 *   void.
 ************************************************************************************/
#define pl_port_cpu_barrier   asm volatile("": : :"memory")

//////////////////////////////////// extern  ports ///////////////////////////////////
/*************************************************************************************
 * Function Name: pl_port_cpu_dmb
 *
 * Description:
 *   The function is used to store data meory barrier (data memory barrier).
 *
 * Parameters:
 *   none.
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_port_cpu_dmb(void);

/*************************************************************************************
 * Function Name: pl_port_cpu_dsb
 *
 * Description:
 *   The function is used to synchronise data (data synchronization barrier).
 *
 * Parameters:
 *   none.
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_port_cpu_dsb(void);

/*************************************************************************************
 * Function Name: pl_port_cpu_dsb
 *
 * Description:
 *   The function is used to synchronise instruction
 *   (instruction synchronization barrier).
 *
 * Parameters:
 *   none.
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_port_cpu_isb(void);

/*************************************************************************************
 * Function Name: pl_port_mask_interrupts
 *
 * Description:
 *   The function is used to mask all interrupt.
 *
 * Parameters:
 *   none.
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_port_mask_interrupts(void);

/*************************************************************************************
 * Function Name: pl_port_mask_interrupts
 *
 * Description:
 *   The function is used to unmask all interrupt.
 *
 * Parameters:
 *   none.
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_port_unmask_interrupts(void);

/*************************************************************************************
 * Function Name: void pl_port_enter_critical(void)
 * Description: enter critical area.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_port_enter_critical(void);

/*************************************************************************************
 * Function Name: void pl_poty_exit_critical(void)
 * Description: exit critical area.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_poty_exit_critical(void);

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
 *  Greater than or equal to 0 on success, less than 0 with failure.
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
void *pl_callee_get_next_context_sp(void);


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
 *  @task_entry: the task entry of initialization.
 *  @task_stack: task stack.
 *  @stack_size: stack size.
 *  @param: parameter passed.
 *
 * Return:
 *  pointer to the task_stack + regs.
 ************************************************************************************/
void *pl_port_task_stack_init(void *task_entry, void *task_stack,
                              size_t stack_size, void *param);


#endif /* __PLAINOS_PORT_H__ */
