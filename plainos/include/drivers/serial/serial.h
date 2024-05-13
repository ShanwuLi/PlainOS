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

#ifndef __DRV_SERIAL_H__
#define __DRV_SERIAL_H__

#include <types.h>
#include <kernel/list.h>
#include <kernel/kfifo.h>
#include <kernel/workqueue.h>
#include <kernel/semaphore.h>
#include <kernel/kfifo.h>

struct pl_serial_desc;

/*************************************************************************************
 * struct serial_recv_info:
 * Description:
 *   @call_condition: callback function will be called when call_condition return
 *                    equal to or greater 0.
 *   @recv_fifo: fifo of receiving characters.
 *   @chars: recevied characters at the moment.
 *   @chars_len: the length of recevied characters at the moment.
 *   @callback: callback function will be called when call_condition return equal to
 *              or greater 0.
 ************************************************************************************/
typedef int (*pl_serial_call_condition_t)(struct pl_kfifo *recv_fifo,
                                          char *chars, uint_t chars_len);
typedef void (*pl_serial_callback_t)(struct pl_kfifo *recv_fifo);

/*************************************************************************************
 * struct serial_ops:
 * Description:
 *   @set_baud_rate: set buard rate for serial.
 *   @set_data_bits: set data bits for serial.
 *   @set_parity_bit: set parity bit for serial.
 *   @set_stop_bits: set stop bits for serial.
 *   @send_char: synchronous sending char.
 *   @recv_char: synchronous reception char.
 *   @send_str: synchronous sending string.
 *
 * NOTE:
 *   Trigger_condition has a higher priority than length triggering. 
 ************************************************************************************/
struct pl_serial_ops {
	/* serial settings */
	int (*set_baud_rate)(struct pl_serial_desc *desc, u32_t baud_rate);
	int (*set_data_bits)(struct pl_serial_desc *desc, u8_t data_bits);
	int (*set_parity_bit)(struct pl_serial_desc *desc, u8_t parity_bit);
	int (*set_stop_bits)(struct pl_serial_desc *desc, u8_t stop_bits);
	/* serial functions */
	int (*send_char)(struct pl_serial_desc *desc, char c);
	int (*recv_char)(struct pl_serial_desc *desc, char *c);
	int (*send_str)(struct pl_serial_desc *desc, char *str);
};

/*************************************************************************************
 * struct serial_recv_info:
 * Description:
 *   @fifo: fifo of receiving characters.
 *   @call_condition: callback function will be called when call_condition return
 *                    equal to or greater 0.
 *   @callback: callback function will be called when call_condition return equal to
 *              or greater 0.
 *   @cb_work: work for callback(will be executed in thread context).
 ************************************************************************************/
struct pl_serial_recv_info {
	struct pl_kfifo *fifo;
	pl_serial_call_condition_t call_condition;
	pl_serial_callback_t callback;
	struct pl_work cb_work;
};

/*************************************************************************************
 * struct serial_ops:
 * Description:
 *   @port: the number of the serial port.
 *   @data_bits: data bits of the serial.
 *   @parity_bit: parity bit.
 *   @stop_bits: stop bit.
 *   @baud_rate: baud rate.
 *   @ops: serial's operations.
 *   @node: list node of pl_serial_desc all registered.
 *   @sem: semaphore for serial.
 *   @recv_info: information of receiving characters.
 ************************************************************************************/
struct pl_serial_desc {
	u8_t port;
	u8_t data_bits;
	u8_t parity_bit;
	u8_t stop_bits;
	u32_t baud_rate;
	struct pl_serial_ops *ops;
	struct list_node node;
	struct pl_sem sem;
	struct pl_serial_recv_info recv_info;
};

/*===================================== BSP Driver =================================*/
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
              struct pl_serial_ops *ops, uint_t recv_buff_size);

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
int pl_serial_desc_register(struct pl_serial_desc *desc);

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
                                char *chars, uint_t chars_len);


/*================================== Client Driver =================================*/
/*************************************************************************************
 * Function Name: pl_serial_find_desc
 * Description: find serial description by port.
 *
 * Param:
 *   @port: serial port.
 *
 * Return:
 *   struct pl_serial_desc if is not NULL.
 ************************************************************************************/
struct pl_serial_desc *pl_serial_find_desc(u8_t port);

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
int pl_serial_set_baud_rate(struct pl_serial_desc *desc, u32_t baud_rate);

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
int pl_serial_set_data_bits(struct pl_serial_desc *desc, u8_t data_bits);

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
int pl_serial_set_parity_bit(struct pl_serial_desc *desc, u8_t parity_bit);

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
int pl_serial_set_stop_bits(struct pl_serial_desc *desc, u8_t stop_bits);

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
int pl_serial_send_char(struct pl_serial_desc *desc, char c);

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
int pl_serial_send_str(struct pl_serial_desc *desc, char *str);

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
                                  pl_serial_call_condition_t condition);

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
int pl_serial_unregister_recv_call_condition(struct pl_serial_desc *desc);

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
                              pl_serial_callback_t recv_callback);

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
int pl_serial_unregister_recv_callback(struct pl_serial_desc *desc);

#endif /* __DRV_SERIAL_H__ */
