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

#include <types.h>
#include <errno.h>
#include <string.h>
#include <kernel/list.h>
#include <kernel/initcall.h>
#include <kernel/syslog.h>
#include <drivers/gpio/gpio.h>

static struct list_node pl_gpio_desc_list;

/*************************************************************************************
 * Function Name: pl_gpio_desc_register
 * Description: register gpio description
 *
 * Param:
 *   @desc: gpio description.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_gpio_desc_register(struct gpio_desc *desc)
{
	if (desc == NULL)
		return -EFAULT;

	if (desc->name == NULL || desc->gpio == NULL || desc->ops == NULL)
		return -EACCES;

	list_add_node_at_tail(&pl_gpio_desc_list, &desc->node);
	return OK;
}

/*************************************************************************************
 * Function Name: pl_gpio_desc_unregister
 * Description: unregister gpio description
 *
 * Param:
 *   @desc: gpio description.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_gpio_desc_unregister(struct gpio_desc *desc)
{
	if (desc == NULL)
		return -EFAULT;

	list_del_node(&desc->node);
	return OK;
}

/*************************************************************************************
 * Function Name: pl_gpio_desc_find
 * Description: find gpio description
 *
 * Param:
 *   @name: gpio description name.
 *
 * Return:
 *   gpio description.
 ************************************************************************************/
struct gpio_desc *pl_gpio_desc_find(const char *name)
{
	struct gpio_desc *pos;

	if (list_is_empty(&pl_gpio_desc_list) || name == NULL)
		return NULL;

	list_for_each_entry(pos, &pl_gpio_desc_list, struct gpio_desc, node) {
		if (strcmp(pos->name, name) == 0) {
			return pos;
		}
	}

	return NULL;
}

/*************************************************************************************
 * Function Name: pl_gpio_set_io_one
 * Description: set one of the io.
 *
 * Param:
 *   @desc: gpio description.
 *   @io_idx: index of io.
 *   @value: value of the io.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_gpio_set_io_one(struct gpio_desc *desc, u16_t io_idx, u8_t set, u8_t value)
{
	int ret;

	if (desc == NULL || desc->gpio == NULL)
		return -EFAULT;

	if (io_idx >= desc->gpio->io_nr)
		return -ERANGE;

	ret = desc->ops->set_one(desc->gpio, io_idx, set, value);
	return ret;
}

/*************************************************************************************
 * Function Name: pl_gpio_get_io_one
 * Description: get one of the io.
 *
 * Param:
 *   @desc: gpio description.
 *   @io_idx: index of io.
 *   @value: value of the io.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_gpio_get_io_one(struct gpio_desc *desc, u16_t io_idx, u8_t get, u8_t *value)
{
	int ret;

	if (desc == NULL || desc->gpio == NULL || value == NULL)
		return -EFAULT;

	if (io_idx >= desc->gpio->io_nr)
		return -ERANGE;

	ret = desc->ops->get_one(desc->gpio, io_idx, get, value);
	return ret;
}

/*************************************************************************************
 * Function Name: pl_gpio_set_io_grp
 * Description: set group of the io.
 *
 * Param:
 *   @desc: gpio description.
 *   @grp_idx: index of io group.
 *   @set: set cmd.
 *   @value: group value.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_gpio_set_io_grp(struct gpio_desc *desc, u16_t grp_idx, u8_t set, uintptr_t value)
{
	int ret;

	if (desc == NULL || desc->gpio == NULL)
		return -EFAULT;

	if (grp_idx >= desc->gpio->grp_nr)
		return -ERANGE;

	ret = desc->ops->set_grp(desc->gpio, grp_idx, set, value);
	return ret;
}

/*************************************************************************************
 * Function Name: pl_gpio_get_io_grp
 * Description: get group of the io.
 *
 * Param:
 *   @desc: gpio description.
 *   @grp_idx: index of io group.
 *   @get: get cmd.
 *   @value: group value.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
int pl_gpio_get_io_grp(struct gpio_desc *desc, u16_t grp_idx, u8_t get, uintptr_t *value)
{
	int ret;

	if (desc == NULL || desc->gpio == NULL || value == NULL)
		return -EFAULT;

	if (grp_idx >= desc->gpio->grp_nr)
		return -ERANGE;

	ret = desc->ops->get_grp(desc->gpio, grp_idx, get, value);
	return ret;
}

/*************************************************************************************
 * Function Name: pl_gpio_init
 * Description: gpio initialization.
 *
 * Param:
 *   none.
 *
 * Return:
 *   Greater than or equal to 0 on success, less than 0 on failure.
 ************************************************************************************/
static int pl_gpio_init(void)
{
	list_init(&pl_gpio_desc_list);
	pl_early_syslog_info("gpio init successfully\r\n");
	return OK;
}
pl_core_initcall(pl_gpio_init);
