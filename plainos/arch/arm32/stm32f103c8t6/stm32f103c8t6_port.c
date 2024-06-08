#include <port.h>
#include <config.h>
#include <kernel/initcall.h>
#include "early_setup/early_uart.h"
#include "stm32f10x.h"

static volatile int pl_critical_ref = 0;

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
void pl_port_enter_critical(void)
{
	/* 禁用中断 */
	__asm__ volatile("cpsid	i\n\t");
	/* 增加临界区引用计数，确保不会过早离开临界区 */
	++pl_critical_ref;
}

/*************************************************************************************
 * Function Name: void pl_port_exit_critical(void)
 * Description: exit critical area.
 *
 * Parameters:
 *   none
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_port_exit_critical(void)
{
	/* 减少临界区参考计数 */
	--pl_critical_ref;
	/* 当参考计数减少到0时，重新启用中断 */
	if (pl_critical_ref == 0)
		__asm__ volatile("cpsie	i\n\t");     /*< 开中断 */
}


/**
 * @brief 系统复位函数
 * 
 * 该函数用于触发系统复位，以重新启动系统。它通过写入SCB_AIRCR寄存器的特定值来实现复位。
 * 在写入复位值之前和之后，都调用了CPU的DSB指令，以确保写操作完成并生效。
 * 
 * 注意：该函数没有返回值，因为它在执行复位操作后不会返回。
 */
#define SCB_AIRCR      0xE000ED0C
#define SYS_RESET      ((0x5FA << 16) | (1 << 2))
void pl_port_system_reset(void)
{
    /* 强制数据同步屏障，确保以下复位寄存器的写入操作不会被重排序 */
    pl_port_cpu_dsb();
    
    /* 写入SCB_AIRCR寄存器的复位值，触发系统复位 */
    *((volatile u32_t *)SCB_AIRCR) = SYS_RESET;
    
    /* 再次强制数据同步屏障，确保复位寄存器的写入操作完全生效 */
    pl_port_cpu_dsb();
    
    /* 由于系统将复位，因此进入无限循环，不再执行后续代码 */
    while(1);
}

//RTS OS滴答定时器初始化，移植时需要用户自己实现
int pl_port_systick_init(void)
{
	/** 填写你的OS滴答定时器初始化代码 **/
	/* 设置PendSV为最低优先级 */
	*((volatile uint32_t *)(0xE000ED22)) = 0xff;
	__asm__ volatile("cpsid	i\n\t");     /*< 关中断 */
	SysTick_Config(PL_CFG_SYSTICK_TIME_SLICE_US * 72); // 1us 1900: 12.5us,  1800:25us,   3600:50us,   72000:1ms
	__asm__ volatile("cpsie	i\n\t");     /*< 开中断 */
	return 0;
}

void SysTick_Handler(void);
void SysTick_Handler(void)
{
	pl_callee_systick_expiration();
}

/**
 * 初始化任务栈
 * 
 * 该函数用于初始化任务的栈，为任务的执行准备必要的栈环境。它将栈指针向下移动到适当的位置，
 * 并在栈上设置初始的上下文值，包括任务函数的返回地址、参数和通用寄存器的值。
 * 
 * @param task 任务函数的指针。
 * @param task_stack 任务栈的起始地址。
 * @param stack_size 任务栈的大小。
 * @param context_top_sp 指向栈顶的指针，用于保存任务的初始栈顶地址。
 * @param param 传递给任务函数的参数。
 * @return 返回初始化后的栈指针。
 */
void *pl_port_task_stack_init(void *task, void *task_stack, size_t stack_size,
                              void **context_top_sp, void *param)
{
    /* 将任务栈指针转换为32位整型指针，以便于栈操作 */
    u32_t *stack = (u32_t *)task_stack;
    
    /* 设置栈顶指针并向下移动栈指针到正确的位置 */
    *context_top_sp = stack;
    stack       +=  stack_size / sizeof(u32_t);
    
    /* 从栈顶向下依次设置任务的初始上下文 */
    /* 设置XPSR寄存器的值 */
    *(--stack)  = (u32_t)(1<<24);
    /* 设置PC（程序计数器）寄存器的值，即任务函数的入口地址 */
    *(--stack)  = (u32_t)task;
    /* 设置LR（链接寄存器）寄存器的值，对于初试任务，其值为0 */
    *(--stack)  = (u32_t)0;
    /* 设置R12、R3、R2和R1寄存器的值，初始化为0 */
    *(--stack)  = (u32_t)0;
    *(--stack)  = (u32_t)0;
    *(--stack)  = (u32_t)0;
    *(--stack)  = (u32_t)0;
    /* 设置R0（参数寄存器）的值，即任务函数的参数 */
    *(--stack)  = (u32_t)param;
    
    /* 设置R4-R11寄存器的值，初始化为0 */
    *(--stack)  = (u32_t)0;
    *(--stack)  = (u32_t)0;
    *(--stack)  = (u32_t)0;
    *(--stack)  = (u32_t)0;
    *(--stack)  = (u32_t)0;
    *(--stack)  = (u32_t)0;
    *(--stack)  = (u32_t)0;
    *(--stack)  = (u32_t)0;
    
    /* 返回初始化后的栈指针 */
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