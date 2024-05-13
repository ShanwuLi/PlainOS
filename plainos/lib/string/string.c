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

#include <types.h>
#include <errno.h>
#include <lib/string.h>

int pl_lib_ull2str(char *str, unsigned long long n, unsigned char base)
{
	int ret;
	int val;
	int i = 1;
	unsigned long long num = n;

	if (base > 16)
		return -ERANGE;

	while(num / base) {
		num /= base;
		i++;
	}

	ret = i;
	for(; i > 0; i--) {
		val = (n % base);
		str[i - 1] = (val > 9) ? (val - 10 + 'a') : (val + '0');
		n /= base;
	}

	str[ret] = '\0';
	return ret;
}

int pl_lib_ll2str(char *str, long long n, unsigned char base)
{
	int ret;
	int i = 0;

	if (n < 0) {
		i = 1;
		str[0] = '-';
		n = (~(unsigned long long)n) + 1;
	}

	ret = pl_lib_ull2str(&str[i], n, base);
	return ret;
}

void *pl_memmove(void *dest, const void *src, size_t len)
{
	char *d;
	const char *s;
	const char *lasts;
	char *lastd;

	if (dest == NULL || src == NULL || len == 0)
		return NULL;

	d = dest;
	s = src;
	if (d < s) {
		while (len--)
			*d++ = *s++;
	} else {
		lasts = s + (len - 1);
		lastd = d + (len - 1);
		while (len--)
			*lastd-- = *lasts--;
	}

	return dest;
}
