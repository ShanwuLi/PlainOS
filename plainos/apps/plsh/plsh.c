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
#include <config.h>
#include <kernel/syslog.h>
#include <kernel/initcall.h>
#include <drivers/serial/serial.h>

#define ASCLL_ENTER                    13
#define ASCLL_SPACE                    32
#define ASCLL_BACKSPACE                8

static int plsh_recv_process(struct pl_kfifo *recv_fifo,
                             char *chars, uint_t chars_len)
{
	uint_t i;
	int need_callback = 0;
	uint_t idx_mask = recv_fifo->size - 1;

	for (i = 0; i < chars_len; i++) {
		/* skip control characters */
		if (chars[i] < ASCLL_SPACE &&
		    chars[i] != ASCLL_BACKSPACE &&
		    chars[i] != ASCLL_ENTER)
			continue;

		/* delete char */
		if (chars[i] == ASCLL_BACKSPACE) {
			if (pl_kfifo_len(recv_fifo) == 0)
				continue;

			pl_port_putc(chars[i]);
			pl_port_putc('\033');
			pl_port_putc('[');
			pl_port_putc('P');
			recv_fifo->in--;
			continue;
		}

		/* put char to recv_fifo */
		if (pl_kfifo_len(recv_fifo) < recv_fifo->size) {
			recv_fifo->buff[recv_fifo->in & idx_mask] = chars[i];
			recv_fifo->in++;
		}

		/* skip enter key when put chars */
		if (chars[i] == ASCLL_ENTER) {
			need_callback = 1;
			continue;
		}

		/* put chars */
		pl_port_putc(chars[i]);
	}

	return need_callback;
}

static void plsh_callback(struct pl_kfifo *recv_fifo)
{
	char ch;
	pl_syslog("\r\n");

	while (pl_kfifo_len(recv_fifo) != 0) {
		ch = recv_fifo->buff[recv_fifo->out & (recv_fifo->size - 1)];
		++recv_fifo->out;
		if (ch != ASCLL_ENTER)
			pl_port_putc(ch);
	}
	pl_syslog(PL_CFG_SHELL_PREFIX_NAME"# ");
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

	ret = pl_serial_register_recv_process(serial, plsh_recv_process);
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
	pl_syslog(PL_CFG_SHELL_PREFIX_NAME"# ");
	return OK;
}
pl_late_initcall(pl_shell_init);