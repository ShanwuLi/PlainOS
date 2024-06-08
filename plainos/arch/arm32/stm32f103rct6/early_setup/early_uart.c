#include  "../stm32f10x.h"
#include <errno.h>
#include <types.h>
#include "early_uart.h"

/*************************************************************************************
 * 初始化USART1
 * @param USART1_BaudRate 波特率，用于计算USART的波特率寄存器值
 *************************************************************************************/
void USART1_Init(uint_t USART1_BaudRate)
{
   unsigned int integer;                    // 用于存储整数部分
   float decimal;                           // 用于存储小数部分
   
   // 启用GPIOA时钟和USART1时钟，然后对USART1进行复位
   RCC->APB2ENR|=(1<<14)|(1<<2);           //将IOPA时钟和USART1外设时钟使能
   RCC->APB2RSTR|=(1<<14);                 //将USART1外设复位 
   RCC->APB2RSTR&=(~(1<<14));              //将USART1外设复位状态解除 
  
   // 配置GPIOA的CRH寄存器，设置PA9和PA10为USART1的TX和RX
   GPIOA->CRH&=0XFFFFF00F;                 //将GPIOA A9和A10引脚输出模式清零       
   GPIOA->CRH|=0X000008B0;                 //将GPIOA A9设置为上拉/下拉输入模式，A10引脚设置为复用推挽输出50MHz时钟模式  
  
   // 配置USART1的控制寄存器1，启用USART，接收和发送功能，以及中断
   USART1->CR1|=(1<<13);                   //使能USART外设        
   USART1->CR1|=(1<<2);                    //USART接收被使能     
   USART1->CR1|=(1<<5);                    //缓冲区非空中断使能      
   USART1->CR1|=(1<<3);                    //USART发送被使能           
  
   // 计算并设置USART的波特率寄存器值
   integer=128*1000*1000/(USART1_BaudRate*16);         
   decimal=(float)(128*1000*1000/(USART1_BaudRate*16))-integer;   
   USART1->BRR=(integer<<4)|((unsigned int)decimal*16);  
   
   // 启用USART1的中断
   //NVIC_SetPriorityGrouping(1);            
   //NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(1,2,1));     
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