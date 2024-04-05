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
#ifndef __PLAINOS_CONFIG_H__
#define __PLAINOS_CONFIG_H__

/*************************************************************************************
 * kernel definitions
 *************************************************************************************/

/* maximum priority of plainos */
#define PL_CFG_ASSERT
#define PL_CFG_OS_CHAR_LOGO
#define PL_CFG_DEFAULT_MEMPOOL_SIZE             (10 * 1024)
#define PL_CFG_DEFAULT_MEMPOOL_GRAIN_ORDER      (4)
#define PL_CFG_MAX_TASKS_NUM                    (900u)
#define PL_CFG_TASK_PRIORITIES_MAX              (99u)
#define PL_CFG_IDLE_TASK_STACK_SIZE             (512)
#define PL_CFG_CPU_RATE_INTERVAL_TICKS          (102400)
//#define PL_CFG_SYSLOG_ANSI_COLOR
#define PL_CFG_SOFTTIMER_DAEMON_TASK_STACK_SIZE (512)
#define PL_CFG_HI_WORKQUEUE_TASK_STACK_SIZE     (512) /* priority is highest */
#define PL_CFG_LO_WORKQUEUE_TASK_PRIORITY       (PL_CFG_TASK_PRIORITIES_MAX)
#define PL_CFG_LO_WORKQUEUE_TASK_STACK_SIZE     (512)

#endif /* __PLAINOS_CONFIG_H__ */
