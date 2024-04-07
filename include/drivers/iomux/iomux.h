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


enum {
	IOMUX_SET_FUNC = 0,
	IOMUX_SET_DRV,
	IOMUX_SET_STATE,
	IOMUX_SET_END
};

enum {
	IOMUX_GET_FUNC = 0,
	IOMUX_GET_DRV,
	IOMUX_GET_STATE,
	IOMUX_GET_END
};

/*************************************************************************************
 * struct iomux:
 * Description:
 *   @nr: number of the io.
 *   @some: special purpose for some.
 *
 ************************************************************************************/
struct iomux {
	u32_t nr;
	void *some;
};

/*************************************************************************************
 * struct iomux_ops:
 * Description:
 *   @set_one: set one of the some.
 *   @get_one: get one of the some.
 *   @set_grp: set group of the some.
 *   @get_grp: get group of the some.
 *
 ************************************************************************************/
struct iomux_ops {
	int (*set_one)(struct iomux *iomux, u16_t io_idx, u8_t set, u8_t one);
	int (*get_one)(struct iomux *iomux, u16_t io_idx, u8_t get, u8_t *one);
	int (*set_grp)(struct iomux *iomux, u16_t io_idx_s, u16_t io_idx_e, u8_t set, u8_t *grp);
	int (*get_grp)(struct iomux *iomux, u16_t io_idx_s, u16_t io_idx_e, u8_t get, u8_t *grp);
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
 * Function Name: pl_iomux_set_io_one
 * Description: set one of the io.
 *
 * Param:
 *   @desc: iomux description.
 *   @io_idx: index of io.
 *   @one: one of the io.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_set_io_one(struct iomux_desc *desc, u16_t io_idx, u8_t set, u8_t one);

/*************************************************************************************
 * Function Name: pl_iomux_get_io_one
 * Description: get one of the io.
 *
 * Param:
 *   @desc: iomux description.
 *   @io_idx: index of io.
 *   @one: one of the io.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_get_io_one(struct iomux_desc *desc, u16_t io_idx, u8_t get, u8_t *one);

/*************************************************************************************
 * Function Name: pl_iomux_set_io_grp
 * Description: set group of the io.
 *
 * Param:
 *   @desc: iomux description.
 *   @io_idx_s: start index of io group.
 *   @io_idx_e: end index of io group.
 *   @set: set cmd.
 *   @grp: group.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_set_io_grp(struct iomux_desc *desc, u16_t io_idx_s,
                            u16_t io_idx_e, u8_t set, u8_t *grp);

/*************************************************************************************
 * Function Name: pl_iomux_get_io_grp
 * Description: get group of the io.
 *
 * Param:
 *   @desc: iomux description.
 *   @io_idx_s: start index of io group.
 *   @io_idx_e: end index of io group.
 *   @set: set cmd.
 *   @grp: group.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_get_io_grp(struct iomux_desc *desc, u16_t io_idx_s,
                            u16_t io_idx_e, u8_t get, u8_t *grp);

#endif /* __DRV_IOMUX_H__ */
