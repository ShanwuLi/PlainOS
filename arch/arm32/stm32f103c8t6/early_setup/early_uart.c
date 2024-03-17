#include  "../stm32f10x.h"
#include <errno.h>
#include <types.h>
#include "early_uart.h"

void USART1_Init(uint_t USART1_BaudRate)
{
   unsigned int integer;                  
   float   decimal;                       
   RCC->APB2ENR|=(1<<14)|(1<<2);          
	RCC->APB2RSTR|=(1<<14);                
	RCC->APB2RSTR&=(~(1<<14));            
	 
	GPIOA->CRH&=0XFFFFF00F;                
	GPIOA->CRH|=0X000008B0;                
	
	
   USART1->CR1|=(1<<13);                  
   USART1->CR1|=(1<<2);                  
	USART1->CR1|=(1<<5);                  
   USART1->CR1|=(1<<3);                    
	 
	
   integer=72*1000*1000/(USART1_BaudRate*16);         
   decimal=(float)(72*1000*1000/(USART1_BaudRate*16))-integer;   
   USART1->BRR=(integer<<4)|((unsigned int)decimal*16);  
   
   NVIC_SetPriorityGrouping(1);            
   NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(1,2,1));     
   NVIC_EnableIRQ(USART1_IRQn);            
}

int USART1_PrintChar(char c)
{
	uint_t times = 0;
	USART1->DR = c;

	for (times = 0; times < 120000; times++)
		if(USART1->SR & (1 << 6))
			return 0;

	return -1;
}