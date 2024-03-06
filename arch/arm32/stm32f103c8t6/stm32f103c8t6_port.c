#include <port.h>
#include <kernel/initcall.h>
#include "early_setup/early_uart.h"
#include "stm32f10x.h"


int pl_port_putc_init(void)
{
	NVIC_EnableIRQ(PendSV_IRQn);
	USART1_Init(115200);
	USART1_PrintChar(' ');
	return 0;
}

int pl_port_putc(char c)
{
	int ret = USART1_PrintChar(c);
	return ret;
}

void pl_port_mask_interrupts(void)
{
	__asm__ volatile("cpsid	i\n\t");     /*< 关中断 */
}

void pl_port_unmask_interrupts(void)
{
	__asm__ volatile("cpsie	i\n\t");     /*< 开中断 */
}

//RTS OS滴答定时器初始化，移植时需要用户自己实现
int pl_port_systick_init(void)
{
	/** 填写你的OS滴答定时器初始化代码 **/
	__asm__ volatile("cpsid	i\n\t");     /*< 关中断 */
	SysTick_Config(900); //900: 12.5us,  1800:25us,   3600:50us,   72000:1ms
	NVIC_EnableIRQ(PendSV_IRQn);
	__asm__ volatile("cpsie	i\n\t");     /*< 开中断 */
	return 0;
}

void SysTick_Handler(void);
void SysTick_Handler(void)
{
	pl_callee_systick_expiration();
}

void *pl_port_task_stack_init(void *task, void *task_stack,
                               size_t stack_size, void *param)
{
	u32_t *stack = (u32_t *)task_stack;

	stack       +=  stack_size / sizeof(u32_t);
	*(--stack)  = (u32_t)(1<<24);  /* XPSR */
	*(--stack)  = (u32_t)task;     /* PC */
	*(--stack)  = (u32_t)0;        /* LR */
	*(--stack)  = (u32_t)0;        /* R12 */
	*(--stack)  = (u32_t)0;        /* R3 */
	*(--stack)  = (u32_t)0;        /* R2 */
	*(--stack)  = (u32_t)0;        /* R1 */
	*(--stack)  = (u32_t)param;    /* R0 */

	*(--stack)  = (u32_t)0;        /* R4 */
	*(--stack)  = (u32_t)0;        /* R5 */
	*(--stack)  = (u32_t)0;        /* R6 */
	*(--stack)  = (u32_t)0;        /* R7 */
	*(--stack)  = (u32_t)0;        /* R8 */
	*(--stack)  = (u32_t)0;        /* R9 */
	*(--stack)  = (u32_t)0;        /* R10 */
	*(--stack)  = (u32_t)0;        /* R11 */
	return stack;
}

u8_t pl_port_rodata_read8(void *addr)
{
	return *(u8_t *)(addr);
}

u16_t pl_port_rodata_read16(void *addr)
{
	return *(u16_t *)(addr);
}

u32_t pl_port_rodata_read32(void *addr)
{
	return *(u32_t *)(addr);
}

uintptr_t pl_port_rodata_read(void *addr)
{
	return *(uintptr_t *)(addr);
}