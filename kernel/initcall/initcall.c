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

#include <errno.h>
#include <pl_port.h>
#include <kernel/initcall.h>
#include "internal_initcall.h"
#include <kernel/syslog.h>

extern initcall_t __early_initcall_start[];
extern initcall_t __early_initcall_end[];
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

/*************************************************************************************
 * Function Name: call_initcall_level
 *
 * Description:
 *   The function is used to call init_levels.
 *
 * Parameters:
 *   @init_fns_start: init_fns array start.
 *   @init_fns_end: init_fns array end.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
static int call_initcall_level(initcall_t *init_fns_start,
                               initcall_t *init_fns_end)
{
	int ret;
	initcall_t *init_fn;
	initcall_t init_f;

	for (init_fn = init_fns_start; init_fn < init_fns_end; init_fn++) {
		init_f = (initcall_t)pl_port_rodata_read(init_fn);
		ret = init_f();
		if (ret < 0)
			return ret;
	}

	return OK;
}

/*************************************************************************************
 * Function Name: pl_do_early_initcalls
 *
 * Description:
 *   The function is used to call early initcalls.
 *
 * Parameters:
 *   none.
 *
 * Return:
 *   none.
 ************************************************************************************/
void pl_do_early_initcalls(void)
{
	int ret;
	initcall_t *init_fn;
	initcall_t init_f;

	for (init_fn = __early_initcall_start; init_fn < __early_initcall_end;
	     init_fn++) {
		init_f = (initcall_t)pl_port_rodata_read(init_fn);
		ret = init_f();
		if (ret < 0) {
			pl_early_syslog_err("early init call return error:%d\r\n", ret);
			while(1);
		}
	}
}

/*************************************************************************************
 * Function Name: pl_do_initcalls
 *
 * Description:
 *   The function is used to call initcalls.
 *
 * Parameters:
 *   none.
 *
 * Return:
 *   none.
 ************************************************************************************/
void pl_do_initcalls(void)
{
	int ret;
	initcall_t **init_fns;

	for (init_fns = &initcall_levels[0]; init_fns < &initcall_levels[10];
	     init_fns++) {
		ret = call_initcall_level(*init_fns, *(init_fns + 1));
		if (ret < 0) {
			pl_early_syslog_err("init call return error:%d\r\n", ret);
			while(1);
		}
	}
}
