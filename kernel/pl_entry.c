
#include <kernel/initcall.h>
#include "errno.h"
#include <pl_port.h>
#include <kernel/syslog.h>
#include "common/ansi_color.h"
#include "initcall/internal_initcall.h"
#include <kernel/task.h>
#include <pl_port.h>


static u32_t g_pl_idle_task_stack[128];
static struct tcb g_pl_idle_task_tcb;

extern int RTS_PORT_SystickInit(void);
static int idle_task(int argc, char *argv[])
{
	USED(argc);
	USED(argv);

	RTS_PORT_SystickInit();

	while(1) {
		pl_early_syslog_info("idle task++++++++++++++++++++++\r\n");
		for (volatile int i = 0; i < 2000000; i++)
			;
		pl_early_syslog_info("systick\r\n");
	}

	return 0;
}

static int pl_idle_task_init(void)
{
	pl_early_syslog_info("g_pl_idle_task_stack:0x%x\r\n", g_pl_idle_task_stack);
	pl_task_create_with_stack("idle_task", idle_task, PL_CFG_PRIORITIES_MAX,
	                           &g_pl_idle_task_tcb, g_pl_idle_task_stack,
	                           sizeof(g_pl_idle_task_stack), 0, NULL);
	return 0;
}


void pl_callee_entry(void)
{
	pl_do_early_initcalls();
	pl_do_initcalls();
	pl_idle_task_init();
	while(1);
}
