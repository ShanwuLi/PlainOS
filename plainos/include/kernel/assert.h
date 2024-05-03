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

#ifndef __KERNEL_ASSERT_H__
#define __KERNEL_ASSERT_H__

#include <config.h>
#include <port.h>
#include <kernel/task.h>
#include <kernel/syslog.h>
#include <kernel/kernel.h>

/*************************************************************************************
 * Function Name: pl_assert
 * Description: assertion for PlainOS.
 *
 * Parameters:
 *   @assertion: if assertion is true, OS will be block here.
 *
 * Return:
 *   void.
 ************************************************************************************/
#ifdef PL_CFG_ASSERT
#define pl_assert(assertion)    \
		do { \
			if (!(assertion)) { \
				pl_port_enter_critical(); \
				pl_early_syslog_err("=>[ASSERT]:func:%s, line:%d\r\n", \
				                   __func__, __LINE__); \
				pl_port_exit_critical(); \
				pl_task_pend(NULL);\
			} \
		} while (false)
#else
#define pl_assert(assertion)   ((void)(assertion))
#endif /* PL_CFG_ASSERT */


#endif /* __KERNEL_ASSERT_H__ */
