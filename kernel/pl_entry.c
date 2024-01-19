
#include <kernel/initcall.h>
#include "errno.h"
#include <pl_port.h>
#include <kernel/syslog.h>
#include "common/ansi_color.h"
#include <stdio.h>

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
	early_put_string((char*)"pure_initcall\r\n");
	//early_put_string(ANSI_COLOR_RESET);

	return 0;
}
core_initcall(UART_INIT0);

static int __init UART_INIT1(void)
{
	early_put_string((char*)"pure_initcall_sync\r\n");
	return 0;
}
pure_initcall_sync(UART_INIT1);

void pl_entry(void)
{
	uint32_t val;
	char val1;
	uint16_t val2;
	char str[40] = {0};

	pl_early_port_putc_init();
	initcalls_call();
	early_put_string((char*)"PlainOS\r\n");

	val = pl_port_rodata_read32((uint32_t *)0x058e);
	val2 = pl_port_rodata_read16((uint16_t *)0x058e);
	val1 = pl_port_rodata_read8((char *)0x058e);
	snprintf(str, sizeof(str), "0x%lx, 0x%x, 0x%x", val, val2, val1);
	
	early_put_string(str);
	while (1)
	{
		early_put_string((char*)ANSI_COLOR_FORE_YELLOW"PlainOS\r\n");
		delay(1000);
	}
}
