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

#ifndef __KERNEL_DATAFIFO_H__
#define __KERNEL_DATAFIFO_H__

#include <types.h>
#include <errno.h>

#define PL_DATAFIFO_EMPTY       (-EEMPTY)
#define PL_DATAFIFO_FULL        (-EFULL)

typedef void* datafifo_handle;

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************
 * Function Name: pl_datafifo_init
 * Description: initialize a data fifo.
 *
 * Param:
 *   @buff: buffer of datafifo.
 *   @buff_size: size of the buffer.
 *
 * Return:
 *   fifodata handle.
 ************************************************************************************/
datafifo_handle pl_datafifo_init(char *buff, size_t buff_size);

/*************************************************************************************
 * Function Name: pl_datafifo_request
 * Description: request and initialize a data fifo.
 *
 * Param:
 *   @buff_size: size of the datafifo buffer.
 *
 * Return:
 *   fifodata handle.
 ************************************************************************************/
datafifo_handle pl_datafifo_request(size_t buff_size);

/*************************************************************************************
 * Function Name: pl_datafifo_destroy
 * Description: destroy a data fifo.
 *
 * Param:
 *   @datafifo: datafifo handle.
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_datafifo_destroy(datafifo_handle fifo);

/*************************************************************************************
 * Function Name: pl_datafifo_push
 * Description: push data to datafifo.
 *
 * Param:
 *   @datafifo: datafifo handle.
 *   @data: data to be pushed.
 *   @data_len: data length.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_datafifo_push(datafifo_handle fifo, char *data, size_t data_len);

/*************************************************************************************
 * Function Name: pl_datafifo_pop
 * Description: pop data from datafifo.
 *
 * Param:
 *   @datafifo: datafifo handle.
 *   @data: data to be poped.
 *   @data_len: data length.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_datafifo_pop(datafifo_handle fifo, char *data, size_t data_len);

#ifdef __cplusplus
}
#endif

#endif /* __KERNEL_DATAFIFO_H__ */
