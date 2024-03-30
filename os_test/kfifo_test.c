#include <kernel/initcall.h>
#include <kernel/kfifo.h>
#include <kernel/syslog.h>

static int fifo_test(void)
{
	
	return 0;
}

core_initcall(fifo_test);
