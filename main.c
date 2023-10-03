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

static void delay(int x)
{
    volatile int i,j;
    for (i = 0; i < x; i++)
    {
        for (j = 0; j < 10; j++)
            ;
    }
}
#include "drivers/USART.h"

int main()
{
	//开启GPIOB端口时钟
	RCC->APB2ENR |= (1<<4);
	//清空控制PB0的端口位
	GPIOC->CRH &= ~( 0x0f<< (20));
	//配置PB0为通用推挽输出，速度为10M
	GPIOC->CRH |= (0x03<<20);
	//PB0输出低电平
	GPIOC->ODR &= (~(1 << 13));

	USART1_Init();
	USART1_PrintString("RTS Start\r\n");

	while (1)
	{
		USART1_PrintString("RTS Start\r\n");
		GPIOC->ODR = ~GPIOC->ODR;
		delay(100000);
	}
}




