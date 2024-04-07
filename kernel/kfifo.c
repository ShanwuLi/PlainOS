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

#include <errno.h>
#include <types.h>
#include <string.h>
#include <port.h>
#include <kernel/kernel.h>
#include <kernel/mempool.h>
#include <kernel/kfifo.h>

struct kfifo {
	uint_t in;
	uint_t out;
	uint_t size;
	char *buff;
};

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
pl_kfifo_handle pl_kfifo_init(char *buff, uint_t buff_size)
{
	struct kfifo *fifo;

	if (buff == NULL || buff_size == 0)
		return NULL;

	if (!is_power_of_2(buff_size))
		return NULL;

	fifo = (struct kfifo *)pl_mempool_malloc(g_pl_default_mempool, sizeof(struct kfifo));
	if (fifo == NULL)
		return NULL;

	fifo->buff = buff;
	fifo->size = buff_size;
	fifo->in = 0;
	fifo->out = 0;

	return fifo;
}

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
pl_kfifo_handle pl_kfifo_request(uint_t buff_size)
{
	struct kfifo *kfifo;

	if (!is_power_of_2(buff_size))
		return NULL;

	kfifo = pl_mempool_malloc(g_pl_default_mempool, sizeof(struct kfifo) + buff_size);
	if (kfifo == NULL)
		return NULL;

	kfifo->buff = (char *)kfifo + sizeof(struct kfifo);
	kfifo->size = buff_size;
	kfifo->in = 0;
	kfifo->out = 0;

	return kfifo;
}

/*************************************************************************************
 * Function Name: pl_kfifo_destroy
 * Description: destroy a kfifo.
 *
 * Param:
 *   @datafifo: datafifo handle.
 *
 * Return:
 *   void.
 ************************************************************************************/
void pl_kfifo_destroy(pl_kfifo_handle fifo)
{
	struct kfifo *kfifo = (struct kfifo *)fifo;

	if (fifo == NULL)
		return;

	kfifo->in = 0;
	kfifo->out = 0;
	kfifo->size = 0;
	kfifo->buff = NULL;
	pl_mempool_free(g_pl_default_mempool, kfifo);
}

/*************************************************************************************
 * Function Name: kfifo_len
 * Description: get the data length of the kfifo.
 *
 * Param:
 *   @fifo: kfifo handle.
 *
 * Return:
 *   the length of the available data in kfifo.
 ************************************************************************************/
uint_t kfifo_len(pl_kfifo_handle fifo)
{
	struct kfifo *kfifo = (struct kfifo *)fifo;

	return (kfifo->in - kfifo->out);
}

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
uint_t pl_kfifo_get(pl_kfifo_handle fifo, char *data, uint_t data_len)
{
	uint_t len;
	struct kfifo *kfifo = (struct kfifo *)fifo;
	uint_t size = min(data_len, kfifo->in - kfifo->out);

	pl_port_cpu_dmb();
	/* first get the data from fifo->out until the end of the buffer */
	len = min(size, kfifo->size - (kfifo->out & (kfifo->size - 1)));
	memcpy(data, kfifo->buff + (kfifo->out & (kfifo->size - 1)), len);
	/* then get the rest (if any) from the beginning of the buffer */
	memcpy(data + len, kfifo->buff, size - len);

	pl_port_cpu_dmb();
	kfifo->out += size;

	return size;
}

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
uint_t pl_kfifo_put(pl_kfifo_handle fifo, char *data, uint_t data_len)
{
	uint_t len;
	struct kfifo *kfifo = (struct kfifo *)fifo;
	uint_t size = min(data_len, kfifo->size - kfifo->in + kfifo->out);

	pl_port_cpu_dmb();
	/* first put the data starting from fifo->in to buffer end */
	len  = min(size, kfifo->size - (kfifo->in & (kfifo->size - 1)));
	memcpy(kfifo->buff + (kfifo->in & (kfifo->size - 1)), data, len);
	/* then put the rest (if any) at the beginning of the buffer */
	memcpy(kfifo->buff, data + len, size - len);

	pl_port_cpu_dmb();
	kfifo->in += size;

	return size;
}