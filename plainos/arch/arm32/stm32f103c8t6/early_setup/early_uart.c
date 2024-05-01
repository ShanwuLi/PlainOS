#include  "../stm32f10x.h"
#include <errno.h>
#include <types.h>
#include "early_uart.h"

void USART1_Init(uint_t USART1_BaudRate)
{
   unsigned int integer;                   //����һ���������洢��������
   float   decimal;                        //����һ���������洢С������
   RCC->APB2ENR|=(1<<14)|(1<<2);           //����USART1ʱ�Ӻ�GPIOAʱ��
	 RCC->APB2RSTR|=(1<<14);                 //��λ����USART1
	 RCC->APB2RSTR&=(~(1<<14));              //ֹͣ��λ
	 
	 GPIOA->CRH&=0XFFFFF00F;                 //���֮ǰ����PA10��PA9
	 GPIOA->CRH|=0X000008B0;                 //����PA10Ϊ�������룬����PA9Ϊ�����������
	
	
   USART1->CR1|=(1<<13);                   //ʹ��USARTģ��
   USART1->CR1|=(1<<2);                    //ʹ�ܽ���
	 USART1->CR1|=(1<<5);                    //���ж�
   USART1->CR1|=(1<<3);                    //ʹ�ܷ���	
	 
	
   integer=72*1000*1000/(USART1_BaudRate*16);         //ȡ����������
   decimal=(float)(72*1000*1000/(USART1_BaudRate*16))-integer;   //ȡ��С������
   USART1->BRR=(integer<<4)|((unsigned int)decimal*16);  //��ת�����ֵ����BRR�Ĵ���
   
   NVIC_SetPriorityGrouping(1);            //�������ȼ�����1
   NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(1,2,1));     //������ռ���ȼ�Ϊ1�������ȼ�Ϊ1
   NVIC_EnableIRQ(USART1_IRQn);            //ʹ��USART�ж�
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