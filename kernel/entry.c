
#include <kernel/initcall.h>
#include "errno.h"
#include <port.h>
#include <types.h>
#include <kernel/syslog.h>
#include "common/ansi_color.h"
#include "initcall_private.h"
#include <kernel/task.h>
#include <kernel/kernel.h>
#include "task_private.h"
#include <kernel/mempool.h>
#include "syslog_private.h"

static u32_t g_pl_idle_task_stack[128];

static u32_t g_pl_idle_task_stack1[128];

static u32_t g_pl_idle_task_stack2[128];

volatile u32_t idle_task2_run_count = 0;
volatile u32_t idle_task1_run_count = 0;

static int idle_task2(int argc, char *argv[])
{
	argc = 0;
	USED(argv);

	pl_syslog_info("+++++++++++8\r\n");

	while(argc < 900) {
		//pl_schedule_lock();
		pl_syslog_err("+++++++++++++++++++++++++++++++++++++++++++++++\r\n");
		idle_task2_run_count++;
		//pl_schedule_unlock();
		//pl_task_delay_ticks(1000);
		argc++;
	}

	//pl_assert(true);

	return -200;
}

static int idle_task1(int argc, char *argv[])
{
	USED(argc);
	USED(argv);
	//int ret;
	//struct tcb *tcb;

	pl_syslog("////////////////8\r\n");
	pl_task_create_with_stack("idle_task2", idle_task2, PL_CFG_PRIORITIES_MAX,
	                           g_pl_idle_task_stack2,
	                           sizeof(g_pl_idle_task_stack2), 0, NULL);

	//pl_task_join(tcb, &ret);
	//pl_early_syslog_info("%s: ret:%d\r\n", "idle_task1", ret);
	while(1) {
		//pl_schedule_lock();
		pl_syslog_err("////////////////////////////////////////////\r\n");
		idle_task1_run_count++;
		//pl_schedule_unlock();
		//pl_task_delay_ticks(50);
	}

	return 900;
}

static int idle_task(int argc, char *argv[])
{
	USED(argc);
	USED(argv);

	pl_port_systick_init();
	pl_syslog_info("============8\r\n");
	pl_task_create_with_stack("idle_task1", idle_task1, PL_CFG_PRIORITIES_MAX,
	                           g_pl_idle_task_stack1,
	                           sizeof(g_pl_idle_task_stack1), 0, NULL);

	while(1) {
		//pl_schedule_lock();
		//pl_early_syslog_info("========================\r\n");
		//pl_schedule_unlock();
		for (volatile int i = 0; i < 10000; i++)
		;
		//pl_schedule_lock();
		pl_syslog_warn("====================================================\r\n");
		//pl_schedule_unlock();
	}

	return 0;
}

static int pl_idle_task_init(void)
{
	pl_early_syslog_info("g_pl_idle_task_stack:0x%x\r\n", g_pl_idle_task_stack);
	pl_task_create_with_stack("idle_task", idle_task, PL_CFG_PRIORITIES_MAX,
	                            g_pl_idle_task_stack,
	                           sizeof(g_pl_idle_task_stack), 0, NULL);

	return 0;
}


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
