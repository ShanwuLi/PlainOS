#include "stm32f10x_it.h"
#include <types.h>
#include <kernel/syslog.h>

void pl_panic_dump(u32_t panic_reason, void *arg);
void pl_sys_dump(u32_t *fault_args)
{
	pl_early_syslog_err("system dump:\r\n");
	pl_early_syslog_err("r0:0x%x\r\n",  fault_args[0]);
	pl_early_syslog_err("r1:0x%x\r\n",  fault_args[1]);
	pl_early_syslog_err("r2:0x%x\r\n",  fault_args[2]);
	pl_early_syslog_err("r3:0x%x\r\n",  fault_args[3]);
	pl_early_syslog_err("r12:0x%x\r\n", fault_args[4]);
	pl_early_syslog_err("lr:0x%x\r\n",  fault_args[5]);
	pl_early_syslog_err("pc:0x%x\r\n",  fault_args[6]);
	pl_early_syslog_err("psr:0x%x\r\n", fault_args[7]);
	pl_panic_dump((u32_t)74827, NULL);
	while (true);
}
