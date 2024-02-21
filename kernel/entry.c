
#include <kernel/initcall.h>
#include "errno.h"
#include <pl_port.h>
#include <kernel/syslog.h>
#include "common/ansi_color.h"
#include "internal_initcall.h"
#include <kernel/task.h>
#include <pl_port.h>


static u32_t g_pl_idle_task_stack[128];
static struct tcb g_pl_idle_task_tcb;

static u32_t g_pl_idle_task_stack1[128];
static struct tcb g_pl_idle_task_tcb1;

static u32_t g_pl_idle_task_stack2[128];
static struct tcb g_pl_idle_task_tcb2;


extern int RTS_PORT_SystickInit(void);

volatile u32_t idle_task2_run_count;
volatile u32_t idle_task1_run_count;




static int idle_task2(int argc, char *argv[])
{
	USED(argc);
	USED(argv);

	while(1) {
		//pl_schedule_lock();
		//pl_early_syslog("+");
		idle_task2_run_count++;
		//pl_schedule_unlock();
		pl_delay_ticks(20);
	}

	return 0;
}

static int idle_task1(int argc, char *argv[])
{
	USED(argc);
	USED(argv);

	pl_task_create_with_stack("idle_task2", idle_task2, PL_CFG_PRIORITIES_MAX,
	                           &g_pl_idle_task_tcb2, g_pl_idle_task_stack2,
	                           sizeof(g_pl_idle_task_stack2), 0, NULL);

	while(1) {
		//pl_schedule_lock();
		//pl_early_syslog("/");
		idle_task1_run_count++;
		//pl_schedule_unlock();
		pl_delay_ticks(20);
	}

	return 0;
}


static int idle_task(int argc, char *argv[])
{
	USED(argc);
	USED(argv);

	RTS_PORT_SystickInit();
	pl_early_syslog_info("=\r\n");
	pl_task_create_with_stack("idle_task1", idle_task1, PL_CFG_PRIORITIES_MAX,
	                           &g_pl_idle_task_tcb1, g_pl_idle_task_stack1,
	                           sizeof(g_pl_idle_task_stack1), 0, NULL);

	while(1) {
		//pl_schedule_lock();
		//pl_early_syslog_info("========================\r\n");
		//pl_schedule_unlock();
		for (volatile int i = 0; i < 10000; i++)
		;
		pl_schedule_lock();
		pl_early_syslog_info("\r\n idle_task1_run_count:%d, idle_task2_run_count:%d\r\n",
		idle_task1_run_count, idle_task2_run_count);
		pl_schedule_unlock();
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
