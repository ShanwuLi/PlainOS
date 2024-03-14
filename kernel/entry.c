#include <types.h>

void pl_callee_entry(void)
{
	int ret;
	pl_syslog_init();
	ret = pl_port_putc_init();
	if (ret < 0)
		while(1);

	pl_default_mempool_init();
	pl_do_early_initcalls();
	pl_do_initcalls();
	pl_task_core_init();
	pl_idle_task_init();
	while(1);
}

