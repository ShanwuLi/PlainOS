#include <types.h>
#include <early_setup\early_uart.h>

void pl_callee_entry(void);
void pl_callee_entry(void)
{
	while(1){
		USART1_PrintChar('A');
	};
}

