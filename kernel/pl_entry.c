
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
	pl_do_early_initcalls();
	pl_do_initcalls();

	pl_early_syslog_info("++++++++++++++++++++++++\r\n");
	pl_port_task_switch();
	while(1);
}
