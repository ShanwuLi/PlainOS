#include  "../stm32f10x.h"
#include <errno.h>
#include <types.h>
#include "early_uart.h"

/**
 * 初始化USART1，设置波特率和相关配置
 * @param USART1_BaudRate 波特率，指定USART1的通信速度
 */
void USART1_Init(uint_t USART1_BaudRate)
{
   // 以下两行声明了整型和浮点型变量，用于计算USART的时钟配置
   unsigned int integer;
   float decimal;

   // 启用GPIOA和USART1的时钟，并对USART1进行复位
   RCC->APB2ENR|=(1<<14)|(1<<2);
   RCC->APB2RSTR|=(1<<14);
   RCC->APB2RSTR&=(~(1<<14));

   // 配置GPIOA的CRH寄存器，为USART1的TX和RX引脚设置合适的模式
   GPIOA->CRH&=0XFFFFF00F;
   GPIOA->CRH|=0X000008B0;

   // 启用USART1的多种功能，包括接收、发送和中断
   USART1->CR1|=(1<<13);
   USART1->CR1|=(1<<2);
   USART1->CR1|=(1<<5);
   USART1->CR1|=(1<<3);

   // 计算并设置USART的波特率发生器，以达到指定的波特率
   integer = 128 * 1000 * 1000 / (USART1_BaudRate * 16);
   decimal = (float)(128 * 1000 * 1000 / (USART1_BaudRate * 16)) - integer;
   USART1->BRR = (integer << 4) | ((unsigned int)decimal * 16);

   // 启用USART1的中断，以便在数据发送和接收时得到通知
   // NVIC_SetPriorityGrouping(1);
   // NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(1,2,1));
   NVIC_EnableIRQ(USART1_IRQn);
}

int USART1_PrintChar(char c)
{
	uint_t times = 0;
	USART1->DR = c;

	for (times = 0; times < 120000; times++)
		if(USART1->SR & (1 << 6))
			return 0;

	return -ETIMEOUT;
}