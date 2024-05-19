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
#include <kernel/task.h>
#include <kernel/syslog.h>
#include <kernel/initcall.h>
#include <kernel/mempool.h>
#include <drivers/serial/serial.h>

#define ASCLL_ENTER                    13
#define ASCLL_SPACE                    32
#define ASCLL_BACKSPACE                8

enum cmd_parse_state {
	CMD_PARSE_STATE_INIT = 0,
	CMD_PARSE_STATE_CMD,
	CMD_PARSE_STATE_ARG,
	CMD_PARSE_STATE_END,
};

static int plsh_cmd_argc = 0;
static char *plsh_cmd_argvs[PL_CFG_SHELL_CMD_ARGC_MAX];
static char plsh_cmd_buffer[PL_CFG_SHELL_CMD_BUFF_MAX];

/*************************************************************************************
 * @breaf: copy the cmd to plsh_cmd_buffer.
 *
 * @param recv_fifo: fifo of receiving characters.
 *
 * @return: if seuccess, return OK, else return error code.
 ************************************************************************************/
static int plsh_get_argvs_from_buffer( char *cmd_buff, int cmd_argc, char **cmd_argvs)
{
	char ch;
	int i = 0;
	int j = 0;
	bool new_arg = false;

	/* return error straightly */
	if (cmd_argc == 0)
		return -ENOEXEC;

	/* get the argvs */
	cmd_argvs[j++] = cmd_buff;
	do {
		ch = cmd_buff[i];
		if (new_arg) {
			new_arg = false;
			cmd_argvs[j++] = cmd_buff + i;
		}

		if (ch == ASCLL_SPACE) {
			new_arg = true;
			cmd_buff[i] = '\0';
		}

		if (j >= PL_CFG_SHELL_CMD_ARGC_MAX)
			return -ERANGE;

		i++;
	} while (ch);

	return OK;
}

/*************************************************************************************
 * @breaf: parse the cmd.
 *
 * @param recv_fifo: fifo of receiving characters.
 *
 * @return: none.
 ************************************************************************************/
static int plsh_cmd_parse(struct pl_kfifo *recv_fifo)
{
	int ret;
	char ch;
	int i = 0;
	int state = CMD_PARSE_STATE_INIT;

	/* copy cmd to plsh_cmd_buffer */
	plsh_cmd_argc = 0;
	while (pl_kfifo_len(recv_fifo) != 0) {
		ch = recv_fifo->buff[recv_fifo->out & (recv_fifo->size - 1)];
		++recv_fifo->out;

		/* check ending condition */
		if (ch == ASCLL_ENTER || i == PL_CFG_SHELL_CMD_BUFF_MAX - 1) {
			if (state == CMD_PARSE_STATE_CMD)
				plsh_cmd_argc++;

			plsh_cmd_buffer[i] = '\0';
			break;
		}

		/* state machine for get the cmd */
		switch (state) {
		case CMD_PARSE_STATE_INIT:
			if (ch != ASCLL_SPACE) {
				state = CMD_PARSE_STATE_CMD;
				plsh_cmd_buffer[i++] = ch;
			}
			break;

		case CMD_PARSE_STATE_CMD:
			if (ch == ASCLL_SPACE) {
				state = CMD_PARSE_STATE_ARG;
				plsh_cmd_argc++;
			}
			plsh_cmd_buffer[i++] = ch;
			break;

		case CMD_PARSE_STATE_ARG:
			if (ch != ASCLL_SPACE) {
				state = CMD_PARSE_STATE_CMD;
				plsh_cmd_buffer[i++] = ch;
			}
			break;

		default:
			break;
		}
	}

	ret = plsh_get_argvs_from_buffer(plsh_cmd_buffer, plsh_cmd_argc, plsh_cmd_argvs);
	if (ret < 0) {
		return ret;
	}

	pl_early_syslog_info("plsh_cmd_buffer, argc:%d, argv:%s\r\n",
	                      plsh_cmd_argc, plsh_cmd_buffer);
	
	for (i = 0; i < plsh_cmd_argc; i++) {
		pl_early_syslog_info("argv[%d]:%s\r\n", i, plsh_cmd_argvs[i]);
	}

	return OK;
}

/*************************************************************************************
 *
 * @brief: precess the received characters.
 * 
 * @param recv_fifo: fifo of receiving characters.
 * @param chars: received characters.
 * @param chars_len: length of received characters.
 *
 * @return: if success, return 0; otherwise return negative value.
 ************************************************************************************/
static int plsh_recv_process(struct pl_kfifo *recv_fifo, char *chars, uint_t chars_len)
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
		if (pl_kfifo_len(recv_fifo) >= recv_fifo->size) {
			pl_early_syslog_err("recv fifo is full\r\n");
			break;
		}

		recv_fifo->buff[recv_fifo->in & idx_mask] = chars[i];
		recv_fifo->in++;

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

/*************************************************************************************
 *
 * @brief: callback of serial port.
 * 
 * @param recv_fifo: fifo of receiving characters.
 *
 * @return none.
 ************************************************************************************/
static void plsh_callback(struct pl_kfifo *recv_fifo)
{
	pl_syslog("\r\n");
	plsh_cmd_parse(recv_fifo);

	/* create task for cmd execution */
	pl_syslog(PL_CFG_SHELL_PREFIX_NAME"# ");
}

/*************************************************************************************
 *
 * @brief: shell initialization.
 * 
 * @param none.
 *
 * @return: if success, return 0; otherwise return negative value..
 ************************************************************************************/
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
