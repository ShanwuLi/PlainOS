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
#include <appcall.h>
#include <string.h>
#include <kernel/syslog.h>
#include <kernel/initcall.h>
#include <kernel/mempool.h>
#include <kernel/workqueue.h>
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

extern struct pl_app_entry __appcall_start[];
extern struct pl_app_entry __appcall_end[];

struct pl_shell {
	int cmd_argc;
	struct pl_serial_desc *desc;
	struct pl_work cmd_exec_work;
	char *cmd_argv[PL_CFG_SHELL_CMD_ARGC_MAX];
	char cmd_buffer[PL_CFG_SHELL_CMD_BUFF_MAX];
};

static struct pl_shell plsh;
/**************************************************************************************
 * @brief: Searches for an application entry with the given name.
 *
 * @param name: The name of the application to find, a null-terminated string.
 * @return: A pointer to the found application entry; returns NULL if no match is found.
 *************************************************************************************/
static struct pl_app_entry *plsh_find_app_entry(char *name)
{
	struct pl_app_entry *entry;

	for (entry = __appcall_start; entry < __appcall_end; entry++) {
		if (strcmp(entry->name, name) == 0) {
			return entry;
		}
	}

	return NULL;
}

/*************************************************************************************
 * @breaf: exec the cmd.
 *
 * @param work: work to be executed.
 *
 * @return none.
 ************************************************************************************/
static void plsh_exec_work(struct pl_work *work)
{
	USED(work);
	int ret;
	struct pl_app_entry *app_entry;

	app_entry = plsh_find_app_entry(plsh.cmd_argv[0]);
	if (app_entry == NULL) {
		pl_syslog_err("app[%s] not found\r\n", plsh.cmd_argv[0]);
		goto out;
	}

	/* disable receiving */
	plsh.desc->ops->recv_disable(plsh.desc);
	ret = app_entry->entry(plsh.cmd_argc, plsh.cmd_argv);
	if (ret < 0) {
		pl_syslog_err("app[%s] exec cmd failed, ret:%d\r\n",
		              app_entry->name, ret);
	}

out:
	/* enable receiving */
	plsh.desc->ops->recv_enable(plsh.desc);
	pl_syslog(PL_CFG_SHELL_PREFIX_NAME"# ");
}


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

		i++;
	} while (ch);

	return OK;
}

/*************************************************************************************
 * @breaf: copy the cmd to plsh_cmd_buffer.
 *
 * @param recv_fifo: copies a command from the receive FIFO to the command buffer.
 *
 * @return: None.
 ************************************************************************************/
static void plsh_copy_cmd_to_buffer(struct pl_kfifo *recv_fifo)
{
	char ch;
	int i = 0;
	int state = CMD_PARSE_STATE_INIT;

	/* copy cmd to plsh_cmd_buffer */
	plsh.cmd_argc = 0;
	while (pl_kfifo_len(recv_fifo) != 0) {
		ch = recv_fifo->buff[recv_fifo->out & (recv_fifo->size - 1)];
		++recv_fifo->out;

		/* check ending condition */
		if (ch == ASCLL_ENTER || i == PL_CFG_SHELL_CMD_BUFF_MAX - 1) {
			if (state == CMD_PARSE_STATE_CMD)
				plsh.cmd_argc++;

			plsh.cmd_buffer[i] = '\0';
			break;
		}

		/* state machine for get the cmd */
		switch (state) {
		case CMD_PARSE_STATE_INIT:
			if (ch != ASCLL_SPACE) {
				state = CMD_PARSE_STATE_CMD;
				plsh.cmd_buffer[i++] = ch;
			}
			break;

		case CMD_PARSE_STATE_CMD:
			if (ch == ASCLL_SPACE) {
				state = CMD_PARSE_STATE_ARG;
				plsh.cmd_argc++;
			}
			plsh.cmd_buffer[i++] = ch;
			break;

		case CMD_PARSE_STATE_ARG:
			if (ch != ASCLL_SPACE) {
				state = CMD_PARSE_STATE_CMD;
				plsh.cmd_buffer[i++] = ch;
			}
			break;

		default:
			break;
		}
	}
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

	/* copy cmd to plsh_cmd_buffer  */
	plsh_copy_cmd_to_buffer(recv_fifo);

	/* check argc */
	if (plsh.cmd_argc > PL_CFG_SHELL_CMD_ARGC_MAX) {
		pl_early_syslog_err("cmd argvs[%d] is too many, limited[%d]\r\n",
		                     plsh.cmd_argc, PL_CFG_SHELL_CMD_ARGC_MAX);
		return -ERANGE;
	}

	/* get the argvs from the buffer */
	ret = plsh_get_argvs_from_buffer(plsh.cmd_buffer, plsh.cmd_argc, plsh.cmd_argv);
	if (ret < 0) {
		return ret;
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
			pl_early_syslog_err("recv fifo is full[%d], in:%u, out:%u\r\n",
			                     pl_kfifo_len(recv_fifo), recv_fifo->in,
			                     recv_fifo->out);
			need_callback = 1;
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
	int ret;

	pl_syslog("\r\n");
	ret = plsh_cmd_parse(recv_fifo);
	if (ret < 0) {
		pl_syslog(PL_CFG_SHELL_PREFIX_NAME"# ");
		return;
	}

	/* add cmd exec work to low priority work queue */
	plsh.desc->ops->recv_disable(plsh.desc);
	ret = pl_work_add(g_pl_sys_lowq_handle, &plsh.cmd_exec_work);
	if (ret < 0)
		pl_syslog_err("work quenen is too busy\r\n");

}

/*************************************************************************************
 *
 * @brief: shell initialization.
 *
 * @param serial: serial port descriptor.
 *
 * @return none.
 ************************************************************************************/
static void pl_plsh_init(struct pl_serial_desc *serial)
{
	plsh.desc = serial;
	plsh.cmd_argc = 0;
	plsh.cmd_exec_work.fun = plsh_exec_work;
	plsh.cmd_exec_work.priv_data = &plsh;
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

	pl_plsh_init(serial);
	serial->ops->recv_enable(serial);

	pl_syslog_info("plainos shell init done\r\n");
	pl_syslog(PL_CFG_SHELL_PREFIX_NAME"# ");

	return OK;
}
pl_late_initcall(pl_shell_init);
