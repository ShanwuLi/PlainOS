#include <types.h>
#include <config.h>
#include <port.h>
#include <task.h>
#include "early_setup/early_uart.h"
#include "stm32f10x.h"

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
int pl_port_putc_init(void)
{
	NVIC_EnableIRQ(PendSV_IRQn);
	USART1_Init(115200);
	USART1_PrintChar(' ');
	return 0;
}

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
int pl_port_putc(char c)
{
	int ret = USART1_PrintChar(c);
	return ret;
}

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
//RTS OS滴答定时器初始化，移植时需要用户自己实现
#define PendSV_Priority_Level_Register 0xE000ED22
#define PendSV_Priority_Value 0xFF
void pl_port_systick_init(void)
{
	(*(uint32_t *)PendSV_Priority_Level_Register) = PendSV_Priority_Value;

	/** 填写你的OS滴答定时器初始化代码 **/
	__asm__ volatile("cpsid	i\n\t");     /*< 关中断 */
	SysTick_Config(PL_CFG_SYSTICK_TIME_SLICE_US * 72); // 1us 1900: 12.5us,  1800:25us,   3600:50us,   72000:1ms
	NVIC_EnableIRQ(SysTick_IRQn);
	__asm__ volatile("cpsie	i\n\t");     /*< 开中断 */
}

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
				uint32_t stack_size, TCB *task_control_block, void *arg)
{
	uint32_t *stack = (uint32_t *)task_stack;
	stack = stack + stack_size; //为了方便理解这里将任务栈存储空间设置为Nx32结构和STM32寄存器位数保持一样，确保SP入栈一次减少4,stack变量减少1

	*(--stack)  = (u32_t)(1<<24);  /* XPSR */ //这里入栈顺序和《Cortex-M3权威指南9.1中断/异常的响应序列》一样，注意软件入栈顺序和硬件底层流水线入栈有些不一样
	*(--stack)  = (u32_t)task;     /* PC ,将任务地址压栈*/
	*(--stack)  = (u32_t)0;        /* LR */
	*(--stack)  = (u32_t)0;        /* R12 */
	*(--stack)  = (u32_t)0;        /* R3 */
	*(--stack)  = (u32_t)0;        /* R2 */
	*(--stack)  = (u32_t)0;        /* R1 */
	*(--stack)  = (u32_t)arg;      /* R0 */



	*(--stack)  = (u32_t)0;        /* R4 */	//这里入栈顺序依照用户喜欢的顺序，这里就按照寄存器名称顺序入栈
	*(--stack)  = (u32_t)0;        /* R5 */
	*(--stack)  = (u32_t)0;        /* R6 */
	*(--stack)  = (u32_t)0;        /* R7 */
	*(--stack)  = (u32_t)0;        /* R8 */
	*(--stack)  = (u32_t)0;        /* R9 */
	*(--stack)  = (u32_t)0;        /* R10 */
	*(--stack)  = (u32_t)0;        /* R11 */
	task_control_block->TaskStackPointer = (uint32_t)stack; //使用全局变量TaskControlBlock->TaskStackPointer成员记录个任务栈SP位置
}
