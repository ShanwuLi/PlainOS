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

#include <types.h>
#include <errno.h>
#include <kernel/syslog.h>
#include <kernel/initcall.h>
#include <drivers/serial/serial.h>


static int plsh_filter(struct pl_kfifo *recv_fifo, char *chars, uint_t chars_len)
{
	if (chars[chars_len - 1] == 0x08) {
		if (pl_kfifo_len(recv_fifo) == 0)
			return -EINVAL;

		pl_kfifo_back(recv_fifo);
		pl_early_syslog("\033[P");
		return -EINVAL;
	}

	return OK;
}

static int plsh_callback_condition(struct pl_kfifo *recv_fifo, char *chars, uint_t chars_len)
{
	USED(recv_fifo);

	pl_syslog_put_chars(pl_port_putc, chars, chars + chars_len - 1);
	if (chars[chars_len - 1] == '\r') {
		pl_port_putc('\n');
		return OK;
	}

	return -EINVAL;
}

static void plsh_callback(struct pl_kfifo *recv_fifo)
{
	char ch;

	while (recv_fifo->out != recv_fifo->in) {
		ch = recv_fifo->buff[recv_fifo->out & (recv_fifo->size - 1)];
		if (ch != '\r')
			pl_port_putc(ch);

		++recv_fifo->out;
	}

	pl_syslog("plsh# ");
}

static int pl_shell_init(void)
{
	int ret;
	struct pl_serial_desc *serial;

	serial = pl_serial_find_desc(0);
	if (serial == NULL) {
		pl_syslog_err("plsh init failed\r\n");
		return -EFAULT;
	}

	ret = pl_serial_register_recv_filter(serial, plsh_filter);
	if (ret < 0) {
		pl_syslog_err("plsh init failed, ret:%d\r\n", ret);
		return ret;
	}

	ret = pl_serial_register_recv_call_condition(serial,
	                            plsh_callback_condition);
	if (ret < 0) {
		pl_syslog_err("plsh init failed, ret:%d\r\n", ret);
		return ret;
	}

	ret = pl_serial_register_recv_callback(serial, plsh_callback);
	if (ret < 0) {
		pl_syslog_err("plsh init failed, ret:%d\r\n", ret);
		return ret;
	}

	pl_syslog_info("plainos shell init done\r\n");
	pl_syslog("plsh# ");
	return OK;
}
pl_late_initcall(pl_shell_init);