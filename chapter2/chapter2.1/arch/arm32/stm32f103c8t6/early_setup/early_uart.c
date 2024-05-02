#include  "../stm32f10x.h"
#include <errno.h>
#include <types.h>
#include "early_uart.h"

void USART1_Init(uint_t USART1_BaudRate)
{
	unsigned int integer;                  
	float   decimal;                       
	RCC->APB2ENR|=(1<<14)|(1<<2);   //将IOPA时钟和USART1外设时钟使能       
	RCC->APB2RSTR|=(1<<14);         //将USART1外设复位       
	RCC->APB2RSTR&=(~(1<<14));      //将USART1外设复位状态解除            

	GPIOA->CRH&=0XFFFFF00F;         //将GPIOA A9和A10引脚输出模式清零       
	GPIOA->CRH|=0X000008B0;         //将GPIOA A9设置为上拉/下拉输入模式，A10引脚设置为复用推挽输出50MHz时钟模式    


	USART1->CR1|=(1<<13);           //使能USART外设        
	USART1->CR1|=(1<<2);            //USART接收被使能     
	USART1->CR1|=(1<<5);            //缓冲区非空中断使能      
	USART1->CR1|=(1<<3);            //USART发送被使能                    

	integer=72*1000*1000/(USART1_BaudRate*16);         
	decimal=(float)(72*1000*1000/(USART1_BaudRate*16))-integer;   
	USART1->BRR=(integer<<4)|((unsigned int)decimal*16);
	USART1_PrintChar('S');
}

int USART1_PrintChar(char c)
{
	uint_t times = 0;
	USART1->DR = c;

	for (times = 0; times < 120000; times++)
		if(USART1->SR & (1 << 6)) //USART_SR第六位表示Transmission Complete
			return 0;

	return -1;
}

