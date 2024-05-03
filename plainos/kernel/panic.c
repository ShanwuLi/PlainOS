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
#include <port.h>
#include <kernel/syslog.h>
#include "panic.h"

/*************************************************************************************
 * Function Name: pl_task_dump
 * Description: dump task info.
 *
 * Param:
 *   @tcb: dump task.
 *
 * Return:
 *   void.
 ************************************************************************************/
static void pl_task_dump(struct tcb *tcb)
{
	pl_early_syslog_err("task dump:\r\n");
	pl_early_syslog_err("task:                  %s\r\n", tcb->name);
	pl_early_syslog_err("task->prio:            %u\r\n", tcb->prio);
	pl_early_syslog_err("task->curr_state:      %d\r\n", tcb->curr_state);
	pl_early_syslog_err("task->parent:          %s\r\n", tcb->parent->name);
	pl_early_syslog_err("task->argc:            %d\r\n", tcb->argc);
	pl_early_syslog_err("task->argv:            0x%x\r\n", tcb->argv);
	pl_early_syslog_err("task->sp:              0x%x\r\n", tcb->context_sp);
	pl_early_syslog_err("task->sp_min:          0x%x\r\n", tcb->context_sp_min);
	pl_early_syslog_err("task->sp_max:          0x%x\r\n", tcb->context_sp_max);
	pl_early_syslog_err("task->wait_for_ret:    %d\r\n", tcb->wait_for_task_ret);
	pl_early_syslog_err("task->delay_ticks.lo32:%u\r\n", tcb->delay_ticks.lo32);
	pl_early_syslog_err("task->delay_ticks.hi32:%u\r\n", tcb->delay_ticks.hi32);
}

/*************************************************************************************
 * Function Name: pl_panic_dump
 * Description: dump task info when system panic.
 *
 * Param:
 *   @panic_tcb: tcb of panic task.
 *   @panic_reason: reason for panic.
 *   @arg: argument.
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_panic_dump(struct tcb *panic_tcb, u32_t panic_reason, void *arg)
{
	if (panic_tcb == NULL)
		return;

	pl_port_enter_critical();
	pl_early_syslog_err("\r\n");
	pl_early_syslog_err("=======================================================\r\n");
	pl_task_dump(panic_tcb);

	pl_early_syslog_err("\r\n");
	pl_early_syslog_err("PANIC REASON:\r\n");
	switch (panic_reason) {
	case PL_PANIC_REASON_STACKOVF:
		pl_early_syslog_err("[stack overflow] size of overflow:%d\r\n", (int)arg);
		break;

	default:
		pl_early_syslog_err("unknow reason\r\n");
	}

	pl_early_syslog_err("=======================================================\r\n");
	while (true);
}
