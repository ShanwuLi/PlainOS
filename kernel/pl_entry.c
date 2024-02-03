
#include <kernel/initcall.h>
#include "errno.h"
#include <pl_port.h>
#include <kernel/syslog.h>
#include "common/ansi_color.h"
#include "initcall/internal_initcall.h"
#include <kernel/task.h>
#include <pl_port.h>


void pl_callee_entry(void)
{
	int i = 0;
	int j = 0;
	tid_t task_tid;
	pl_early_port_putc_init();

	pl_early_syslog_warn("PlainOS 0x%x, %s, %d\r\n", 0x89, "dwowocwvwv", -89);
	pl_early_syslog_warn("PlainOS 0x%x, %ld, %d\r\n", 0x89, 0xfffffff, 0x89);
	pl_early_syslog_warn("PlainOS 0x%x, %llu, %u\r\n", 87, 0xfffffff, 1233535);
	pl_early_syslog_warn("PlainOS 0x%fdgd, %s, %x\r\n", "dwowocwvwv", 1233535);

	initcalls_call();

	pl_early_syslog_warn("task:0x%x\r\n", task);
	task_tid = pl_task_create_with_stack("task1", task, 1, &task_tcb, task_stack,
	                                      256, 0, NULL);

	pl_early_syslog_info("task_tid:%d\r\n", task_tid);
	pl_port_task_switch(NULL);

	while(1);
}
