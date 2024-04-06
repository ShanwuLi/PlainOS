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

#ifndef __DRV_IOMUX_H__
#define __DRV_IOMUX_H__

#include <types.h>
#include <kernel/list.h>

/*************************************************************************************
 * struct iomux:
 * Description:
 *   @nr: number of the io.
 *   @map: map of the ios.
 *   @function: function of the io.
 *   @state: state of the io.
 *
 *  NOTE:
 *   map:       [io_idx0, io_idx1, ..., io_idx_nr - 1]
 *   function:  [idx0_fun, idx1_fun, ..., (idxnr - 1)_fun]
 *   state:     [idx0_stat, idx1_stat, ..., (idxnr - 1)_stat]
 *
 ************************************************************************************/
struct iomux {
	u16_t nr;
	u16_t *map;
	u8_t *function;
	u8_t *state;
};

/*************************************************************************************
 * struct iomux_ops:
 * Description:
 *   @set_function: set function of the io.
 *   @get_function: get function of the ios.
 *   @set_state: set state of the io.
 *   @get_state: get state of the io.
 *
 ************************************************************************************/
struct iomux_ops {
	int (*set_function)(struct iomux *iomux, u16_t io_idx, u8_t function);
	int (*get_function)(struct iomux *iomux, u16_t io_idx, u8_t *function);
	int (*set_state)(struct iomux *iomux, u16_t io_idx, u8_t state);
	int (*get_state)(struct iomux *iomux, u16_t io_idx, u8_t *state);
};

/*************************************************************************************
 * struct iomux_desc:
 * Description:
 *   @name: the name of the iomux description.
 *   @iomux: iomux pointer.
 *   @ops: iomux operations.
 *   @node: node of iomux_desc list.
 *
 ************************************************************************************/
struct iomux_desc {
	const char *name;
	struct iomux *iomux;
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
 * Function Name: pl_iomux_desc_find
 * Description: find iomux description
 *
 * Param:
 *   @name: iomux description name.
 *
 * Return:
 *   iomux description.
 ************************************************************************************/
struct iomux_desc *pl_iomux_desc_find(const char *name);

/*================================ Client interfaces ===============================*/
/*************************************************************************************
 * Function Name: pl_iomux_set_io_function
 * Description: set function for io.
 *
 * Param:
 *   @desc: iomux description.
 *   @io_idx: index of io.
 *   @function: function of the io.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_set_io_function(struct iomux_desc *desc, u16_t io_idx, u8_t function);

/*************************************************************************************
 * Function Name: pl_iomux_get_io_function
 * Description: get function of the io.
 *
 * Param:
 *   @desc: iomux description.
 *   @io_idx: index of io.
 *   @function: function of the io.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_get_io_function(struct iomux_desc *desc, u16_t io_idx, u8_t *function);

/*************************************************************************************
 * Function Name: pl_iomux_set_io_state
 * Description: set state of the io.
 *
 * Param:
 *   @desc: iomux description.
 *   @io_idx: index of io.
 *   @state: state of the io.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_set_io_state(struct iomux_desc *desc, u16_t io_idx, u8_t state);

/*************************************************************************************
 * Function Name: pl_iomux_get_io_state
 * Description: get state of the io.
 *
 * Param:
 *   @desc: iomux description.
 *   @io_idx: index of io.
 *   @state: state of the io.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_get_io_state(struct iomux_desc *desc, u16_t io_idx, u8_t *state);

#endif /* __DRV_IOMUX_H__ */
