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

#ifndef __KERNEL_SYSLOG_H__
#define __KERNEL_SYSLOG_H__

#include <pl_cfg.h>
#include <stdarg.h>
#include "../common/ansi_color.h"

#ifdef PL_CFG_SYSLOG_ANSI_COLOR
#define PL_EARLY_SYSLOG_WARN_ANSI_COLOR   ANSI_COLOR_FORE_YELLOW
#define PL_EARLY_SYSLOG_ERR_ANSI_COLOR    ANSI_COLOR_FORE_RED
#define PL_EARLY_SYSLOG_ANSI_COLOR_RESET  ANSI_COLOR_FORE_RESET
#else
#define PL_EARLY_SYSLOG_WARN_ANSI_COLOR
#define PL_EARLY_SYSLOG_ERR_ANSI_COLOR
#define PL_EARLY_SYSLOG_ANSI_COLOR_RESET NULL
#endif

void pl_put_format_log(int (*putc)(const char c), const char *front,
                       const char *rear, const char *fmt, ...);

#define pl_early_syslog(fmt, ...)  \
	pl_put_format_log(pl_port_early_putc, NULL, NULL, fmt, ## __VA_ARGS__)

#define pl_early_syslog_info(fmt, ...)  \
	pl_put_format_log(pl_port_early_putc, "[info]", NULL, fmt, ## __VA_ARGS__)

#define pl_early_syslog_warn(fmt, ...)  \
	pl_put_format_log(pl_port_early_putc, PL_EARLY_SYSLOG_WARN_ANSI_COLOR"[warn]", \
	PL_EARLY_SYSLOG_ANSI_COLOR_RESET, fmt, ## __VA_ARGS__)

#define pl_early_syslog_err(fmt, ...)  \
	pl_put_format_log(pl_port_early_putc, PL_EARLY_SYSLOG_ERR_ANSI_COLOR"[erro]", \
	PL_EARLY_SYSLOG_ANSI_COLOR_RESET, fmt, ## __VA_ARGS__)

#endif /* __KERNEL_SYSLOG_H__ */
