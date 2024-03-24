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

#ifndef __KERNEL_SOFTTIMER_H__
#define __KERNEL_SOFTTIMER_H__

#include <kernel/kernel.h>

typedef void *stimer_handle_t;
typedef void (*stimer_fun_t)(stimer_handle_t timer);

stimer_handle_t pl_softtimer_request(void);
int pl_softtimer_set_private_data(stimer_handle_t timer, void *data);
int pl_softtimer_get_private_data(stimer_handle_t timer, void **data);
int pl_softtimer_start(stimer_handle_t timer, stimer_fun_t fun,
                       struct count *timing_cnt, void *priv_data);
#endif /* __KERNEL_SOFTTIMER_H__ */
