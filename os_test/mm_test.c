#include "../include/errno.h"
#include <stdio.h>


static int put_format_log(const char *fmt, char **log)
{
	int ret;
	int state = 0;
	const char *ch = fmt;

	for (; *fmt != '\0' && log != 0; fmt++) {

		state += *fmt;
		switch (state) {
		case '%':
		case '%' + 'l':
		case '%' + 'l' + 'l':
		continue;

		case '%' + 's':
			printf("%s", *(log++));
			break;

		case '%' + 'd':
			printf("%d", *(log++));
			break;

		case '%' + 'x':
			printf("%x", *(log++));
			break;

		case '%' + 'b':
			printf("b:%x", *(log++));
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
		
		case '%' + 'l' + 'b':
			break;

		case '%' + 'l' + 'u':
			printf("%lu", *(log++));
			break;

		default:
			ret = printf("%c", *ch);
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

/* avoid to dangling pointer */
#define va(x)     (void *)(x)
#define LOG(...)  ((char **)({char *temp[] = { __VA_ARGS__ , NULL}; (uintptr_t)temp;}))

int main(void)
{
	put_format_log("%s, 0x%x, %x, dehdekfenv\n", LOG("this is a test", va(0x90), "defef"));
	put_format_log("%s, %u, %s, dehdekfenv\n", LOG("this is a test", va(0x90), "defef"));
	put_format_log("%s, %d, %s, dehdekfenv\n", LOG(va("this is a test"), va(0x90), va("defef")));
	return 0;
}
