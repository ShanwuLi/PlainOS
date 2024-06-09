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

#include <config.h>
#include <stdarg.h>
#include <port.h>
#include "../common/ansi_color.h"


#ifdef PL_CFG_SYSLOG_ANSI_COLOR
	#define PL_EARLY_SYSLOG_WARN_ANSI_COLOR   ANSI_COLOR_FORE_YELLOW
	#define PL_EARLY_SYSLOG_ERR_ANSI_COLOR    ANSI_COLOR_FORE_RED
	#define PL_SYSLOG_WARN_ANSI_COLOR         ANSI_COLOR_FORE_YELLOW
	#define PL_SYSLOG_ERR_ANSI_COLOR          ANSI_COLOR_FORE_RED
	#define PL_EARLY_SYSLOG_ANSI_COLOR_RESET  ANSI_COLOR_FORE_RESET
	#define PL_SYSLOG_ANSI_COLOR_RESET        ANSI_COLOR_FORE_RESET
#else
	#define PL_EARLY_SYSLOG_WARN_ANSI_COLOR
	#define PL_EARLY_SYSLOG_ERR_ANSI_COLOR
	#define PL_SYSLOG_WARN_ANSI_COLOR
	#define PL_SYSLOG_ERR_ANSI_COLOR
	#define PL_EARLY_SYSLOG_ANSI_COLOR_RESET
	#define PL_SYSLOG_ANSI_COLOR_RESET 
#endif


#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************
 * 函数名称: pl_put_format_log_locked
 * 
 * 描述:
 *   格式化字符串输出（在PlainOS未启动的情况下）。
 *
 * 输出格式:
 *   [$(前缀)][$(格式内容)][$(后缀)]
 *
 * 参数:
 *  @rear: 格式内容后的字符串。
 *  @fmt: 需要格式化的字符串。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
void pl_put_format_log_locked(int (*putc)(const char c), const char *fmt, ...);

/**********************************************************************************
 * 函数名称: pl_syslog_put_chars
 * 
 * 描述:
 *   使用putc功能输出字符序列。
 *
 * 参数:
 *   @putc: 字符输出函数指针。
 *   @start: 开始字符。
 *   @end: 结束字符。
 * 
 * 返回值:
 *   无。
 ***********************************************************************************/
void pl_syslog_put_chars(int (*putc)(const char c), const char *start, const char *end);

/**********************************************************************************
 * 函数名称: pl_early_syslog
 * 
 * 描述:
 *   早期系统日志记录（在PlainOS尚未启动时）。
 *
 * 输出格式:
 *   [$(格式字符串内容)]
 *
 * 参数:
 *  @fmt: 格式化字符串。
 *  @...: 可变参数列表。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
#define pl_early_syslog(fmt, ...)  \
	pl_put_format_log_locked(pl_port_putc, fmt, ## __VA_ARGS__)

/**********************************************************************************
 * 函数名称: pl_early_syslog_info
 * 
 * 描述:
 *   早期系统日志信息记录（PlainOS未启动时）。
 *
 * 输出格式:
 *   [info][$(格式字符串内容)]
 *
 * 参数:
 *  @fmt: 格式化字符串。
 *  @...: 可变参数列表。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
#define pl_early_syslog_info(fmt, ...)  \
	pl_put_format_log_locked(pl_port_putc, "[info] "fmt, ## __VA_ARGS__)

/**********************************************************************************
 * 函数名称: pl_early_syslog_warn
 * 
 * 描述:
 *   早期警告系统日志记录（PlainOS未启动时）。
 *
 * 输出格式:
 *   [warn][$(格式字符串内容)]
 *
 * 参数:
 *  @fmt: 格式化字符串。
 *  @...: 可变参数列表。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
#define pl_early_syslog_warn(fmt, ...)  \
	pl_put_format_log_locked(pl_port_putc, PL_EARLY_SYSLOG_WARN_ANSI_COLOR"[warn] "fmt \
	PL_EARLY_SYSLOG_ANSI_COLOR_RESET, ## __VA_ARGS__)

/**********************************************************************************
 * 函数名称: pl_early_syslog_err
 * 
 * 描述:
 *   早期错误系统日志记录（PlainOS未启动时）。
 *
 * 输出格式:
 *   [erro][$(格式字符串内容)]
 *
 * 参数:
 *  @fmt: 格式化字符串。
 *  @...: 可变参数列表。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
#define pl_early_syslog_err(fmt, ...)  \
	pl_put_format_log_locked(pl_port_putc, PL_EARLY_SYSLOG_ERR_ANSI_COLOR"[erro] "fmt \
	PL_EARLY_SYSLOG_ANSI_COLOR_RESET, ## __VA_ARGS__)

/**********************************************************************************
 * 函数名称: pl_put_format_log
 * 
 * 描述:
 *   格式化输出日志（PlainOS已启动后使用）。
 *
 * 参数:
 *  @fmt: 格式化字符串。
 *  @...: 可变参数列表。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
void pl_put_format_log(const char *fmt, ...);

/**********************************************************************************
 * 函数名称: pl_syslog_redirect
 * 
 * 描述:
 *   系统日志重定向到用户提供的字符输出函数。
 *
 * 参数:
 *  @put_char: 用户自定义的字符输出函数指针。
 *
 * 返回值:
 *  成功时返回大于等于0的值，失败时返回小于0的值。
 ***********************************************************************************/
int pl_syslog_redirect(int (*put_char)(char c));

/**********************************************************************************
 * 函数名称: pl_syslog
 * 
 * 描述:
 *   系统日志记录（PlainOS已经启动后使用）。
 *
 * 输出格式:
 *   [$(格式字符串内容)]
 *
 * 参数:
 *  @fmt: 格式化字符串。
 *  @...: 可变参数列表。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
#define pl_syslog(fmt, ...)   \
	pl_put_format_log(fmt, ## __VA_ARGS__)

/**********************************************************************************
 * 函数名称: pl_syslog_info
 * 
 * 描述:
 *   信息级别系统日志记录（PlainOS已启动）。
 *
 * 输出格式:
 *   [info][$(格式字符串内容)]
 *
 * 参数:
 *  @fmt: 格式化字符串。
 *  @...: 可变参数列表。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
#define pl_syslog_info(fmt, ...)   \
	pl_put_format_log("[info] "fmt, ## __VA_ARGS__)

/**********************************************************************************
 * 函数名称: pl_syslog_warn
 * 
 * 描述:
 *   警告级别系统日志记录（PlainOS已启动）。
 *
 * 输出格式:
 *   [warn][$(格式字符串内容)]
 *
 * 参数:
 *  @fmt: 格式化字符串。
 *  @...: 可变参数列表。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
#define pl_syslog_warn(fmt, ...)   \
	pl_put_format_log(PL_SYSLOG_WARN_ANSI_COLOR"[warn] "fmt \
	PL_SYSLOG_ANSI_COLOR_RESET, ## __VA_ARGS__)

/**********************************************************************************
 * 函数名称: pl_syslog_err
 * 
 * 描述:
 *   错误级别系统日志记录（PlainOS已启动）。
 *
 * 输出格式:
 *   [erro][$(格式字符串内容)]
 *
 * 参数:
 *  @fmt: 格式化字符串。
 *  @...: 可变参数列表。
 *
 * 返回值:
 *   无。
 ***********************************************************************************/
#define pl_syslog_err(fmt, ...)   \
	pl_put_format_log(PL_SYSLOG_ERR_ANSI_COLOR"[erro] "fmt \
	PL_SYSLOG_ANSI_COLOR_RESET, ## __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_SYSLOG_H__ */
