/* MIT License

Copyright (c) 2023 PlainOS

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <errno.h>
#include <types.h>
#include <kernel/initcall.h>
#include <kernel/syslog.h>
#include <drivers/serial/serial.h>
#include "../stm32f10x_it.h"

static struct pl_serial_desc stm32f10x_serial_desc;

static int callback_condition(struct pl_kfifo *recv_fifo, char *chars, uint_t chars_len)
{
	USED(recv_fifo);

	if (chars[chars_len - 1] == '\r') {
		return 0;
	}

	return -1;
}

static void callback(struct pl_kfifo *recv_fifo)
{
	USED(recv_fifo);
	pl_syslog_info("in:0x%x, out:0x%x\r\n", recv_fifo->in, recv_fifo->out);
	while (recv_fifo->out != recv_fifo->in) {
		pl_port_putc(recv_fifo->buff[recv_fifo->out & (recv_fifo->size - 1)]);
		++recv_fifo->out;
	}

	pl_port_putc('\r');
	pl_port_putc('\n');
}

void USART1_IRQHandler(void)
{
	char recv_char;

	if(USART1->SR & (1 << 5)) {
		recv_char = USART1->DR;
	}

	pl_serial_callee_recv_handler(&stm32f10x_serial_desc, &recv_char, 1);
}

static struct pl_serial_ops stm32f10x_serial_ops = {
	.send_char = NULL,
	.send_str = NULL,
	.set_baud_rate = NULL,
	.set_data_bits = NULL,
	.set_parity_bit = NULL,
	.set_stop_bits = NULL,
	.recv_char = NULL,
};

int stm32f10x_serial_init(void);
int stm32f10x_serial_init(void)
{
	int ret;

	ret = pl_serial_desc_init(&stm32f10x_serial_desc, 0,
	                          &stm32f10x_serial_ops, 1024);
	if (ret < 0) {
		pl_syslog_err("stm32f10x serial init failed, ret:%d\r\n", ret);
		return ret;
	}

	ret = pl_serial_desc_register(&stm32f10x_serial_desc);
	if (ret < 0) {
		pl_syslog_err("stm32f10x serial register failed, ret:%d\r\n", ret);
		return ret;
	}

	ret = pl_serial_register_recv_call_condition(&stm32f10x_serial_desc,
	            callback_condition);
	if (ret < 0) {
		pl_syslog_err("stm32f10x callback condition register failed, ret:%d\r\n", ret);
		return ret;
	}

	ret = pl_serial_register_recv_callback(&stm32f10x_serial_desc, callback);
	if (ret < 0) {
		pl_syslog_err("stm32f10x callback register failed, ret:%d\r\n", ret);
		return ret;
	}

	return 0;
}
pl_bsp_initcall(stm32f10x_serial_init);
