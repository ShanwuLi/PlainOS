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

#include <port/port.h>
#include <kernel/assert.h>
#include <kernel/kernel.h>
#include <kernel/task.h>
#include <kernel/syslog.h>
#include "task.h"
#include "initcall.h"
#include "inittask.h"

/*************************************************************************************
 * Function Name: init_task
 * Description: init task for system.
 ************************************************************************************/
static int init_task(int argc, char *argv[])
{
	USED(argc);
	USED(argv);

	int ret = pl_port_systick_init();
	pl_assert(ret == 0);

	pl_do_initcalls();

	pl_syslog_info("init task done\r\n");
	return 0;
}

/*************************************************************************************
 * Function Name: pl_init_task_init
 * Description: initialization task.
 *
 * Param:
 *   none.
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_init_task_init(void)
{
	pl_tid_t init_taskid;

	init_taskid = pl_task_sys_create("init_task", init_task, 0,
	                                 CONFIG_PL_INIT_TASK_STACK_SIZE, 0, NULL);
	if (init_taskid == NULL) {
		pl_early_syslog_err("init task create failed\r\n");
		return -1;
	}

	return 0;
}
