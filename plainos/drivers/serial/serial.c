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

#include <config.h>
#include <errno.h>
#include <string.h>
#include <kernel/kernel.h>
#include <kernel/list.h>
#include <kernel/mempool.h>
#include <kernel/initcall.h>
#include <kernel/syslog.h>
#include <drivers/serial/serial.h>

static struct list_node pl_serial_desc_list;

/*************************************************************************************
 * Function Name: pl_serial_desc_init
 * Description: initialize serial description
 *
 * Param:
 *   @desc: serial description.
 *   @port: serial port.
 *   @ops: operations of serial.
 *   @recv_buff_size: size of buffer for receiving chars, MUST BE POWER OF 2.
 *   @recv_buff: buffer for receiving chars, if is NULL,
 *               system will alloc memory for it.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_desc_init(struct serial_desc *desc, u8_t port, struct serial_ops *ops,
                                             size_t recv_buff_size, void *recv_buff)
{
	pl_kfifo_handle recv_fifo;
	pl_semaphore_handle_t sem;

	if (!is_power_of_2(recv_buff_size))
		return -EINVAL;

	if (recv_buff == NULL) {
		recv_buff = pl_mempool_malloc(g_pl_default_mempool, recv_buff_size);
		if (recv_buff == NULL)
			return -ENOMEM;
	}

	recv_fifo = pl_kfifo_init(recv_buff, recv_buff_size);
	if (recv_fifo == NULL)
		return -EINVAL;

	sem = pl_semaplore_request(1);
	if (sem == NULL)
		return -EFAULT;

	desc->sem = sem;
	desc->port = port;
	desc->ops = ops;
	desc->trigger_length = 0;
	desc->trigger_condition = NULL;
	desc->recv_buff = recv_fifo;
	return OK;
}

/*************************************************************************************
 * Function Name: pl_serial_desc_register
 * Description: register serial description
 *
 * Param:
 *   @desc: serial description.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_desc_register(struct serial_desc *desc)
{
	if (desc == NULL)
		return -EFAULT;

	if (desc->ops == NULL)
		return -EACCES;

	list_add_node_at_tail(&pl_serial_desc_list, &desc->node);
	return OK;
}

/*************************************************************************************
 * Function Name: pl_serial_recv_handler
 * Description: the handler of serial when received characters.
 *
 * Param:
 *   @desc: serial description.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_recv_handler(struct serial_desc *desc, char *chars, size_t len)
{
	int ret;
	uint_t fifo_data_size;

	if (desc == NULL || (desc->trigger_condition == NULL &&
	    desc->trigger_length == 0))
		return -EFAULT;

	pl_kfifo_put(desc->recv_buff, chars, len);
	if (desc->trigger_condition != NULL) {
		ret = desc->trigger_condition(desc, chars, len);
		if (ret < 0)
			return ret;

		/* call callbcak */
		return OK;
	}

	fifo_data_size = pl_kfifo_len(desc->recv_buff);
	if (fifo_data_size >= desc->trigger_length) {

	}

	return OK;
}

/*************************************************************************************
 * Function Name: pl_serial_set_baud_rate
 * Description: set baud rate for serial.
 *
 * Param:
 *   @desc: serial description.
 *   @baud_rate: baud rate of the serial.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_set_baud_rate(struct serial_desc *desc, u32_t baud_rate)
{
	int ret;

	if (desc == NULL)
		return -EFAULT;

	if (desc->ops == NULL || desc->ops->set_baud_rate == NULL)
		return -EACCES;

	pl_semaplore_take(desc->sem);
	ret = desc->ops->set_baud_rate(desc, baud_rate);
	pl_semaplore_give(desc->sem);
	return ret;
}

/*************************************************************************************
 * Function Name: pl_serial_set_data_bits
 * Description: set data bits for serial.
 *
 * Param:
 *   @desc: serial description.
 *   @data_bits: data bits of the serial.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_set_data_bits(struct serial_desc *desc, u8_t data_bits)
{
	int ret;

	if (desc == NULL)
		return -EFAULT;

	if (desc->ops == NULL || desc->ops->set_data_bits == NULL)
		return -EACCES;

	pl_semaplore_take(desc->sem);
	ret = desc->ops->set_data_bits(desc, data_bits);
	pl_semaplore_give(desc->sem);
	return ret;
}

/*************************************************************************************
 * Function Name: pl_serial_set_parity_bit
 * Description: set parity bit for serial.
 *
 * Param:
 *   @desc: serial description.
 *   @parity_bit: parity bit of the serial.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_set_parity_bit(struct serial_desc *desc, u8_t parity_bit)
{
	int ret;

	if (desc == NULL)
		return -EFAULT;

	if (desc->ops == NULL || desc->ops->set_parity_bit == NULL)
		return -EACCES;

	pl_semaplore_take(desc->sem);
	ret = desc->ops->set_parity_bit(desc, parity_bit);
	pl_semaplore_give(desc->sem);
	return ret;
}

/*************************************************************************************
 * Function Name: pl_serial_set_stop_bits
 * Description: set stop bit for serial.
 *
 * Param:
 *   @desc: serial description.
 *   @stop_bits: stop bits of the serial.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_set_stop_bits(struct serial_desc *desc, u8_t stop_bits)
{
	int ret;

	if (desc == NULL)
		return -EFAULT;

	if (desc->ops == NULL || desc->ops->set_stop_bits == NULL)
		return -EACCES;

	pl_semaplore_take(desc->sem);
	ret = desc->ops->set_stop_bits(desc, stop_bits);
	pl_semaplore_give(desc->sem);
	return ret;
}

/*************************************************************************************
 * Function Name: pl_serial_send_char
 * Description: send a character by the serial.
 *
 * Param:
 *   @desc: serial description.
 *   @c: character.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_send_char(struct serial_desc *desc, char c)
{
	int ret;

	if (desc == NULL)
		return -EFAULT;

	if (desc->ops == NULL || desc->ops->send_char == NULL)
		return -EACCES;

	pl_semaplore_take(desc->sem);
	ret = desc->ops->send_char(desc, c);
	pl_semaplore_give(desc->sem);
	return ret;
}

/*************************************************************************************
 * Function Name: pl_serial_send_string
 * Description: send a string by the send_char.
 *
 * Param:
 *   @desc: serial description.
 *   @str: the string you want to send.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
static int pl_serial_send_string(struct serial_desc *desc, char *str)
{
	int ret;

	while (*str) {
		ret = desc->ops->send_char(desc, *str);
		if (ret < 0)
			return ret;

		str++;
	}

	return OK;
}

/*************************************************************************************
 * Function Name: pl_serial_send_str
 * Description: send a string by the serial.
 *
 * Param:
 *   @desc: serial description.
 *   @str: the string you want to send.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_send_str(struct serial_desc *desc, char *str)
{
	int ret;

	if (desc == NULL)
		return -EFAULT;

	if (desc->ops == NULL || (desc->ops->send_char == NULL &&
	    &desc->ops->send_str == NULL))
		return -EACCES;

	pl_semaplore_take(desc->sem);
	if (desc->ops->send_str == NULL)
		ret = pl_serial_send_string(desc, str);
	else
		ret = desc->ops->send_str(desc, str);
	pl_semaplore_give(desc->sem);

	return ret;
}

