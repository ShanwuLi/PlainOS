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

/*************************************************************************************
 * Function Name: pl_softtimer_request
 *
 * Description:
 *   request a soft timer.
 * 
 * Parameters:
 *  @name: timer name.
 *
 * Return:
 *  @stimer_handle_t: handle of soft timer requested.
 ************************************************************************************/
stimer_handle_t pl_softtimer_request(const char *name);

/*************************************************************************************
 * Function Name: pl_softtimer_get_private_data
 *
 * Description:
 *   get private data of soft timer.
 * 
 * Parameters:
 *  @timer: handle of soft timer requested.
 *  @data: private data addr.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_softtimer_get_private_data(stimer_handle_t timer, void **data);

/*************************************************************************************
 * Function Name: pl_softtimer_start
 *
 * Description:
 *   start soft timer.
 * 
 * Parameters:
 *  @timer: handle of soft timer requested.
 *  @fun: callback function.
 *  @timing_cnt: the count of timing.
 *  @priv_data: private data.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_softtimer_start(stimer_handle_t timer, stimer_fun_t fun,
                       struct count *timing_cnt, void *priv_data);

/*************************************************************************************
 * Function Name: pl_softtimer_cancel
 *
 * Description:
 *   cancel soft timer.
 * 
 * Parameters:
 *  @timer: handle of soft timer requested.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_softtimer_cancel(stimer_handle_t timer);

/*************************************************************************************
 * Function Name: pl_softtimer_cancel
 *
 * Description:
 *   cancel soft timer.
 * 
 * Parameters:
 *  @timer: handle of soft timer requested.
 *
 * Return:
 *  void.
 ************************************************************************************/
void pl_softtimer_release(stimer_handle_t timer);

#endif /* __KERNEL_SOFTTIMER_H__ */
