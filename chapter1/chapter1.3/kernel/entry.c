#include <types.h>
#include <early_setup/early_uart.h>
#include <cmsis/cmsis_gcc.h>
#include <stm32f10x_it.h>

void pl_callee_entry(void);
void pl_callee_entry(){
	uint_t USART1_BaudRate = 115200;
	USART1_Init(USART1_BaudRate);
	while(1){
		USART1_PrintChar('c');
		USART1_PrintChar('h');
		USART1_PrintChar('a');
		USART1_PrintChar('p');
		USART1_PrintChar('t');
		USART1_PrintChar('e');
		USART1_PrintChar('r');
		USART1_PrintChar('1');
		USART1_PrintChar('.');
		USART1_PrintChar('3');
		USART1_PrintChar('\t');
	}
}

