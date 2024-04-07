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
 * Function Name: pl_iomux_set_io_fun
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
int pl_iomux_set_io_fun(struct iomux_desc *desc, u16_t io_idx, u8_t fun)
{
	int ret;
	u16_t idx = io_idx;

	if (desc == NULL || desc->iomux == NULL)
		return -EFAULT;

	if (io_idx >= desc->iomux->nr)
		return -ERANGE;

	ret = desc->ops->set_fun(desc->iomux, idx, fun);
	return ret;
}

/*************************************************************************************
 * Function Name: pl_iomux_get_io_fun
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
int pl_iomux_get_io_fun(struct iomux_desc *desc, u16_t io_idx, u8_t *fun)
{
	int ret;
	u16_t idx = io_idx;

	if (desc == NULL || desc->iomux == NULL)
		return -EFAULT;

	if (io_idx >= desc->iomux->nr)
		return -ERANGE;

	ret = desc->ops->get_fun(desc->iomux, idx, fun);
	return ret;
}

/*************************************************************************************
 * Function Name: pl_iomux_set_io_drv
 * Description: set driver strength for io.
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
	int ret;
	u16_t idx = io_idx;

	if (desc == NULL || desc->iomux == NULL)
		return -EFAULT;

	if (io_idx >= desc->iomux->nr)
		return -ERANGE;

	ret = desc->ops->set_drv(desc->iomux, idx, drv);
	return ret;
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
	int ret;
	u16_t idx = io_idx;

	if (desc == NULL || desc->iomux == NULL)
		return -EFAULT;

	if (io_idx >= desc->iomux->nr)
		return -ERANGE;

	ret = desc->ops->get_drv(desc->iomux, idx, drv);
	return ret;
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
	int ret;
	u16_t idx = io_idx;

	if (desc == NULL || desc->iomux == NULL)
		return -EFAULT;

	if (io_idx >= desc->iomux->nr)
		return -ERANGE;

	ret = desc->ops->set_state(desc->iomux, idx, state);
	return ret;
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
	int ret;
	u16_t idx = io_idx;

	if (desc == NULL || desc->iomux == NULL)
		return -EFAULT;

	if (io_idx >= desc->iomux->nr)
		return -ERANGE;

	ret = desc->ops->get_state(desc->iomux, idx, state);
	return ret;
}

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
int pl_iomux_set_io_one(struct iomux_desc *desc, u16_t io_idx, u8_t set, u8_t one)
{
	int ret;
	u16_t idx = io_idx;

	if (desc == NULL || desc->iomux == NULL)
		return -EFAULT;

	if (io_idx >= desc->iomux->nr)
		return -ERANGE;

	ret = desc->ops->set_one(desc->iomux, idx, set, one);
	return ret;
}

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
int pl_iomux_get_io_one(struct iomux_desc *desc, u16_t io_idx, u8_t get, u8_t *one)
{
	int ret;
	u16_t idx = io_idx;

	if (desc == NULL || desc->iomux == NULL)
		return -EFAULT;

	if (io_idx >= desc->iomux->nr)
		return -ERANGE;

	ret = desc->ops->get_one(desc->iomux, idx, get, one);
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
core_initcall(pl_iomux_init);
