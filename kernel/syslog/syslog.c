
#include <kernel/syslog.h>
#include <pl_cfg.h>
#include "../common/ansi_color.h"
#include <pl_port.h>
#include <errno.h>
#include <lib/pl_string.h>

static void put_string(int (*putc)(const char c), char *str)
{
	int ret;

	while (*str) {
		putc(*str);
		str++;
	}

	return OK;
}

static void put_chars(int (*putc)(const char c),
             const char *start, const char *end)
{
	int ret;
	const char *ch;
 
	for (ch = start; ch <= end; ch++)
		putc(*ch);

	return OK;
}

static int put_format_log(int (*putc)(const char c),
                        const char *fmt, char **log)
{
	int ret;
	int state = 0;
	const char *ch = fmt;
	char str[24];

	for (; *fmt != '\0' && log != 0; fmt++) {
		state += *fmt;
		switch (state) {
		case '%':
		case '%' + 'l':
		case '%' + 'l' + 'l':
		continue;

		case '%' + 's':
			put_string(putc, *(log++));
			break;

		case '%' + 'd':
			ull2str(str, (*(log++)));
			put_string(putc, str);
			break;

		case '%' + 'x':
			printf("%x", *(log++));
			break;

		case '%' + 'u':
			printf("%u", *(log++));
			break;

		case '%' + 'l' + 'd':
			printf("%ld", *(log++));
			break;

		case '%' + 'l' + 'x':
			printf("%lx", *(log++));
			break;

		case '%' + 'l' + 'u':
			printf("%lu", *(log++));
			break;

		default:
			ret = put_chars(ch, fmt);
			if (ret < 0) {
				printf("putc error");
				return -EFAULT;
			}
			break;
		}

		state = 0;
		ch = fmt + 1;
	}

	return 0;
}

int main(void)
{
	put_format_log("%s, 0x%x, %x, dehdekfenv\n", LOG("this is a test", va(0x90), "defef"));
	put_format_log("%s, %u, %s, dehdekfenv\n", LOG("this is a test", va(0x90), "defef"));
	put_format_log("%s, %d, %s, dehdekfenv\n", LOG(va("this is a test"), va(0x90), va("defef")));
	put_format_log("%s, 0x%, %x, deh%uekfenv\n", LOG("this is a test", va(0x90), "defef"));
	put_format_log("%s, %lu, %s, dehdekfenv\n", LOG("this is a test", va(0x90), "defef"));
	put_format_log("%s, %d, %s, dehde%kfenv\n", LOG(va("this is a test"), va(0x90), va("defef")));



	printf("%s, %d\n", "%%", '%');
	printf("%s, %d\n", "%d", '%' + 'd');
	printf("%s, %d\n", "%x", '%' + 'x');
	printf("%s, %d\n", "%b", '%' + 'b');
	printf("%s, %d\n", "%s", '%' + 's');
	printf("%s, %d\n", "%u", '%' + 'u');

	printf("%s, %d\n", "%ld", '%' + 'l' + 'd');
	printf("%s, %d\n", "%lx", '%' + 'l' + 'x');
	printf("%s, %d\n", "%lb", '%' + 'l' + 'b');
	printf("%s, %d\n", "%lu", '%' + 'l' + 'u');

	printf("%s, %d\n", "%lld", '%' + 'l' + 'l' + 'd');
	printf("%s, %d\n", "%llx", '%' + 'l' + 'l' + 'x');
	printf("%s, %d\n", "%llb", '%' + 'l' + 'l' + 'b');
	printf("%s, %d\n", "%llu", '%' + 'l' + 'l' + 'u');

	return 0;
}



#if 0
int pl_early_syslog_info(char *fmt, char **log)
{
	return OK;
}

int pl_early_syslog_warn(char *fmt, char **log)
{
#ifdef PL_CFG_SYSLOG_ANSI_COLOR
	
#endif /* PL_CFG_SYSLOG_ANSI_COLOR */

	return OK;
}

int pl_early_syslog_err(char *fmt, char **log)
{
#ifdef PL_CFG_SYSLOG_ANSI_COLOR

#endif /* PL_CFG_SYSLOG_ANSI_COLOR */
	return OK;
}

#endif

