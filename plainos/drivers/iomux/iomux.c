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
#include <kernel/list.h>
#include <kernel/initcall.h>
#include <kernel/syslog.h>
#include <drivers/iomux/iomux.h>

typedef int (*set_item_t)(struct iomux *iomux, u16_t io_idx, u8_t item);
typedef int (*get_item_t)(struct iomux *iomux, u16_t io_idx, u8_t *item);

static struct list_node pl_iomux_desc_list;

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
int pl_iomux_desc_register(struct iomux_desc *desc)
{
	if (desc == NULL)
		return -EFAULT;

	if (desc->name == NULL || desc->iomux == NULL || desc->ops == NULL)
		return -EACCES;

	list_add_node_at_tail(&pl_iomux_desc_list, &desc->node);
	return OK;
}

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
void pl_iomux_desc_unregister(struct iomux_desc *desc)
{
	if (desc == NULL || list_is_empty(&desc->node))
		return;

	list_del_node(&desc->node);
}

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
struct iomux_desc *pl_iomux_desc_find(const char *name)
{
	struct iomux_desc *pos;

	if (list_is_empty(&pl_iomux_desc_list) || name == NULL)
		return NULL;

	list_for_each_entry(pos, &pl_iomux_desc_list, struct iomux_desc, node) {
		if (strcmp(pos->name, name) == 0) {
			return pos;
		}
	}

	return NULL;
}

