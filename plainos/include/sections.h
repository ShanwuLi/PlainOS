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
#ifndef __PLAINOS_SECTIONS_H__
#define __PLAINOS_SECTIONS_H__

#include "errno.h"

#define CONST_SECTION               *(pl_const)

#define INIT_CALLS_LEVEL(level)     __initcall##level##_start = .; \
	                                KEEP(*(.initcall##level##.init)) \
	                                KEEP(*(.initcall##level##s.init))

#define APP_CALLS_LEVEL(level)      __appcall##level##_start = .; \
	                                KEEP(*(.appcall##level##.app)) \
	                                KEEP(*(.appcall##level##s.app))

#define PL_INIT_SECTION             *(.pl_init.init) /* __init */

#define INIT_CALLS_SECTION          __initcall_start = .; \
	                                __early_initcall_start = .; \
	                                KEEP(*(.initcallearly.init)) \
	                                __early_initcall_end = .; \
	                                INIT_CALLS_LEVEL(0) \
	                                INIT_CALLS_LEVEL(1) \
	                                INIT_CALLS_LEVEL(2) \
	                                INIT_CALLS_LEVEL(3) \
	                                INIT_CALLS_LEVEL(4) \
	                                INIT_CALLS_LEVEL(5) \
	                                INIT_CALLS_LEVEL(6) \
	                                INIT_CALLS_LEVEL(7) \
	                                INIT_CALLS_LEVEL(8) \
	                                INIT_CALLS_LEVEL(9) \
	                                __initcall_end = .;

#define APPS_CALLS_SECTION          __appcall_start = .; \
	                                APP_CALLS_LEVEL(0) \
	                                APP_CALLS_LEVEL(1) \
	                                APP_CALLS_LEVEL(2) \
	                                APP_CALLS_LEVEL(3) \
	                                APP_CALLS_LEVEL(4) \
	                                APP_CALLS_LEVEL(5) \
	                                __appcall_end = .;

#endif /* __PLAINOS_SECTIONS_H__ */