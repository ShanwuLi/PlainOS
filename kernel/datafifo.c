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

#include <string.h>
#include <kernel/kernel.h>
#include <kernel/mempool.h>
#include <kernel/datafifo.h>

struct data_fifo {
	size_t in;
	size_t out;
	size_t size;
	char *buff;
};

static size_t datafifo_data_num(struct data_fifo *fifo)
{
	return (fifo->in - fifo->out + fifo->size) % fifo->size;
}

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
datafifo_handle pl_datafifo_init(char *buff, size_t buff_size)
{
	struct data_fifo *fifo = pl_mempool_malloc(g_pl_default_mempool,
	                                      sizeof(struct data_fifo));

	if (fifo == NULL)
		return NULL;

	fifo->buff = buff;
	fifo->size = buff_size;
	fifo->in = 0;
	fifo->out = 0;

	return fifo;
}

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
datafifo_handle pl_datafifo_request(size_t buff_size)
{
	struct data_fifo *fifo = pl_mempool_malloc(g_pl_default_mempool,
	                         sizeof(struct data_fifo) + buff_size);

	if (fifo == NULL)
		return NULL;

	fifo->buff = (char *)fifo + sizeof(struct data_fifo);
	fifo->size = buff_size;
	fifo->in = 0;
	fifo->out = 0;

	return fifo;
}

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
void pl_datafifo_destroy(datafifo_handle fifo)
{
	struct data_fifo *datafifo = (struct data_fifo *)fifo;

	if (fifo == NULL)
		return;

	datafifo->in = 0;
	datafifo->out = 0;
	datafifo->size = 0;
	datafifo->buff = NULL;
	pl_mempool_free(g_pl_default_mempool, datafifo);
}

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
int pl_datafifo_push(datafifo_handle fifo, char *data, size_t data_len)
{
	struct data_fifo *datafifo = (struct data_fifo *)fifo;
	size_t datafifo_in;

	pl_enter_critical();
	if ((datafifo->size - datafifo_data_num(fifo) - 1) < data_len) {
		pl_exit_critical();
		return -EFULL;
	}

	datafifo_in = datafifo->in;
	datafifo->in = (datafifo->in + data_len + 1) % datafifo->size;
	pl_exit_critical();

	if (datafifo_in + data_len < datafifo->size) {
		memcpy(&datafifo->buff[datafifo_in], data, data_len);
		return OK;
	}

	memcpy(&datafifo->buff[datafifo_in], data, datafifo->size - datafifo_in);
	memcpy(&datafifo->buff[0], data + datafifo->size - datafifo_in, datafifo->in);

	return OK;
}

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
int pl_datafifo_pop(datafifo_handle fifo, char *data, size_t data_len)
{
	size_t datafifo_out;
	struct data_fifo *datafifo = (struct data_fifo *)fifo;

	pl_enter_critical();
	if (datafifo_data_num(fifo) < data_len) {
		pl_exit_critical();
		return -EEMPTY;
	}

	datafifo_out = datafifo->out;
	datafifo->out = (datafifo->out + data_len) % datafifo->size;
	pl_exit_critical();

	if (datafifo_out + data_len < datafifo->size) {
		memcpy(&datafifo->buff[datafifo_out], data, data_len);
		return OK;
	}

	memcpy(data, &datafifo->buff[datafifo_out], datafifo->size - datafifo_out);
	memcpy(data + datafifo->size - datafifo_out, &datafifo->buff[0], datafifo->in);
	return OK;
}
