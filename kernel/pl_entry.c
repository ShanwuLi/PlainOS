
#include <kernel/initcall.h>
#include "errno.h"
#include <pl_port.h>
#include <kernel/syslog.h>
#include "common/ansi_color.h"
#include <stdio.h>
#include <kernel/task.h>
#include <pl_port.h>

static struct tcb task_tcb;
static u32_t __attribute__((aligned(8))) task_stack[256];

static int task(int argc, char *argv[])
{
	pl_early_syslog_warn("task===============================\r\n");
	USED(argc);
	USED(argv);
	int i = 0;
	while (1)
	{
		i++;
		pl_early_syslog_warn("task===============================\r\n");
		if (i > 12)
			i = 0;
	}

	return -100;
}



static void delay(int x)
{
    volatile int i,j;
    for (i = 0; i < x; i++)
    {
        for (j = 0; j < 10; j++)
            ;
    }
}

extern initcall_t __initcall0_start[];
extern initcall_t __initcall1_start[];
extern initcall_t __initcall2_start[];
extern initcall_t __initcall3_start[];
extern initcall_t __initcall4_start[];
extern initcall_t __initcall5_start[];
extern initcall_t __initcall6_start[];
extern initcall_t __initcall7_start[];
extern initcall_t __initcall8_start[];
extern initcall_t __initcall9_start[];
extern initcall_t __initcall_end[];

static initcall_t *initcall_levels[] = {
	__initcall0_start,
	__initcall1_start,
	__initcall2_start,
	__initcall3_start,
	__initcall4_start,
	__initcall5_start,
	__initcall6_start,
	__initcall7_start,
	__initcall8_start,
	__initcall9_start,
	__initcall_end,
};

void initcalls_call(void);
void initcalls_call(void)
{
	initcall_t **init_fns;
	initcall_t *init_fn;
	initcall_t init_f;

	for (init_fns = &initcall_levels[0]; init_fns < &initcall_levels[10]; init_fns++) {
		for (init_fn = *init_fns; init_fn < *(init_fns + 1); init_fn++) {
			init_f = (initcall_t)pl_port_rodata_read(init_fn);
			init_f();
		}
	}
}


static int __init UART_INIT0(void)
{
	//early_put_string(ANSI_COLOR_FORE_CYAN);
	pl_early_syslog_info("UART_INIT0\r\n");
	//early_put_string(ANSI_COLOR_RESET);

	return 0;
}
core_initcall(UART_INIT0);

static int __init UART_INIT1(void)
{
	pl_early_syslog_info("UART_INIT1\r\n");
	return 0;
}
pure_initcall_sync(UART_INIT1);

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
	pl_task_core_blk_init();




	pl_early_syslog_warn("task:0x%x\r\n", task);
	task_tid = pl_task_create_with_stack("task1", task, 1, &task_tcb, task_stack,
	                                      256, 0, NULL);


	pl_early_syslog_info("task_tid:%d\r\n", task_tid);
	//void *context_sp = pl_callee_update_context();
	//pl_early_syslog_warn("context_sp:0x%x\r\n", context_sp);
	pl_port_task_switch(NULL);



	while(1);
	while (1)
	{
		pl_early_syslog_err("PlainOS:%ld, counter:%d\r\n", 1233535, i++);
		pl_early_syslog_warn("PlainOS counter:0x%x\r\n", j + i);
		j++;
		delay(100000);
	}
}
