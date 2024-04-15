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
#include "mempool.h"
#include "syslog.h"
#include "task.h"
#include "idletask.h"
#include <kernel/assert.h>
#include <kernel/syslog.h>
#include <common/oschlogo.h>

void pl_callee_entry(void)
{
	int ret;
	ret = pl_port_putc_init();
	if (ret < 0)
		while(1);

	ret = pl_syslog_init();
	if (ret < 0)
		while(1);

	pl_early_syslog("PlainOS is starting...\r\n");
	pl_dis_os_char_logo();

	ret = pl_default_mempool_init();
	pl_assert(ret == 0);

	ret = pl_task_core_init();
	pl_assert(ret == 0);

	ret = pl_idle_task_init();
	pl_assert(ret == 0);

	while(true);
}
