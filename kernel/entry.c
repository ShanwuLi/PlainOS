/*
 * ************************************************
 * 
 *              STM32 blink gcc demo
 * 
 *  CPU: STM32F103C8
 *  PIN: PA1
 * 
 * ************************************************
*/
#include "stm32f10x.h"
#include <kernel/initcall.h>
#include "errno.h"

static void delay(int x)
{
    volatile int i,j;
    for (i = 0; i < x; i++)
    {
        for (j = 0; j < 10; j++)
            ;
    }
}
#include "drivers/uart/USART.h"

extern initcall_t __initcall0_start[];
extern initcall_t __initcall1_start[];
extern initcall_t __initcall2_start[];
extern initcall_t __initcall3_start[];
extern initcall_t __initcall4_start[];
extern initcall_t __initcall5_start[];
extern initcall_t __initcall6_start[];
extern initcall_t __initcall7_start[];
extern initcall_t __initcall8_start[];
extern initcall_t __initcall9_start[];
extern initcall_t __initcall_end[];

static initcall_t *initcall_levels[] = {
	__initcall0_start,
	__initcall1_start,
	__initcall2_start,
	__initcall3_start,
	__initcall4_start,
	__initcall5_start,
	__initcall6_start,
	__initcall7_start,
	__initcall8_start,
	__initcall9_start,
	__initcall_end,
};

static void initcalls_call(void)
{
	initcall_t **init_fns;
	initcall_t *init_fn = 0;

	for (init_fns = &initcall_levels[0]; init_fns < &initcall_levels[10]; init_fns++) {
		for (init_fn = *init_fns; init_fn < *(init_fns + 1); init_fn++) {
			(*init_fn)();
		}
	}
}

static int UART_INIT0(void)
{
	USART1_Init();

	USART1_PrintString("pure_initcall\r\n");

	return 0;
}
pure_initcall(UART_INIT0);

static int UART_INIT1(void)
{
	USART1_PrintString("pure_initcall_sync\r\n");

	return 0;
}
pure_initcall_sync(UART_INIT1);

static int UART_INIT2(void)
{

	USART1_PrintString("core_initcall\r\n");

	return 0;
}
core_initcall(UART_INIT2);

static int UART_INIT3(void)
{
	USART1_PrintString("core_initcall_sync\r\n");

	return 0;
}
core_initcall_sync(UART_INIT3);

static int UART_INIT4(void)
{

	USART1_PrintString("device_initcall\r\n");

	return 0;
}
device_initcall(UART_INIT4);

static int UART_INIT5(void)
{

	USART1_PrintString("late_initcall\r\n");

	return 0;
}
late_initcall(UART_INIT5);


static __const int adenckenv = 190;


int main()
{
	initcalls_call();
	//开启GPIOB端口时钟
	RCC->APB2ENR |= (1<<4);
	//清空控制PB0的端口位
	GPIOC->CRH &= ~( 0x0f<< (20));
	//配置PB0为通用推挽输出，速度为10M
	GPIOC->CRH |= (0x03<<20);
	//PB0输出低电平
	GPIOC->ODR &= (~(1 << 13));

	USART1_PrintString("PlainOS\r\n");


	USART1_PrintInteger(adenckenv);
	USART1_PrintString("\r\n");
	while (1)
	{
		USART1_PrintString("PlainOS\r\n");
		GPIOC->ODR = ~GPIOC->ODR;
		delay(100000);
	}
}
