#include <types.h>
#include <early_setup/early_uart.h>

void pl_callee_entry(void);
void pl_callee_entry(void)
{
	uint_t USART1_BaudRate = 115200;
	USART1_Init(USART1_BaudRate);
	while(1){
		USART1_PrintChar('A');
	};
}
