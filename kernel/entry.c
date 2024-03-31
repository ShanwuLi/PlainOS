#include <port.h>
#include "mempool.h"
#include "syslog.h"
#include "task.h"
#include "idletask.h"
#include <kernel/syslog.h>
#include <common/oschlogo.h>

void pl_callee_entry(void)
{
	int ret;
	pl_syslog_init();
	ret = pl_port_putc_init();
	if (ret < 0)
		while(1);

	pl_early_syslog("PlainOS is starting...\r\n");
	pl_dis_os_char_logo();
	pl_default_mempool_init();
	pl_task_core_init();
	pl_idle_task_init();
	while(1);
}
