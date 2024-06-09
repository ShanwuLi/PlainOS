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
/**********************************************************************************
 * 函数名称: pl_port_compile_barrier
 * 描述: 编译器屏障，确保编译器不会对屏障两侧的指令进行重排序。
 *
 * 参数:
 *   无。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
#define pl_port_compile_barrier   asm volatile("": : :"memory")

//////////////////////////////////// extern  ports ///////////////////////////////////
/**********************************************************************************
 * 函数名称: pl_port_cpu_dmb
 * 描述: 执行数据存储内存屏障（Data Memory Barrier），确保数据操作的完成顺序。
 *
 * 参数:
 *   无。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
void pl_port_cpu_dmb(void);

/**********************************************************************************
 * 函数名称: pl_port_cpu_dsb
 * 描述: 数据同步屏障，确保数据操作的完成并同步到所有观察者。
 *
 * 参数:
 *   无。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
void pl_port_cpu_dsb(void);

/**********************************************************************************
 * 函数名称: pl_port_cpu_isb
 * 描述: 指令同步屏障，确保指令执行顺序的一致性。
 *
 * 参数:
 *   无。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
void pl_port_cpu_isb(void);

/**********************************************************************************
 * 函数名称: pl_port_enter_critical
 * 描述: 进入临界区，保护共享资源不被并发访问。
 *
 * 参数:
 *   无。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
void pl_port_enter_critical(void);

/**********************************************************************************
 * 函数名称: pl_port_exit_critical
 * 描述: 退出临界区，解除对共享资源的保护。
 *
 * 参数:
 *   无。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
void pl_port_exit_critical(void);

/**********************************************************************************
 * 函数名称: pl_port_system_reset
 * 描述: 重启系统。
 *
 * 参数:
 *   无。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
void pl_port_system_reset(void);

/**********************************************************************************
 * 函数名称: pl_port_putc_init
 * 描述: 初始化字符输出功能。
 *
 * 参数:
 *   无。
 *
 * 返回值:
 *   成功时返回大于等于0的值，失败时返回小于0的值。
 ***********************************************************************************/
int pl_port_putc_init(void);

/**********************************************************************************
 * 函数名称: pl_port_putc
 * 描述: 输出单个字符。
 *
 * 参数:
 *   @c: 要输出的字符。
 *
 * 返回值:
 *   成功时返回大于等于0的值，失败时返回小于0的值。
 ***********************************************************************************/
int pl_port_putc(const char c);

/**********************************************************************************
 * 函数名称: pl_callee_entry
 * 描述: PlainOS 的入口点。在自定义架构的机器上，必须调用此函数以使PlainOS接管机器并开始运行。
 *
 * 参数:
 *   无。
 * 
 * 返回值:
 *   无。
 ***********************************************************************************/
void pl_callee_entry(void);

/**********************************************************************************
 * 函数名称: pl_port_systick_init
 * 描述: 初始化系统滴答定时器（Systick）。
 *
 * 参数:
 *   无。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
int pl_port_systick_init(void);

/**********************************************************************************
 * 函数名称: pl_callee_systick_expiration
 * 描述: 在系统滴答中断处理程序中调用此函数以切换任务。
 *
 * 参数:
 *   无。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
void pl_callee_systick_expiration(void);

/**********************************************************************************
 * 函数名称: pl_callee_save_curr_context_sp
 * 描述: 更新当前任务的上下文信息，并返回该任务的堆栈指针（Context SP）。
 *
 * 参数:
 *   无。
 *
 * 返回值:
 *   返回当前任务的上下文堆栈指针（void *）。
 ***********************************************************************************/
void pl_callee_save_curr_context_sp(void *context_sp);

/**********************************************************************************
 * 函数名称: pl_callee_get_next_context
 * 描述: 准备下一个任务的上下文，并返回该任务的堆栈指针（Context SP）。
 *
 * 参数:
 *   无。
 *
 * 返回值:
 *   返回下一个任务的上下文堆栈指针（void *）。
 ***********************************************************************************/
void *pl_callee_get_next_context_sp(void);


/////////////////////////////////// rodata ports /////////////////////////////////////
/**********************************************************************************
 * 函数名称: pl_port_rodata_read8
 * 描述: 读取只读数据区域中的8位数据。
 *
 * 参数:
 *   @addr: 要读取的数据地址。
 *
 * 返回值:
 *   8位数据。
 ***********************************************************************************/
u8_t pl_port_rodata_read8(void *addr);

/**********************************************************************************
 * 函数名称: pl_port_rodata_read16
 * 描述: 读取只读数据区域中的16位数据。
 *
 * 参数:
 *   @addr: 要读取的数据地址。
 *
 * 返回值:
 *   16位数据。
 ***********************************************************************************/
u16_t pl_port_rodata_read16(void *addr);

/**********************************************************************************
 * 函数名称: pl_port_rodata_read32
 * 描述: 读取只读数据区域中的32位数据。
 *
 * 参数:
 *   @addr: 要读取的数据地址。
 *
 * 返回值:
 *   32位数据。
 ***********************************************************************************/
u32_t pl_port_rodata_read32(void *addr);

/**********************************************************************************
 * 函数名称: pl_port_rodata_read_ptr
 * 描述: 用于读取只读数据区中的uintptr_t类型数据。
 *
 * 参数:
 *   @addr: 要读取数据的地址。
 *
 * 返回值:
 *   uintptr_t类型的数据。
 ***********************************************************************************/
uintptr_t pl_port_rodata_read(void *addr);

/**********************************************************************************
 * 函数名称: pl_port_switch_context
 * 描述: 切换任务上下文。
 *
 * 参数:
 *   无。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
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
 *  @context_top_sp: top stack pointer used to check stack overflow.
 *  @param: parameter passed.
 *
 * Return:
 *  pointer to the task_stack + regs.
 ************************************************************************************/
void *pl_port_task_stack_init(void *task, void *task_stack, size_t stack_size,
                              void **context_top_sp, void *param);


#endif /* __PLAINOS_PORT_H__ */
