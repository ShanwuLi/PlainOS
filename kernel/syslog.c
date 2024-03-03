
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
 * Function Name: pl_put_format_log
 *
 * Description:
 *   put format log using putc. It will put [front][fmt][rear] string using putc.
 *
 * Param:
 *   @putc: putc function.
 *   @front: the front of fmt.
 *   @rear: the rear of fmt .
 *   @fmt: format string.
 * 
 * Return:
 *   none.
 ************************************************************************************/
void pl_put_format_log(int (*putc)(const char c), const char *front,
                             const char *rear, const char *fmt, ...)
{
	char str[24];
	int state = 0;
	va_list valist;
	const char *ch = fmt;

	put_string(putc, front);
	va_start(valist, fmt);
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
			pl_ll2str(str, va_arg(valist, int_t), 10);
			put_string(putc, str);
			break;

		case '%' + 'x':
			pl_ull2str(str, va_arg(valist, uint_t), 16);
			put_string(putc, str);
			break;

		case '%' + 'u':
			pl_ull2str(str, va_arg(valist, uint_t), 10);
			put_string(putc, str);
			break;

		case '%' + 'l' + 'd':
			pl_ll2str(str, va_arg(valist, l_t), 10);
			put_string(putc, str);
			break;

		case '%' + 'l' + 'x':
			pl_ull2str(str, va_arg(valist, ul_t), 16);
			put_string(putc, str);
			break;

		case '%' + 'l' + 'u':
			pl_ull2str(str, va_arg(valist, ul_t), 10);
			put_string(putc, str);
			break;

		case '%' + 'l' + 'l' + 'd':
			pl_ll2str(str, va_arg(valist, ll_t), 10);
			put_string(putc, str);
			break;

		case '%' + 'l' + 'l' + 'x':
			pl_ull2str(str, va_arg(valist, ull_t), 16);
			put_string(putc, str);
			break;

		case '%' + 'l' + 'l' + 'u':
			pl_ull2str(str, va_arg(valist, ull_t), 10);
			put_string(putc, str);
			break;

		default:
			put_chars(putc, ch, fmt);
			break;
		}

		state = 0;
		ch = fmt + 1;
	}

	va_end(valist);
	put_string(putc, rear);
}
