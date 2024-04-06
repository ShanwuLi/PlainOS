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

	if (desc->name == NULL || desc->gpios == NULL || desc->ops == NULL)
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


