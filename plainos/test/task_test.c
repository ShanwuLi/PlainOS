#include <config.h>
#include <errno.h>
#include <types.h>
#include <kernel/initcall.h>
#include <port/port.h>
#include <kernel/syslog.h>
#include "common/ansi_color.h"
#include <kernel/task.h>
#include <kernel/kernel.h>
#include <kernel/mempool.h>

volatile u32_t idle_task2_run_count = 0;
volatile u32_t idle_task1_run_count = 0;

static int idle_task2(int argc, char *argv[])
{
	argc = 0;

	while(argc < 10) {
		//pl_schedule_lock();
		pl_syslog_info("%s\r\n", argv[0]);
		idle_task2_run_count++;
		//pl_schedule_unlock();
		pl_task_delay_ticks(100);
		argc++;
	}

	//pl_assert(true);

	return -200;
}

static int idle_task1(int argc, char *argv[])
{
	USED(argc);
	USED(argv);
	int r;
	int ret;
	pl_tid_t tcb2;
	pl_tid_t tcb3;
	u32_t int_part;
	u32_t deci_part;

	char *argvs2[2] = {(char *)"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n", NULL};
	char *argvs3[2] = {(char *)"-----------------------------------------------------------\r\n", NULL};

	tcb2 = pl_task_create("idle_task2", idle_task2, CONFIG_PL_TASK_PRIORITIES_MAX - 1, 256, 1, argvs2);
	tcb3 = pl_task_create("idle_task3", idle_task2, CONFIG_PL_TASK_PRIORITIES_MAX - 2, 256, 1, argvs3);
	if (tcb2 == NULL || tcb3 == NULL) {
		pl_syslog_info("idle_task2 or idle_task3 create failed\r\n");
		return -ENOMEM;
	}

	r = pl_task_join(tcb2, &ret);
	if (r == OK)
		pl_syslog_info("%s: ret:%d\r\n", "idle_task2", ret);
	else
		pl_syslog_err("pl_task_join tcb2 failed\r\n");

	r = pl_task_join(tcb3, &ret);
	if (r == OK)
		pl_syslog_info("%s: ret:%d\r\n", "idle_task3", ret);
	else
		pl_syslog_err("pl_task_join tcb3 failed\r\n");

	while(1) {
		///for (volatile int i = 0; i < 1000000; i++);
		pl_task_get_cpu_rate(&int_part, &deci_part);
		pl_syslog_info("cpu_rate:%u.%u%\r\n", int_part, deci_part);
		//pl_schedule_lock();
		pl_syslog_err("////////////////////////////////////////////\r\n");
		idle_task1_run_count++;
		//pl_schedule_unlock();
		pl_task_delay_ticks(1000);
	}

	return 900;
}

static int task_test(void)
{
	pl_tid_t test_task;
	pl_syslog_info("task test\r\n");

	test_task = pl_task_create("idle_task1", idle_task1, CONFIG_PL_TASK_PRIORITIES_MAX - 1, 512, 0, NULL);
	if (test_task == NULL) {
		pl_syslog_err("test task create failed\r\n");
		return 0;
	}

	pl_syslog_info("task test done\r\n");

	return 0;
}
pl_late_initcall(task_test);
