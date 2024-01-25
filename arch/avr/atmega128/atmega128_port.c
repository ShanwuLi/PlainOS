#include "early_uart.h"
#include <pl_port.h>


static usart USART1={
    .ux_cofg = {
		.number = 1,
		.osc_hz = OSC_16000000,
		.ubrr = UBRR_115200,
		.data_bits_num = DATA_BITS_8,
		.stop_bits_num = STOP_BITS_1,
		.crc_type = CRC_FORBID,
		.wait_or_interrupt = 0,
	},
};

#define DDRC  *((volatile unsigned char*)0x34)  /*< DDRC寄存器地址  */
#define PORTC *((volatile unsigned char*)0x35)  /*< PORTC寄存器地址 */

int main(void)
{
	pl_entry();
}

int pl_early_port_putc_init(void)
{
	usart_init(&USART1);
	return 0;
}

int pl_early_port_putc(const char c)
{
	USART1.transmit_data(&USART1, c);
	return 0;
}

void pl_port_schedule(void)
{

}