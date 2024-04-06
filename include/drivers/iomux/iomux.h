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

#ifndef __KERNEL_DRV_IOMUX_H__
#define __KERNEL_DRV_IOMUX_H__

#include <types.h>
#include <kernel/list.h>

struct iomux {
	u16_t idx;
	u8_t function;
	u8_t state;
};

struct iomux_ops {
	int (*set_function)(struct iomux *iomux, u8_t function);
	int (*get_function)(struct iomux *iomux, u8_t *function);
	int (*set_state)(struct iomux *iomux, u8_t state);
	int (*get_state)(struct iomux *iomux, u8_t *state);
};

struct iomux_desc {
	const char *name;
	u16_t id;
	u16_t ios_nr;
	struct iomux *ios;
	struct iomux_ops *ops;
	struct list_node node;
};

/*============================== Controller Interfaces =============================*/
/*************************************************************************************
 * Function Name: pl_iomux_desc_register
 * Description: register iomux description
 *
 * Param:
 *   @desc: iomux description.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_desc_register(struct iomux_desc *desc);

/*************************************************************************************
 * Function Name: pl_iomux_desc_unregister
 * Description: unregister iomux description
 *
 * Param:
 *   @desc: iomux description.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
void pl_iomux_desc_unregister(struct iomux_desc *desc);

/*************************************************************************************
 * Function Name: pl_iomux_desc_find_by_name
 * Description: find iomux description
 *
 * Param:
 *   @name: iomux description name.
 *
 * Return:
 *   iomux description.
 ************************************************************************************/
struct iomux_desc *pl_iomux_desc_find_by_name(const char *name);

/*************************************************************************************
 * Function Name: pl_iomux_desc_find_by_name
 * Description: find iomux description
 *
 * Param:
 *   @id: iomux description id.
 *
 * Return:
 *   iomux description.
 ************************************************************************************/
struct iomux_desc *pl_iomux_desc_find_by_id(u16_t id);

/*================================ Client interfaces ===============================*/
/*************************************************************************************
 * Function Name: pl_iomux_set_pin_function
 * Description: set function for pin.
 *
 * Param:
 *   @desc: iomux description.
 *   @io_idx: index of io.
 *   @function: function of the pin.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_set_pin_function(struct iomux_desc *desc, u16_t io_idx, u8_t function);

/*************************************************************************************
 * Function Name: pl_iomux_get_pin_function
 * Description: get function of the pin.
 *
 * Param:
 *   @desc: iomux description.
 *   @io_idx: index of io.
 *   @function: function of the pin.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_get_pin_function(struct iomux_desc *desc, u16_t io_idx, u8_t *function);

/*************************************************************************************
 * Function Name: pl_iomux_set_pin_state
 * Description: set state of the pin.
 *
 * Param:
 *   @desc: iomux description.
 *   @io_idx: index of io.
 *   @state: state of the pin.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_set_pin_state(struct iomux_desc *desc, u16_t io_idx, u8_t state);

/*************************************************************************************
 * Function Name: pl_iomux_get_pin_state
 * Description: get state of the pin.
 *
 * Param:
 *   @desc: iomux description.
 *   @io_idx: index of io.
 *   @state: state of the pin.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_get_pin_state(struct iomux_desc *desc, u16_t io_idx, u8_t *state);

#endif /* __KERNEL_DRV_IOMUX_H__ */
