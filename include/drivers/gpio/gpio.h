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
#ifndef __DRV_GPIO_H__
#define __DRV_GPIO_H__

#include <types.h>
#include <kernel/list.h>

struct gpio {
	u16_t nr;
	u16_t *map;
	u32_t *value;
	u32_t *dirction;
};

struct gpio_ops {
	int (*set_direction)(struct gpio *gpio, u16_t gpio_idx, u8_t direction);
	int (*get_direction)(struct gpio *gpio, u16_t gpio_idx, u8_t *direction);
	int (*set_value)(struct gpio *gpio, u16_t gpio_idx, u8_t value);
	int (*get_value)(struct gpio *gpio, u16_t gpio_idx, u8_t *value);
};

struct gpio_desc {
	const char *name;
	struct gpio *gpios;
	struct gpio_ops *ops;
	struct list_node node;
};

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
int pl_gpio_desc_register(struct gpio_desc *desc);

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
int pl_gpio_desc_unregister(struct gpio_desc *desc);

#endif /* __DRV_GPIO_H__ */
