
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
#include <types.h>
#include <lib/string.h>
#include <kernel/syslog.h>
#include <kernel/semaphore.h>
#include "syslog.h"
#include "semaphore.h"

static struct semaphore syslog_semaphore;

/*************************************************************************************
 * Function Name: put_string
 *
 * Description:
 *   put a string using puc.
 *
 * Param:
 *   @putc: putc function.
 *   @str: string.
 * 
 * Return:
 *   none.
 ************************************************************************************/
static void put_string(int (*putc)(const char c), const char *str)
{
	while (*str)
		putc(*(str++));
}

/*************************************************************************************
 * Function Name: put_chars
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
static void put_chars(int (*putc)(const char c),
             const char *start, const char *end)
{
	const char *ch;
 
	for (ch = start; ch <= end; ch++)
		putc(*ch);
}

/*************************************************************************************
 * Function Name: pl_vformat_log
 *
 * Description:
 *   put chars using puc with va_list.
 *
 * Param:
 *   @putc: putc function.
 *   @fmt: format string.
 *   @valist: variable arguments list.
 * 
 * Return:
 *   none.
 ************************************************************************************/
void pl_vformat_log(int (*putc)(const char c), const char *fmt, va_list valist)
{
	char str[24];
	int state = 0;
	const char *ch = fmt;

	for (; *fmt != '\0'; fmt++) {
		state += *fmt;
		switch (state) {
		case '%':
		case '%' + 'l':
		case '%' + 'l' + 'l':
		continue;

		case '%' + 's':
			put_string(putc, va_arg(valist, char *));
			break;

		case '%' + 'd':
			pl_lib_ll2str(str, va_arg(valist, int_t), 10);
			put_string(putc, str);
			break;

		case '%' + 'x':
			pl_lib_ull2str(str, va_arg(valist, uint_t), 16);
			put_string(putc, str);
			break;

		case '%' + 'u':
			pl_lib_ull2str(str, va_arg(valist, uint_t), 10);
			put_string(putc, str);
			break;

		case '%' + 'l' + 'd':
			pl_lib_ll2str(str, va_arg(valist, l_t), 10);
			put_string(putc, str);
			break;

		case '%' + 'l' + 'x':
			pl_lib_ull2str(str, va_arg(valist, ul_t), 16);
			put_string(putc, str);
			break;

		case '%' + 'l' + 'u':
			pl_lib_ull2str(str, va_arg(valist, ul_t), 10);
			put_string(putc, str);
			break;

		case '%' + 'l' + 'l' + 'd':
			pl_lib_ll2str(str, va_arg(valist, ll_t), 10);
			put_string(putc, str);
			break;

		case '%' + 'l' + 'l' + 'x':
			pl_lib_ull2str(str, va_arg(valist, ull_t), 16);
			put_string(putc, str);
			break;

		case '%' + 'l' + 'l' + 'u':
			pl_lib_ull2str(str, va_arg(valist, ull_t), 10);
			put_string(putc, str);
			break;

		default:
			put_chars(putc, ch, fmt);
			break;
		}

		state = 0;
		ch = fmt + 1;
	}
}

/*************************************************************************************
 * Function Name: pl_put_format_log_locked
 *
 * Description:
 *   put format log using putc. It will put [front][fmt][rear] string using putc.
 *
 * Param:
 *   @putc: putc function.
 *   @fmt: format string.
 * 
 * Return:
 *   none.
 ************************************************************************************/
void pl_put_format_log_locked(int (*putc)(const char c), const char *fmt, ...)
{
	va_list valist;

	va_start(valist, fmt);
	pl_vformat_log(putc, fmt, valist);
	va_end(valist);
}

/*************************************************************************************
 * Function Name: pl_syslog_init
 *
 * Description:
 *   syslog initialization.
 * 
 * Parameters:
 *  none.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_syslog_init(void)
{
	pl_semaplore_init(&syslog_semaphore, 1);
	pl_early_syslog("syslog init successfully\r\n");
	return OK;
}

/*************************************************************************************
 * Function Name: pl_put_format_log
 *
 * Description:
 *   put format log (PlainOS has started).
 *
 * Parameters:
 *  @putc: function of putting char.
 *  @fmt: format string.
 *  @...: variable parameters.
 *
 * Return:
 *  void.
 ************************************************************************************/
void pl_put_format_log(int (*putc)(const char c), const char *fmt, ...)
{
	va_list valist;

	pl_semaplore_take(&syslog_semaphore);
	va_start(valist, fmt);
	pl_vformat_log(putc, fmt, valist);
	va_end(valist);
	pl_semaplore_give(&syslog_semaphore);
}
