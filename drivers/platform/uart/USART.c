/**************************************************************************
* �ļ�����USART.c                                                         *
* ��д�ˣ����ŵķ�                                                        *
* ����ʱ�䣺2018.10.6                                                     *
* ˵�������ļ����ڿ�Դ������Ȩ���κ��˿���					                    	*
* ������������ϵ���䣺1477153217@qq.com                                   *
*                                                                         *
* USART���ò������£�                                                     *
* 1.ʹ��USARTʱ��                                                         *
* 2.����USART�Ĵ���(CR1:[13],[2],[3];BRR:[15:4],[4:0]                     *
* 3.����USART���ȼ�����                                                   *
* 4.д���ֽڵ����ݼĴ���(DR)�У�һ��ֻ�ܷ���һ���ֽ�                      *
*                                                                         *
* ��ע�⣺ÿ�η���һ���ֽڱ���Ҫ��ʱ�����ܲ���ϵķ��ͣ���������������  *
***************************************************************************/
//Example:  USART1   bit rate: 9600,CRC: none ,stop bit: n bit

#include "drivers/uart/USART.h"

#if(USART1_EN==1)
		volatile char Receive1BUFF[40]={0};
		unsigned char USART1_ReCoun=0;
#endif
#if(USART2_EN==1)	 
		char Receive2BUFF[40]={0};
		unsigned char USART2_ReCoun=0;
#endif
#if(USART3_EN==1)		
		char Receive3BUFF[40]={0};
		unsigned char USART3_ReCoun=0;
#endif
#if(UART4_EN==1)			
		char Receive4BUFF[40]={0};
		unsigned char UART4_ReCoun=0;
#endif
#if(UART5_EN==1)		
		char Receive5BUFF[40]={0};
		unsigned char UART5_ReCoun=0;
#endif


/*##############################����ΪUSART1��������#################################################*/
#if(USART1_EN==1)
void USART1_Init(void)
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


void USART1_PrintString(const char *String)
{
   const char *p;                            
   p=String;

   while(*p)
   {
      USART1->DR=*p;                        
      while(!(USART1->SR&(1<<6)));
      p++;
    }

}

void USART1_PrintNumber(unsigned long number)
{
      unsigned char usart_save[16]={0};
			unsigned char *usart_p;
			unsigned char i=1;
			unsigned int num=0;
			num=number;
				
			while(num/10)
			{
				num/=10;
				i++;
			}
			usart_p=usart_save;
			for(;i>0;i--)
			{
				usart_save[i-1]=number%10+48;
				number/=10;
			}
      while(*usart_p)
      {
      USART1->DR=*usart_p;                        
      while(!(USART1->SR&(1<<6)));
      usart_p++;
      }
	
}

void USART1_PrintInteger(int integer)
{
	 unsigned int a=0;
   if(integer >= 0)
		 USART1_PrintNumber(integer);
	 else
	 {
		 USART1_PrintString(" -");
		 a =  (~(unsigned int)(integer)) + 1;
		 USART1_PrintNumber(a);
	 }
}

#endif

