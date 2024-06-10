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

typedef int                             (*initcall_t)(void);
typedef int                             (*exitcall_t)(void);

#define __define_initcall(fn, id)        static initcall_t __initcall_##fn##id pl_used \
                                         __attribute__((section(".pl_initcall" #id ".init"))) = fn

#define __init                           __attribute__((section(".pl_init.init")))



/* system setup before */
#define pl_early_initcall(fn)            __define_initcall(fn, early)

/* system setup doing */
#define pl_pure_initcall(fn)             __define_initcall(fn, 0)
#define pl_pure_initcall_sync(fn)        __define_initcall(fn, 0s)

/* system core components setup doing */
#define pl_core_initcall(fn)             __define_initcall(fn, 1)
#define pl_core_initcall_sync(fn)        __define_initcall(fn, 1s)

/* system core components setup done */
#define pl_postcore_initcall(fn)         __define_initcall(fn, 2)
#define pl_postcore_initcall_sync(fn)    __define_initcall(fn, 2s)
#define pl_arch_initcall(fn)             __define_initcall(fn, 3)
#define pl_arch_initcall_sync(fn)        __define_initcall(fn, 3s)

/* subsystems setup */
#define pl_subsys_initcall(fn)           __define_initcall(fn, 4)
#define pl_subsys_initcall_sync(fn)      __define_initcall(fn, 4s)
#define pl_fs_initcall(fn)               __define_initcall(fn, 5)
#define pl_fs_initcall_sync(fn)          __define_initcall(fn, 5s)
#define pl_bsp_initcall(fn)              __define_initcall(fn, 6)
#define pl_bsp_initcall_sync(fn)         __define_initcall(fn, 6s)
#define pl_driver_initcall(fn)           __define_initcall(fn, 7)
#define pl_driver_initcall_sync(fn)      __define_initcall(fn, 7s)
#define pl_device_initcall(fn)           __define_initcall(fn, 8)
#define pl_device_initcall_sync(fn)      __define_initcall(fn, 8s)
#define pl_late_initcall(fn)             __define_initcall(fn, 9)
#define pl_late_initcall_sync(fn)        __define_initcall(fn, 9s)

#endif /* __KERNEL_INITCALL_H__ */
