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

#ifndef __KERNEL_SEMAPHORE_H__
#define __KERNEL_SEMAPHORE_H__

#include <types.h>

typedef void *pl_semaphore_handle_t;

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************
 * Function Name: pl_semaplore_request
 *
 * Description:
 *   request a semaphore.
 * 
 * Parameters:
 *  @val: value of semaphore.
 *
 * Return:
 *  @semaphore_handle.
 ************************************************************************************/
pl_semaphore_handle_t pl_semaplore_request(int val);

/*************************************************************************************
 * Function Name: pl_semaplore_take
 *
 * Description:
 *    take semaphore.
 * 
 * Parameters:
 *  @semap: semaphore handle.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_semaplore_take(pl_semaphore_handle_t semap);

/*************************************************************************************
 * Function Name: pl_semaplore_give
 *
 * Description:
 *    give semaphore.
 * 
 * Parameters:
 *  @semap: semaphore handle.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_semaplore_give(pl_semaphore_handle_t semap);

/*************************************************************************************
 * Function Name: pl_semaplore_release
 *
 * Description:
 *   release a semaphore.
 * 
 * Parameters:
 *   @semap: semaphore handle;
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_semaplore_release(pl_semaphore_handle_t semap);

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_SEMAPHORE_H__ */