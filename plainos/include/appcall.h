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
#ifndef __PLAINOS_APPCALL_H__
#define __PLAINOS_APPCALL_H__

#include <kernel/kernel.h>

typedef int (*pl_app_call_t)(int argc, char *argv[]);

struct pl_app_entry {
	pl_app_call_t entry;
	const char *name;
};

#define __define_appcall(fn, _name) static struct pl_app_entry \
	__appcall_##fn pl_used \
	__attribute__((section(".pl_appcall.app"))) = { \
		.entry = fn, \
		.name = (_name), \
	}

#define pl_app_register(app, _name)        __define_appcall(app, _name)

#endif /* __PLAINOS_APPCALL_H__ */
