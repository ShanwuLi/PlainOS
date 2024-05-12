#ifndef __EARLY_UART_H__
#define __EARLY_UART_H__
#include <types.h>

void USART1_Init(uint_t USART1_BaudRate);
int USART1_PrintChar(char c);

#endif /* __EARLY_UART_H__ */
