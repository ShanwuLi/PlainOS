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

/*************************************************************************************
 * Function Name: pl_put_format_log_locked
 *
 * Description:
 *   format string output (PlainOS has not start).
 *
 * put format:
 *   [$(front)][$(fmt)][$(rear)]
 *
 * Parameters:
 *  @rear: rear string of fmt.
 *  @fmt: format string.
 *
 * Return:
 *  void.
 ************************************************************************************/
void pl_put_format_log_locked(int (*putc)(const char c), const char *fmt, ...);

/*************************************************************************************
 * Function Name: pl_syslog_put_chars
 *
 * Description:
 *   put chars using puc.
 *
 * Param:
 *   @putc: putc function.
 *   @start: start char.
 *   @end: end char.
 * 
 * Return:
 *   none.
 ************************************************************************************/
void pl_syslog_put_chars(int (*putc)(const char c), const char *start, const char *end);

/*************************************************************************************
 * Function Name: pl_early_syslog
 *
 * Description:
 *   early syslog (PlainOS has not start).
 *
 * put format:
 *   [$(fmt)]
 *
 * Parameters:
 *  @fmt: format string.
 *  @...: variable parameters.
 *
 * Return:
 *  void.
 ************************************************************************************/
#define pl_early_syslog(fmt, ...)  \
	pl_put_format_log_locked(pl_port_putc, fmt, ## __VA_ARGS__)

/*************************************************************************************
 * Function Name: pl_early_syslog_info
 *
 * Description:
 *   early syslog_info (PlainOS has not start).
 *
 * put format:
 *   [info][$(fmt)]
 *
 * Parameters:
 *  @fmt: format string.
 *  @...: variable parameters.
 *
 * Return:
 *  void.
 ************************************************************************************/
#define pl_early_syslog_info(fmt, ...)  \
	pl_put_format_log_locked(pl_port_putc, "[info] "fmt, ## __VA_ARGS__)

/*************************************************************************************
 * Function Name: pl_early_syslog_warn
 *
 * Description:
 *   early syslog_warn (PlainOS has not start).
 *
 * put format:
 *   [warn][$(fmt)]
 *
 * Parameters:
 *  @fmt: format string.
 *  @...: variable parameters.
 *
 * Return:
 *  void.
 ************************************************************************************/
#define pl_early_syslog_warn(fmt, ...)  \
	pl_put_format_log_locked(pl_port_putc, PL_EARLY_SYSLOG_WARN_ANSI_COLOR"[warn] "fmt \
	PL_EARLY_SYSLOG_ANSI_COLOR_RESET, ## __VA_ARGS__)

/*************************************************************************************
 * Function Name: pl_early_syslog_err
 *
 * Description:
 *   early syslog_err (PlainOS has not start).
 *
 * put format:
 *   [erro][$(fmt)]
 *
 * Parameters:
 *  @fmt: format string.
 *  @...: variable parameters.
 *
 * Return:
 *  void.
 ************************************************************************************/
#define pl_early_syslog_err(fmt, ...)  \
	pl_put_format_log_locked(pl_port_putc, PL_EARLY_SYSLOG_ERR_ANSI_COLOR"[erro] "fmt \
	PL_EARLY_SYSLOG_ANSI_COLOR_RESET, ## __VA_ARGS__)

/*************************************************************************************
 * Function Name: pl_put_format_log
 *
 * Description:
 *   put format log (PlainOS has started).
 *
 * Parameters:
 *  @fmt: format string.
 *  @...: variable parameters.
 *
 * Return:
 *  void.
 ************************************************************************************/
void pl_put_format_log(const char *fmt, ...);

/*************************************************************************************
 * Function Name: pl_syslog_redirect
 *
 * Description:
 *   syslog redirect to put_char of user provided.
 * 
 * Parameters:
 *  @put_char: redirect to put_char.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_syslog_redirect(int (*put_char)(char c));

/*************************************************************************************
 * Function Name: pl_syslog
 *
 * Description:
 *   syslog (PlainOS has started).
 *
 * put format:
 *   [$(fmt)]
 *
 * Parameters:
 *  @fmt: format string.
 *  @...: variable parameters.
 *
 * Return:
 *  void.
 ************************************************************************************/
#define pl_syslog(fmt, ...)   \
	pl_put_format_log(fmt, ## __VA_ARGS__)

/*************************************************************************************
 * Function Name: pl_syslog_info
 *
 * Description:
 *   syslog_info (PlainOS has started).
 *
 * put format:
 *   [info][$(fmt)]
 *
 * Parameters:
 *  @fmt: format string.
 *  @...: variable parameters.
 *
 * Return:
 *  void.
 ************************************************************************************/
#define pl_syslog_info(fmt, ...)   \
	pl_put_format_log("[info] "fmt, ## __VA_ARGS__)

/*************************************************************************************
 * Function Name: pl_syslog_warn
 *
 * Description:
 *   syslog_warn (PlainOS has started).
 *
 * put format:
 *   [warn][$(fmt)]
 *
 * Parameters:
 *  @fmt: format string.
 *  @...: variable parameters.
 *
 * Return:
 *  void.
 ************************************************************************************/
#define pl_syslog_warn(fmt, ...)   \
	pl_put_format_log(PL_SYSLOG_WARN_ANSI_COLOR"[warn] "fmt \
	PL_SYSLOG_ANSI_COLOR_RESET, ## __VA_ARGS__)

/*************************************************************************************
 * Function Name: pl_syslog_err
 *
 * Description:
 *   syslog_err (PlainOS has started).
 *
 * put format:
 *   [erro][$(fmt)]
 *
 * Parameters:
 *  @fmt: format string.
 *  @...: variable parameters.
 *
 * Return:
 *  void.
 ************************************************************************************/
#define pl_syslog_err(fmt, ...)   \
	pl_put_format_log(PL_SYSLOG_ERR_ANSI_COLOR"[erro] "fmt \
	PL_SYSLOG_ANSI_COLOR_RESET, ## __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_SYSLOG_H__ */
