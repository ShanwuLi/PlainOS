#include <port.h>
#include "mempool.h"
#include "syslog.h"
#include "task.h"
#include "idletask.h"
#include <kernel/assert.h>
#include <kernel/syslog.h>
#include <common/oschlogo.h>

void pl_callee_entry(void)
{
	int ret;
	ret = pl_port_putc_init();
	if (ret < 0)
		while(1);

	ret = pl_syslog_init();
	if (ret < 0)
		while(1);

	pl_early_syslog("PlainOS is starting...\r\n");
	pl_dis_os_char_logo();

	ret = pl_default_mempool_init();
	pl_assert(ret == 0);

	ret = pl_task_core_init();
	pl_assert(ret == 0);

	ret = pl_idle_task_init();
	pl_assert(ret == 0);

	while(1);
}
