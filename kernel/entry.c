
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

volatile u32_t idle_task2_run_count = 0;
volatile u32_t idle_task1_run_count = 0;

static int idle_task2(int argc, char *argv[])
{
	argc = 0;
	USED(argv);

	pl_syslog_info("+++++++++++8\r\n");

	while(argc < 100) {
		//pl_schedule_lock();
		pl_syslog_err("+++++++++++++++++++++++++++++++++++++++++++++++\r\n");
		idle_task2_run_count++;
		//pl_schedule_unlock();
		pl_task_delay_ticks(100);
		argc++;
	}

	pl_syslog_err("idle_task2 exit\r\n");
	//pl_assert(true);

	return -200;
}

static int idle_task1(int argc, char *argv[])
{
	USED(argc);
	USED(argv);
	int ret;
	tid_t tcb;
	u32_t int_part;
	u32_t deci_part;

	pl_syslog_err("////////////////8\r\n");
	tcb = pl_task_create("idle_task2", idle_task2, PL_CFG_PRIORITIES_MAX - 1, 512, 0, NULL);

	pl_task_join(tcb, &ret);
	pl_syslog_info("%s: ret:%d\r\n", "idle_task1", ret);
	pl_syslog_warn("%s: ret:%d\r\n", "idle_task1", 1000);
	pl_syslog_err("%s: ret:%d\r\n", "idle_task1", 26358);
	while(1) {
		for (volatile int i = 0; i < 1000000; i++);
		pl_task_get_cpu_rate(&int_part, &deci_part);
		pl_syslog_info("cpu_rate:%u.%u%, \r\n", int_part, deci_part);
		//pl_schedule_lock();
		pl_syslog_err("////////////////////////////////////////////\r\n");
		idle_task1_run_count++;
		//pl_schedule_unlock();
		//pl_task_delay_ticks(1);
	}

	return 900;
}

static int idle_task(int argc, char *argv[])
{
	USED(argc);
	USED(argv);
	//u32_t cpu_rate_base;
	//u32_t cpu_rate_useful;

	pl_port_systick_init();
	pl_syslog_info("============8\r\n");
	pl_task_create("idle_task1", idle_task1, PL_CFG_PRIORITIES_MAX - 1, 512, 0, NULL);

	while(1) {
		for (volatile int i = 0; i < 1000000; i++);
		//pl_schedule_lock();
		//pl_early_syslog_info("========================\r\n");
		//pl_schedule_unlock();
		//pl_schedule_lock();
		pl_syslog_warn("============================================================================\r\n");
		//pl_task_get_cpu_rate(&cpu_rate_base, &cpu_rate_useful);
		//pl_syslog_info("cpu_rate:%u.%u, \r\n", cpu_rate_useful * 100 / cpu_rate_base,
		//     cpu_rate_useful * 10000 / cpu_rate_base - (cpu_rate_useful * 100 / cpu_rate_base) * 100);
		//pl_schedule_unlock();
	}

	return 0;
}

static int pl_idle_task_init(void)
{
	pl_task_create("idle_task", idle_task, PL_CFG_PRIORITIES_MAX, 512, 0, NULL);

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
