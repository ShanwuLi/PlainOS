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

#include <config.h>
#ifdef PL_CFG_SHELL_SUPPORT

#include <types.h>
#include <errno.h>
#include <config.h>
#include <appcall.h>
#include <string.h>
#include <kernel/task.h>
#include <kernel/syslog.h>
#include <kernel/initcall.h>
#include <kernel/mempool.h>
#include <kernel/workqueue.h>
#include <drivers/serial/serial.h>

#define ASCLL_ENTER                    13
#define ASCLL_SPACE                    32
#define ASCLL_BACKSPACE                8

#define ANSI_BACKSPACE                 "\033[P"
#define SERIAL_PROCESS_NOT_CALL        (-1)

enum cmd_parse_state {
	CMD_PARSE_STATE_ERR = -1,
	CMD_PARSE_STATE_INIT = 0,
	CMD_PARSE_STATE_CMD,
	CMD_PARSE_STATE_ARG,
	CMD_PARSE_STATE_NORMAL,
	CMD_PARSE_STATE_END,
};

extern struct pl_app_entry __appcall_start[];
extern struct pl_app_entry __appcall_end[];

struct pl_shell {
	int state;
	int cmd_argc;
	int cmd_buffer_idx;
	pl_tid_t cmd_task;
	struct pl_serial_desc *desc;
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

	return NULL;

	for (entry = __appcall_start; entry < __appcall_end; entry++) {
		if (strcmp(entry->name, name) == 0) {
			return entry;
		}
	}

	return NULL;
}

static int plsh_process_received_chars(struct pl_shell *sh, char recv_ch)
{
	/* skip control characters */
	if (recv_ch < ASCLL_SPACE && recv_ch != ASCLL_ENTER)
		return CMD_PARSE_STATE_NORMAL;

	/* not support to delete char */
#if 0
	if (recv_ch == ASCLL_BACKSPACE) {
		if (sh->cmd_buffer_idx == 0) {
			sh->cmd_argc = 0;
			sh->state = CMD_PARSE_STATE_INIT;
			return CMD_PARSE_STATE_NORMAL;
		}

		--sh->cmd_buffer_idx;
		pl_port_putc(recv_ch);
		pl_syslog(ANSI_BACKSPACE);
		return CMD_PARSE_STATE_NORMAL;
	}
#endif

	pl_port_putc(recv_ch);
	/* check ending condition */
	if (sh->cmd_buffer_idx >= (PL_CFG_SHELL_CMD_BUFF_MAX - 1) ||
		recv_ch == ASCLL_ENTER) {
		if (sh->state == CMD_PARSE_STATE_CMD)
			sh->cmd_argc++;

		sh->cmd_buffer[sh->cmd_buffer_idx] = '\0';
		sh->cmd_buffer_idx = 0;
		pl_syslog("\r\n");
		return ASCLL_ENTER;
	}

	/* state machine for get the cmd */
	switch (sh->state) {
	case CMD_PARSE_STATE_INIT:
		if (recv_ch != ASCLL_SPACE) {
			sh->state = CMD_PARSE_STATE_CMD;
			sh->cmd_buffer[sh->cmd_buffer_idx++] = recv_ch;
		}
		break;

	case CMD_PARSE_STATE_CMD:
		if (recv_ch == ASCLL_SPACE) {
			sh->state = CMD_PARSE_STATE_ARG;
			sh->cmd_argc++;
		}
		sh->cmd_buffer[sh->cmd_buffer_idx++] = recv_ch;
		break;

	case CMD_PARSE_STATE_ARG:
		if (recv_ch != ASCLL_SPACE) {
			sh->state = CMD_PARSE_STATE_CMD;
			sh->cmd_buffer[sh->cmd_buffer_idx++] = recv_ch;
		}
		break;

	default:
		break;
	}

	return CMD_PARSE_STATE_NORMAL;
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


static void plsh_cmd_reset(	struct pl_shell *sh)
{
	sh->cmd_argc = 0;
	sh->cmd_argv[0] = '\0';
	sh->cmd_buffer_idx = 0;
	sh->state = CMD_PARSE_STATE_INIT;
}
/*************************************************************************************
 * @breaf: exec the cmd.
 *
 * @param work: work to be executed.
 *
 * @return none.
 ************************************************************************************/
static int plsh_exec_cmd_task(int argc, char *argv[])
{
	USED(argc);
	USED(argv);
	int ret;
	char recv_ch;
	struct pl_app_entry *app_entry;
	struct pl_kfifo *recv_fifo = &(plsh.desc->recv_info.fifo);

	pl_early_syslog(PL_CFG_SHELL_PREFIX_NAME"# ");
	plsh_cmd_reset(&plsh);
	while (true) {
		if (pl_kfifo_len(recv_fifo) == 0) {
			pl_task_pend(NULL);
			continue;
		}

		/* get char from recv_fifo */
		recv_ch = recv_fifo->buff[recv_fifo->out & (recv_fifo->size - 1)];
		pl_port_enter_critical();
		recv_fifo->out++;
		pl_port_exit_critical();

		/* process char */
		ret = plsh_process_received_chars(&plsh, recv_ch);
		if (ret != ASCLL_ENTER)
			continue;

		/* get argvs */
		ret = plsh_get_argvs_from_buffer(plsh.cmd_buffer, plsh.cmd_argc, plsh.cmd_argv);
		if (ret < 0) {
			plsh_cmd_reset(&plsh);
			pl_early_syslog(PL_CFG_SHELL_PREFIX_NAME"# ");
			continue;
		}

		/* find the app entry */
		app_entry = plsh_find_app_entry(plsh.cmd_argv[0]);
		if (app_entry == NULL) {
			pl_early_syslog_err("%s not found\r\n", plsh.cmd_argv[0]);
			pl_early_syslog(PL_CFG_SHELL_PREFIX_NAME"# ");
			plsh_cmd_reset(&plsh);
			continue;
		}

		/* exec the cmd */
		ret = app_entry->entry(plsh.cmd_argc, plsh.cmd_argv);
		if (ret < 0)
			pl_early_syslog_err("app[%s] exec cmd failed, ret:%d\r\n", app_entry->name, ret);

		plsh_cmd_reset(&plsh);
		pl_early_syslog(PL_CFG_SHELL_PREFIX_NAME"# ");
	}

	return -EUNKNOWE;
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
	pl_kfifo_put(recv_fifo, chars, chars_len);
	pl_task_resume(plsh.cmd_task);
	return SERIAL_PROCESS_NOT_CALL;
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
		pl_syslog_err("serial not found for %s\r\n", PL_CFG_SHELL_PREFIX_NAME);
		return OK;
	}

	ret = pl_serial_register_recv_process(serial, plsh_recv_process);
	if (ret < 0) {
		pl_syslog_err("serial register failed, ret:%d\r\n", ret);
		return OK;
	}

	plsh.cmd_task = pl_task_create(PL_CFG_SHELL_PREFIX_NAME, plsh_exec_cmd_task,
	                               PL_CFG_SHELL_CMD_EXEC_TASK_PRIORITY,
	                               PL_CFG_SHELL_CMD_EXEC_TASK_STACK_SIZE, 0, NULL);
	if (plsh.cmd_task == NULL) {
		pl_syslog_err("%s task create failed\r\n", PL_CFG_SHELL_PREFIX_NAME);
		return OK;
	}

	plsh.desc = serial;
	plsh.cmd_argc = 0;
	pl_syslog_info("plainos shell init done\r\n");
	return OK;
}
pl_late_initcall(pl_shell_init);

#endif /* PL_CFG_SHELL_SUPPORT */
