
#include <kernel/syslog.h>
#include <errno.h>
#include <lib/pl_string.h>
#include <types.h>

static void put_string(int (*putc)(const char c), char *str)
{
	while (*str)
		putc(*(str++));
}

static void put_chars(int (*putc)(const char c),
             const char *start, const char *end)
{
	const char *ch;
 
	for (ch = start; ch <= end; ch++)
		putc(*ch);
}

void pl_put_format_log(int (*putc)(const char c), const char *fmt, ...)
{
	int ret;
	char str[24];
	int state = 0;
	va_list valist;
	va_start(valist, fmt);
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
			ll2str(str, va_arg(valist, int_t), 10);
			put_string(putc, str);
			break;

		case '%' + 'x':
			ull2str(str, va_arg(valist, uint_t), 16);
			put_string(putc, str);
			break;

		case '%' + 'u':
			ull2str(str, va_arg(valist, uint_t), 10);
			put_string(putc, str);
			break;

		case '%' + 'l' + 'd':
			ll2str(str, va_arg(valist, l_t), 10);
			put_string(putc, str);
			break;

		case '%' + 'l' + 'x':
			ull2str(str, va_arg(valist, ul_t), 16);
			put_string(putc, str);
			break;

		case '%' + 'l' + 'u':
			ull2str(str, va_arg(valist, ul_t), 10);
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
}
