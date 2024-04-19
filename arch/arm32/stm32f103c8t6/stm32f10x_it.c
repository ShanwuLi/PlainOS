#include "stm32f10x_it.h"
#include <types.h>
#include <kernel/syslog.h>

void pl_sys_dump(u32_t *fault_args)
{
	pl_early_syslog_err("r0:0x%x\r\n",  fault_args[0]);
	pl_early_syslog_err("r1:0x%x\r\n",  fault_args[1]);
	pl_early_syslog_err("r2:0x%x\r\n",  fault_args[2]);
	pl_early_syslog_err("r3:0x%x\r\n",  fault_args[3]);
	pl_early_syslog_err("r12:0x%x\r\n", fault_args[4]);
	pl_early_syslog_err("lr:0x%x\r\n",  fault_args[5]);
	pl_early_syslog_err("pc:0x%x\r\n",  fault_args[6]);
	pl_early_syslog_err("psr:0x%x\r\n", fault_args[7]);
	while (true);
}

void USART1_IRQHandler(void)
{
	char recv_char;

	if(USART1->SR & (1 << 5)) {
		recv_char = USART1->DR;
		pl_port_putc(recv_char);
		pl_port_putc('@');
	}
}
