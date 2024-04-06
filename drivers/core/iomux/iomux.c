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
 * Function Name: pl_iomux_desc_find_by_name
 * Description: find iomux description
 *
 * Param:
 *   @name: iomux description name.
 *
 * Return:
 *   iomux description.
 ************************************************************************************/
struct iomux_desc *pl_iomux_desc_find_by_name(const char *name)
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
 * Function Name: pl_iomux_desc_find_by_name
 * Description: find iomux description
 *
 * Param:
 *   @id: iomux description id.
 *
 * Return:
 *   iomux description.
 ************************************************************************************/
struct iomux_desc *pl_iomux_desc_find_by_id(u16_t id)
{
	struct iomux_desc *pos;

	if (list_is_empty(&pl_iomux_desc_list))
		return NULL;

	list_for_each_entry(pos, &pl_iomux_desc_list, struct iomux_desc, node) {
		if (pos->id == id) {
			return pos;
		}
	}

	return NULL;
}

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
int pl_iomux_set_pin_function(struct iomux_desc *desc, u16_t io_idx, u8_t function)
{
	int ret;
	struct iomux *mux;

	if (desc == NULL || io_idx >= desc->ios_nr)
		return -EFAULT;

	mux = &desc->ios[io_idx];
	ret = desc->ops->set_function(mux, function);

	return ret;
}

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
int pl_iomux_get_pin_function(struct iomux_desc *desc, u16_t io_idx, u8_t *function)
{
	int ret;
	struct iomux *mux;

	if (function == NULL || desc == NULL || io_idx >= desc->ios_nr)
		return -EFAULT;

	mux = &desc->ios[io_idx];
	ret = desc->ops->get_function(mux, function);

	return ret;
}

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
int pl_iomux_set_pin_state(struct iomux_desc *desc, u16_t io_idx, u8_t state)
{
	int ret;
	struct iomux *mux;

	if (desc == NULL || io_idx >= desc->ios_nr)
		return -EFAULT;

	mux = &desc->ios[io_idx];
	ret = desc->ops->set_state(mux, state);

	return ret;
}

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
int pl_iomux_get_pin_state(struct iomux_desc *desc, u16_t io_idx, u8_t *state)
{
	int ret;
	struct iomux *mux;

	if (state == NULL || desc == NULL || io_idx >= desc->ios_nr)
		return -EFAULT;

	mux = &desc->ios[io_idx];
	ret = desc->ops->get_state(mux, state);

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
