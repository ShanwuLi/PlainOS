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
#ifndef __KERNEL_SECTIONS_H__
#define __KERNEL_SECTIONS_H__

#include "../errno.h"

#define CONST_SECTION               *(__const)

#define INIT_CALLS_LEVEL(level)     __initcall##level##_start = .; \
	                                KEEP(*(.initcall##level##.init)) \
	                                KEEP(*(.initcall##level##s.init))

#define INIT_CALLS_SECTION          __initcall_start = .; \
	                                KEEP(*(.initcallearly.init)) \
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
	                                KEEP(*(.appcall0.app)) \
	                                KEEP(*(.appcall1.app)) \
	                                KEEP(*(.appcall2.app)) \
	                                KEEP(*(.appcall3.app)) \
	                                KEEP(*(.appcall4.app)) \
	                                KEEP(*(.appcall5.app)) \
	                                __appcall_end = .;

#define ERRNO_SECTION               __errno_start = .;  \
	                                LONG(OK  ) \
	                                LONG(-EPERM  ) \
	                                LONG(-ENOENT ) \
	                                LONG(-ESRCH  ) \
	                                LONG(-EINTR  ) \
	                                LONG(-EIO    ) \
	                                LONG(-ENXIO  ) \
	                                LONG(-E2BIG  ) \
	                                LONG(-ENOEXEC) \
	                                LONG(-EBADF  ) \
	                                LONG(-ECHILD ) \
	                                LONG(-EAGAIN ) \
	                                LONG(-ENOMEM ) \
	                                LONG(-EACCES ) \
	                                LONG(-EFAULT ) \
	                                LONG(-ENOTBLK) \
	                                LONG(-EBUSY  ) \
	                                LONG(-EEXIST ) \
	                                LONG(-EXDEV  ) \
	                                LONG(-ENODEV ) \
	                                LONG(-ENOTDIR) \
	                                LONG(-EISDIR ) \
	                                LONG(-EINVAL ) \
	                                LONG(-ENFILE ) \
	                                LONG(-EMFILE ) \
	                                LONG(-ENOTTY ) \
	                                LONG(-ETXTBSY) \
	                                LONG(-EFBIG  ) \
	                                LONG(-ENOSPC ) \
	                                LONG(-ESPIPE ) \
	                                LONG(-EROFS  ) \
	                                LONG(-EMLINK ) \
	                                LONG(-EPIPE  ) \
	                                LONG(-EDOM   ) \
	                                LONG(-ERANGE ) \
	                                LONG(-EREGED ) \
	                                LONG(-ETIMEOUT) \
	                                LONG(ERROR  ) \
	                                __errno_end = .;

#endif /* __KERNEL_SECTIONS_H__ */
