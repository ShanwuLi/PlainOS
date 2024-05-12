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
 * Function Name: pl_serial_recv_cb_work_func
 * Description: work function for callback.
 *
 * Param:
 *   @work: passed work.
 *
 * Return:
 *   void.
 ************************************************************************************/
static void pl_serial_recv_cb_work_func(struct pl_work *work)
{
	struct pl_serial_recv_info *recv_info = container_of(work,\
	                                        struct pl_serial_recv_info, cb_work);

	if (recv_info->callback == NULL)
		return;

	recv_info->callback(recv_info->fifo);
}

/*************************************************************************************
 * Function Name: pl_serial_desc_init
 * Description: initialize serial description
 *
 * Param:
 *   @desc: serial description.
 *   @port: serial port.
 *   @ops: operations of serial.
 *   @recv_buff_size: size of buffer for receiving chars, MUST BE POWER OF 2.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_desc_init(struct pl_serial_desc *desc, u8_t port,
              struct pl_serial_ops *ops, uint_t recv_buff_size)
{
	int ret;
	struct pl_kfifo *fifo;

	if (desc == NULL)
		return -EFAULT;

	ret = pl_semaphore_init(&desc->sem, 1);
	if (ret < 0)
		return ret;

	ret = pl_work_init(&desc->recv_info.cb_work,
	                   pl_serial_recv_cb_work_func, desc);
	if (ret < 0)
		return ret;

	fifo = pl_kfifo_request(recv_buff_size);
	if (fifo == NULL)
		return -ENOMEM;

	desc->port = port;
	desc->ops = ops;
	desc->recv_info.fifo = fifo;
	desc->recv_info.callback = NULL;
	desc->recv_info.call_condition = NULL;
	list_init(&desc->node);

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
int pl_serial_desc_register(struct pl_serial_desc *desc)
{
	if (desc == NULL)
		return -EFAULT;

	if (desc->ops == NULL)
		return -EACCES;

	list_add_node_at_tail(&pl_serial_desc_list, &desc->node);
	return OK;
}

/*************************************************************************************
 * Function Name: pl_serial_callee_recv_handler
 * Description: the handler of serial when received characters.
 *
 * Param:
 *   @desc: serial description.
 *   @chars: characters received.
 *   @chars_len: length of characters received.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_callee_recv_handler(struct pl_serial_desc *desc,
                                char *chars, uint_t chars_len)
{
	int ret;
	pl_serial_call_condition_t call_condition;

	if (desc == NULL)
		return -EFAULT;

	call_condition = desc->recv_info.call_condition;
	if (call_condition == NULL)
		return OK;

	ret = call_condition(desc->recv_info.fifo, chars, chars_len);
	if (ret < 0)
		return OK;

	/* call callbcak */
	ret = pl_work_add(g_pl_sys_hiwq_handle, &desc->recv_info.cb_work);
	return ret;

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
int pl_serial_set_baud_rate(struct pl_serial_desc *desc, u32_t baud_rate)
{
	int ret;

	if (desc == NULL)
		return -EFAULT;

	if (desc->ops == NULL || desc->ops->set_baud_rate == NULL)
		return -EACCES;

	pl_semaphore_wait(&desc->sem);
	ret = desc->ops->set_baud_rate(desc, baud_rate);
	pl_semaphore_post(&desc->sem);
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
int pl_serial_set_data_bits(struct pl_serial_desc *desc, u8_t data_bits)
{
	int ret;

	if (desc == NULL)
		return -EFAULT;

	if (desc->ops == NULL || desc->ops->set_data_bits == NULL)
		return -EACCES;

	pl_semaphore_wait(&desc->sem);
	ret = desc->ops->set_data_bits(desc, data_bits);
	pl_semaphore_post(&desc->sem);
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
int pl_serial_set_parity_bit(struct pl_serial_desc *desc, u8_t parity_bit)
{
	int ret;

	if (desc == NULL)
		return -EFAULT;

	if (desc->ops == NULL || desc->ops->set_parity_bit == NULL)
		return -EACCES;

	pl_semaphore_wait(&desc->sem);
	ret = desc->ops->set_parity_bit(desc, parity_bit);
	pl_semaphore_post(&desc->sem);
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
int pl_serial_set_stop_bits(struct pl_serial_desc *desc, u8_t stop_bits)
{
	int ret;

	if (desc == NULL)
		return -EFAULT;

	if (desc->ops == NULL || desc->ops->set_stop_bits == NULL)
		return -EACCES;

	pl_semaphore_wait(&desc->sem);
	ret = desc->ops->set_stop_bits(desc, stop_bits);
	pl_semaphore_post(&desc->sem);
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
int pl_serial_send_char(struct pl_serial_desc *desc, char c)
{
	int ret;

	if (desc == NULL)
		return -EFAULT;

	if (desc->ops == NULL || desc->ops->send_char == NULL)
		return -EACCES;

	pl_semaphore_wait(&desc->sem);
	ret = desc->ops->send_char(desc, c);
	pl_semaphore_post(&desc->sem);
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
static int pl_serial_send_string(struct pl_serial_desc *desc, char *str)
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
int pl_serial_send_str(struct pl_serial_desc *desc, char *str)
{
	int ret;

	if (desc == NULL)
		return -EFAULT;

	if (desc->ops == NULL || (desc->ops->send_char == NULL &&
	    &desc->ops->send_str == NULL))
		return -EACCES;

	pl_semaphore_wait(&desc->sem);
	if (desc->ops->send_str == NULL)
		ret = pl_serial_send_string(desc, str);
	else
		ret = desc->ops->send_str(desc, str);
	pl_semaphore_post(&desc->sem);

	return ret;
}

/*************************************************************************************
 * Function Name: pl_serial_register_recv_call_condition
 * Description: register call condition for serial when received some characters.
 *
 * Param:
 *   @desc: serial description.
 *   @condition: call condition.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_register_recv_call_condition(struct pl_serial_desc *desc,
                                  pl_serial_call_condition_t condition)
{
	if (desc == NULL)
		return -EFAULT;

	pl_port_enter_critical();
	if (desc->recv_info.call_condition != NULL) {
		pl_port_exit_critical();
		return -EBUSY;
	}

	desc->recv_info.call_condition = condition;
	pl_port_exit_critical();

	return OK;
}

/*************************************************************************************
 * Function Name: pl_serial_unregister_recv_call_condition
 * Description: unregister call condition for serial when received some characters.
 *
 * Param:
 *   @desc: serial description.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_unregister_recv_call_condition(struct pl_serial_desc *desc)
{
	if (desc == NULL)
		return -EFAULT;

	pl_port_enter_critical();
	desc->recv_info.call_condition = NULL;
	pl_port_exit_critical();

	return OK;
}

/*************************************************************************************
 * Function Name: pl_serial_send_string
 * Description: register callback for serial when received some characters.
 *
 * Param:
 *   @desc: serial description.
 *   @recv_callback: callback function.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_register_recv_callback(struct pl_serial_desc *desc,
                              pl_serial_callback_t recv_callback)
{
	if (desc == NULL)
		return -EFAULT;

	pl_task_schedule_lock();
	if (desc->recv_info.callback != NULL) {
		pl_task_schedule_unlock();
		return -EBUSY;
	}

	desc->recv_info.callback = recv_callback;
	pl_task_schedule_unlock();

	return OK;
}

/*************************************************************************************
 * Function Name: pl_serial_unregister_recv_callback
 * Description: unregister callback for serial when received some characters.
 *
 * Param:
 *   @desc: serial description.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_serial_unregister_recv_callback(struct pl_serial_desc *desc)
{
	if (desc == NULL)
		return -EFAULT;

	pl_task_schedule_lock();
	desc->recv_info.callback = NULL;
	pl_task_schedule_unlock();

	return OK;
}