/*************************************************************************************
 * Function Name: pl_iomux_set_io_item
 * Description: set item of the io.
 *
 * Param:
 *   @desc: iomux description.
 *   @ops_offset: operation function offset.
 *   @io_idx: index of io.
 *   @item: item of the io.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
static int pl_iomux_set_io_item(struct iomux_desc *desc, int ops_offset,
                                u16_t io_idx, u8_t item)
{
	set_item_t set_item;

	if (desc == NULL || desc->iomux == NULL)
		return -EFAULT;

	if (io_idx >= desc->iomux->io_nr)
		return -ERANGE;

	set_item = (set_item_t)(desc->ops) + ops_offset;
	return (set_item == NULL) ? -ENOSUPPORT : set_item(desc->iomux, io_idx, item);
}

/*************************************************************************************
 * Function Name: pl_iomux_get_io_item
 * Description: get item of the io.
 *
 * Param:
 *   @desc: iomux description.
 *   @ops_offset: operation function offset.
 *   @io_idx: index of io.
 *   @item: item of the io.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
static int pl_iomux_get_io_item(struct iomux_desc *desc, int ops_offset,
                                u16_t io_idx, u8_t *item)
{
	get_item_t get_item;

	if (desc == NULL || desc->iomux == NULL || item == NULL)
		return -EFAULT;

	if (io_idx >= desc->iomux->io_nr)
		return -ERANGE;

	get_item = (get_item_t)(desc->ops) + ops_offset;
	return (get_item == NULL) ? -ENOSUPPORT : get_item(desc->iomux, io_idx, item);
}


/*************************************************************************************
 * Function Name: pl_iomux_set_io_func
 * Description: set function of the io.
 *
 * Param:
 *   @desc: iomux description.
 *   @io_idx: index of io.
 *   @func: function of the io.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_set_io_func(struct iomux_desc *desc, u16_t io_idx, u8_t func)
{
	return pl_iomux_set_io_item(desc, 0, io_idx, func);
}

/*************************************************************************************
 * Function Name: pl_iomux_get_io_func
 * Description: get function of the io.
 *
 * Param:
 *   @desc: iomux description.
 *   @io_idx: index of io.
 *   @func: function of the io.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_get_io_func(struct iomux_desc *desc, u16_t io_idx, u8_t *func)
{
	return pl_iomux_get_io_item(desc, 1, io_idx, func); 
}

/*************************************************************************************
 * Function Name: pl_iomux_set_io_drv
 * Description: set driver strength of the io.
 *
 * Param:
 *   @desc: iomux description.
 *   @io_idx: index of io.
 *   @drv: driver strength of the io.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_set_io_drv(struct iomux_desc *desc, u16_t io_idx, u8_t drv)
{
	return pl_iomux_set_io_item(desc, 2, io_idx, drv);
}

/*************************************************************************************
 * Function Name: pl_iomux_get_io_drv
 * Description: get driver strength of the io.
 *
 * Param:
 *   @desc: iomux description.
 *   @io_idx: index of io.
 *   @drv: driver strength of the io.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_get_io_drv(struct iomux_desc *desc, u16_t io_idx, u8_t *drv)
{
	return pl_iomux_get_io_item(desc, 3, io_idx, drv);
}

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
int pl_iomux_set_io_state(struct iomux_desc *desc, u16_t io_idx, u8_t state)
{
	return pl_iomux_set_io_item(desc, 4, io_idx, state);
}

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
int pl_iomux_get_io_state(struct iomux_desc *desc, u16_t io_idx, u8_t *state)
{
	return pl_iomux_get_io_item(desc, 5, io_idx, state);
}

/*************************************************************************************
 * Function Name: pl_iomux_set_io_one
 * Description: set one of the io.
 *
 * Param:
 *   @desc: iomux description.
 *   @io_idx: index of io.
 *   @set: set_cmd.
 *   @value: value of the io.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_set_io_one(struct iomux_desc *desc, u16_t io_idx, u8_t set, u8_t value)
{
	int ret;

	if (desc == NULL || desc->iomux == NULL || desc->ops == NULL)
		return -EFAULT;

	if (io_idx >= desc->iomux->io_nr)
		return -ERANGE;
	
	if (desc->ops->set_one == NULL)
		return -ENOSUPPORT;

	ret = desc->ops->set_one(desc->iomux, io_idx, set, value);
	return ret;
}

/*************************************************************************************
 * Function Name: pl_iomux_get_io_one
 * Description: get one of the io.
 *
 * Param:
 *   @desc: iomux description.
 *   @io_idx: index of io.
 *   @get: get cmd.
 *   @value: value of the io.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_get_io_one(struct iomux_desc *desc, u16_t io_idx, u8_t get, u8_t *value)
{
	int ret;

	if (desc == NULL || value == NULL || desc->iomux == NULL || desc->ops == NULL)
		return -EFAULT;

	if (io_idx >= desc->iomux->io_nr)
		return -ERANGE;
	
	if (desc->ops->get_one == NULL)
		return -ENOSUPPORT;

	ret = desc->ops->get_one(desc->iomux, io_idx, get, value);
	return ret;
}

/*************************************************************************************
 * Function Name: pl_iomux_set_io_grp
 * Description: set group of the io.
 *
 * Param:
 *   @desc: iomux description.
 *   @grp_idx: index of io group.
 *   @set: set cmd.
 *   @value: group value.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_set_io_grp(struct iomux_desc *desc, u16_t grp_idx, u8_t set, uintptr_t value)
{
	int ret;

	if (desc == NULL || desc->iomux == NULL || desc->ops == NULL)
		return -EFAULT;

	if (grp_idx >= desc->iomux->grp_nr)
		return -ERANGE;
	
	if (desc->ops->set_grp == NULL)
		return -ENOSUPPORT;

	ret = desc->ops->set_grp(desc->iomux, grp_idx, set, value);
	return ret;
}

/*************************************************************************************
 * Function Name: pl_iomux_get_io_grp
 * Description: get group of the io.
 *
 * Param:
 *   @desc: iomux description.
 *   @grp_idx: index of io group.
 *   @get: get cmd.
 *   @value: group value.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_iomux_get_io_grp(struct iomux_desc *desc, u16_t grp_idx, u8_t get, uintptr_t *value)
{
	int ret;

	if (desc == NULL || value == NULL || desc->iomux == NULL || desc->ops == NULL)
		return -EFAULT;

	if (grp_idx >= desc->iomux->grp_nr)
		return -ERANGE;
	
	if (desc->ops->get_grp == NULL)
		return -ENOSUPPORT;

	ret = desc->ops->get_grp(desc->iomux, grp_idx, get, value);
	return ret;
}

/*************************************************************************************
 * Function Name: pl_iomux_init
 * Description: iomux initialization.
 *
 * Param:
 *   none.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
static int pl_iomux_init(void)
{
	list_init(&pl_iomux_desc_list);
	pl_early_syslog_info("iomux init successfully\r\n");
	return OK;
}
pl_core_initcall(pl_iomux_init);
