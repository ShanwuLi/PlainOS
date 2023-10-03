/**************************************************************************
* �ļ�����USART.h                                                         *
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
#ifndef   __USART_H
#define   __USART_H

#include  "stm32f10x.h"

/**###########################�ļ����ã��û������Լ��������#########################*/
//��Ҫʹ��������Ϊ1������Ϊ0
#define   USART1_EN                      1
#define   USART2_EN                      0   
#define   USART3_EN                      0
#define   UART4_EN                       0
#define   UART5_EN                       0
/**###########################�������岿�֣����û�Ҫ���޸�#########################*/

#define  USART1_BaudRate       115200        //���������ã�Ĭ��9600
#define  USART2_BaudRate       9600        //���������ã�Ĭ��9600
#define  USART3_BaudRate       9600        //���������ã�Ĭ��9600
#define  UART4_BaudRate        9600        //���������ã�Ĭ��9600
#define  UART5_BaudRate        9600        //���������ã�Ĭ��9600

/*#############################����ΪUSRAT1�ӿں���############################################*/
//��ע��USART1����ΪPA9��PA10��ʹ��USART1�������������޷��������
#if(USART1_EN==1)
void USART1_Init(void);                                
void USART1_PrintString(const  char *String);
#endif
/*#############################����ΪUSRAT2�ӿں���############################################*/
//��ע��USART1����ΪPA3��PA2��ʹ��USART2�������������޷��������
#if(USART2_EN==1)	 
void USART2_Init(void);
void USART2_PrintString( char *String);
unsigned char USART2_ReceiveByte(void);
void USART2_PrintNumber(unsigned long number);
extern char Receive2BUFF[40];
#endif
/*#############################����ΪUSRAT3�ӿں���############################################*/
//��ע��USART3����ΪPB11��PB10��ʹ��USART3�������������޷��������
#if(USART3_EN==1)	
void USART3_Init(void);
void USART3_PrintString( char *String);
unsigned char USART3_ReceiveByte(void);
void USART3_PrintNumber(unsigned long number);
extern char Receive3BUFF[40];
#endif
/*#############################����ΪURAT4�ӿں���############################################*/
//��ע��UART4����ΪPC11��PC10��ʹ��UART4�������������޷��������
#if(UART4_EN==1)
void UART4_Init(void);
void UART4_PrintString( char *String);
unsigned char UART4_ReceiveByte(void);
void UART4_PrintNumber(unsigned long number);
extern char Receive4BUFF[40];
#endif

/*#############################����ΪURAT5�ӿں���############################################*/
//��ע��UART5����ΪPC12��PD2��ʹ��UART5�������������޷��������
#if(UART5_EN==1)	
void UART5_Init(void);
void UART5_PrintString( char *String);
unsigned char UART5_ReceiveByte(void);
void UART5_PrintNumber(unsigned long number);
extern char Receive5BUFF[40];
#endif

#endif


