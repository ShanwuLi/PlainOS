/* MIT License

Copyright (c) 2023 PlainOS

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <port.h>
#include <kernel/kernel.h>
#include <kernel/task.h>
#include <kernel/syslog.h>
#include "initcall.h"
#include "idletask.h"
#include "softtimer.h"

/*************************************************************************************
 * Function Name: idle_task
 * Description: idle task for system.
 ************************************************************************************/
static int idle_task(int argc, char *argv[])
{
	USED(argc);
	USED(argv);
	pl_do_early_initcalls();
	pl_port_systick_init();
	pl_softtimer_core_init();
	pl_do_initcalls();

	while(1) {
		//pl_early_syslog("idletask===============================================\r\n");
		for (volatile int i = 0; i < 10000; i++);
	}

	return 0;
}

/*************************************************************************************
 * Function Name: pl_idle_task_init
 * Description: idle task init.
 *
 * Param:
 *   none.
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_idle_task_init(void)
{
	pl_task_create("idle_task", idle_task, PL_CFG_TASK_PRIORITIES_MAX,
	                PL_CFG_IDLE_TASK_STACK_SIZE, 0, NULL);
	return 0;
}
