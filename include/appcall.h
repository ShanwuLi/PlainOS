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
#ifndef __KERNEL_APPCALL_H__
#define __KERNEL_APPCALL_H__

typedef int (*plainos_app_call_t)(int argc, char *argv[]);

struct plainos_app_call {
	plainos_app_call_t app;
	int argc;
	char **argv;
};

#define __define_appcall(fn, argc, argv, id)  static struct plainos_app_call \
	__appcall_##fn##id __used \
	__attribute__((section(".appcall" #id ".app"))) = \
	{ \
		.app = fn, \
		.argc = argc, \
		.argv = argv, \
	}

#define system_app_register(app, argc, argv)      __define_appcall(app, argc, argv, 0)
#define dfx_app_register(app, argc, argv)         __define_appcall(app, argc, argv, 1)
#define excep_app_register(app, argc, argv)       __define_appcall(app, argc, argv, 2)
#define perf_app_register(app, argc, argv)        __define_appcall(app, argc, argv, 3)
#define test_app_register(app, argc, argv)        __define_appcall(app, argc, argv, 4)
#define user_app_register(app, argc, argv)        __define_appcall(app, argc, argv, 5)

#endif /* __KERNEL_APPCALL_H__ */
