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
static u32_t recv_fifo[256];
static bool recv_enabled = false;

void USART1_IRQHandler(void)
{
	char recv_char;

	if(USART1->SR & (1 << 5)) {
		recv_char = USART1->DR;
	}

	if (recv_enabled)
		pl_serial_callee_recv_handler(&stm32f10x_serial_desc, &recv_char, 1);
}

static int stm32f10x_serial_recv_enable(struct pl_serial_desc *desc)
{
	USED(desc);
	pl_port_enter_critical();
	recv_enabled = true;
	pl_port_exit_critical();
	return 0;
}

static int stm32f10x_serial_recv_disable(struct pl_serial_desc *desc)
{
	USED(desc);
	pl_port_enter_critical();
	recv_enabled = false;
	pl_port_exit_critical();
	return 0;
}

static struct pl_serial_ops stm32f10x_serial_ops = {
	.send_char = NULL,
	.send_str = NULL,
	.set_baud_rate = NULL,
	.set_data_bits = NULL,
	.set_parity_bit = NULL,
	.set_stop_bits = NULL,
	.recv_char = NULL,
	.recv_disable = stm32f10x_serial_recv_disable,
	.recv_enable = stm32f10x_serial_recv_enable,
};

static int stm32f10x_serial_init(void)
{
	int ret;

	ret = pl_serial_desc_init(&stm32f10x_serial_desc, 0, &stm32f10x_serial_ops,
	                          (char *)recv_fifo, 1024);
	if (ret < 0) {
		pl_syslog_err("stm32f10x serial init failed, ret:%d\r\n", ret);
		return ret;
	}

	ret = pl_serial_desc_register(&stm32f10x_serial_desc);
	if (ret < 0) {
		pl_syslog_err("stm32f10x serial register failed, ret:%d\r\n", ret);
		return ret;
	}

	pl_syslog_info("stm32f10x serial init done\r\n");
	return 0;
}
pl_bsp_initcall(stm32f10x_serial_init);
