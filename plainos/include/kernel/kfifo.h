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

#ifndef __KERNEL_KFIFO_H__
#define __KERNEL_KFIFO_H__

#include <types.h>
#include <errno.h>

#define PL_KFIFO_EMPTY       (-EEMPTY)
#define PL_KFIFO__FULL       (-EFULL)

typedef void* pl_kfifo_handle;

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************
 * Function Name: pl_kfifo_init
 * Description: initialize a fifo.
 *
 * Param:
 *   @buff: buffer of fifo.
 *   @buff_size: size of the buffer.
 *
 * Return:
 *   fifodata handle.
 ************************************************************************************/
pl_kfifo_handle pl_kfifo_init(char *buff, uint_t buff_size);

/*************************************************************************************
 * Function Name: pl_kfifo_request
 * Description: request and initialize a kfifo.
 *
 * Param:
 *   @buff_size: size of the datafifo buffer.
 *
 * Return:
 *   fifodata handle.
 ************************************************************************************/
pl_kfifo_handle pl_kfifo_request(uint_t buff_size);

/*************************************************************************************
 * Function Name: pl_kfifo_destroy
 * Description: destroy a kfifo.
 *
 * Param:
 *   @fifo: datafifo handle.
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_kfifo_destroy(pl_kfifo_handle fifo);

/*************************************************************************************
 * Function Name: pl_kfifo_len
 * Description: get the data length of the kfifo.
 *
 * Param:
 *   @fifo: kfifo handle.
 *
 * Return:
 *   the length of the available data in kfifo.
 ************************************************************************************/
uint_t pl_kfifo_len(pl_kfifo_handle fifo);

/*************************************************************************************
 * Function Name: pl_kfifo_put
 * Description: put the data to the kfifo.
 *
 * Param:
 *   @fifo: kfifo handle.
 *   @data: data to be pushed.
 *   @data_len: data length.
 *
 * Return:
 *   The actual length of data put to the kfifo.
 ************************************************************************************/
uint_t pl_kfifo_put(pl_kfifo_handle fifo, char *data, uint_t data_len);

/*************************************************************************************
 * Function Name: pl_kfifo_get
 * Description: get the data from kfifo.
 *
 * Param:
 *   @fifo: kfifo handle.
 *   @data: data to be pushed.
 *   @data_len: data length.
 *
 * Return:
 *   The actual length of data got from kfifo.
 ************************************************************************************/
uint_t pl_kfifo_get(pl_kfifo_handle fifo, char *data, uint_t data_len);

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_KFIFO_H__ */
