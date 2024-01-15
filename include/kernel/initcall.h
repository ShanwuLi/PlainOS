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
#ifndef __KERNEL_INITCALL_H__
#define __KERNEL_INITCALL_H__

#include <kernel/kernel.h>

typedef int                          (*initcall_t)(void);
typedef int                          (*exitcall_t)(void);

#define __define_initcall(fn, id)    static initcall_t __initcall_##fn##id __used \
                                      __attribute__((section(".initcall" #id ".init"))) = fn

#define early_initcall(fn)            __define_initcall(fn, early)
#define pure_initcall(fn)             __define_initcall(fn, 0)
#define pure_initcall_sync(fn)        __define_initcall(fn, 0s)
#define core_initcall(fn)             __define_initcall(fn, 1)
#define core_initcall_sync(fn)        __define_initcall(fn, 1s)
#define postcore_initcall(fn)         __define_initcall(fn, 2)
#define postcore_initcall_sync(fn)    __define_initcall(fn, 2s)
#define arch_initcall(fn)             __define_initcall(fn, 3)
#define arch_initcall_sync(fn)        __define_initcall(fn, 3s)
#define subsys_initcall(fn)           __define_initcall(fn, 4)
#define subsys_initcall_sync(fn)      __define_initcall(fn, 4s)
#define fs_initcall(fn)               __define_initcall(fn, 5)
#define fs_initcall_sync(fn)          __define_initcall(fn, 5s)
#define bsp_initcall(fn)              __define_initcall(fn, 6)
#define bsp_initcall_sync(fn)         __define_initcall(fn, 6s)
#define driver_initcall(fn)           __define_initcall(fn, 7)
#define driver_initcall_sync(fn)      __define_initcall(fn, 7s)
#define device_initcall(fn)           __define_initcall(fn, 8)
#define device_initcall_sync(fn)      __define_initcall(fn, 8s)
#define late_initcall(fn)             __define_initcall(fn, 9)
#define late_initcall_sync(fn)        __define_initcall(fn, 9s)

#endif /* __KERNEL_INITCALL_H__ */
