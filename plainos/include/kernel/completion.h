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

#ifndef __KERNEL_COMPLETE_H__
#define __KERNEL_COMPLETE_H__

#include <types.h>
#include <kernel/list.h>

struct pl_completion {
	struct list_node wait_list;
	int_t done;
};

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************
 * Function Name: pl_completion_init
 *
 * Description:
 *   request a semaphore.
 *
 * Parameters:
 *  @comp: completion handle.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure..
 ************************************************************************************/
int pl_completion_init(struct pl_completion *comp);

/*************************************************************************************
 * Function Name: pl_completion_wait
 *
 * Description:
 *    wait a completion.
 * 
 * Parameters:
 *  @comp: completion handle.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_completion_wait(struct pl_completion *comp);

/*************************************************************************************
 * Function Name: pl_completion_post
 *
 * Description:
 *    post a completion.
 * 
 * Parameters:
 *  @comp: completion handle.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_completion_post(struct pl_completion *comp);

/*************************************************************************************
 * Function Name: pl_completion_post_all
 *
 * Description:
 *    post all completion.
 * 
 * Parameters:
 *  @comp: completion handle.
 *
 * Return:
 *  Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_completion_post_all(struct pl_completion *comp);

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_COMPLETE_H__ */
