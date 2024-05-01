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

enum {
	GPIO_DIRECT_INPUT = 0,
	GPIO_DIRECT_OUTPUT,
};

enum {
	GPIO_SET_DIRECT = 0,
	GPIO_SET_VALUE,
	GPIO_SET_PULL_DOWN,
	GPIO_SET_SPEED,
	GPIO_SET_SAMPLE_RATE,
	GPIO_SET_INTR_TRIG,
	GPIO_SET_INTR_MASK,
	GPIO_SET_END
};

enum {
	GPIO_GET_DIRECT = 0,
	GPIO_GET_VALUE,
	GPIO_GET_PULL_DOWN,
	GPIO_GET_SPEED,
	GPIO_GET_SAMPLE_RATE,
	GPIO_GET_INTR_TRIG,
	GPIO_GET_INTR_MASK,
	GPIO_GET_END
};


/*************************************************************************************
 * struct gpio:
 * Description:
 *   @nr: number of the io.
 *   @some: special purpose for these ioes.
 *
 * NOTE:
 *   group-1:  GPIOA [0 0 0 ... 0]
 *   group-2:  GPIOB [0 0 0 ... 0]
 *   ...
 ************************************************************************************/
struct gpio_grp {
	u16_t nr;
	void *some;
};

/*************************************************************************************
 * struct gpio:
 * Description:
 *   @grp_nr: number of the group.
 *   @grp: gpio group.
 *
 * NOTE:
 *   group-1:  GPIOA [0 0 0 ... 0]
 *   group-2:  GPIOB [0 0 0 ... 0]
 *   ...
 ************************************************************************************/
struct gpio {
	u16_t io_nr;
	u16_t grp_nr;
	struct gpio_grp *grp;
};

/*************************************************************************************
 * struct gpio_ops:
 * Description:
 *   @set_one: set one of the some.
 *   @get_one: get one of the some.
 *   @set_grp: set group of the some.
 *   @get_grp: get group of the some.
 *
 ************************************************************************************/
struct gpio_ops {
	int (*set_one)(struct gpio *gpio, u16_t io_idx, u8_t set, u8_t value);
	int (*get_one)(struct gpio *gpio, u16_t io_idx, u8_t get, u8_t *value);
	int (*set_grp)(struct gpio *gpio, u16_t grp_idx, u8_t set, uintptr_t value);
	int (*get_grp)(struct gpio *gpio, u16_t grp_idx, u8_t get, uintptr_t *value);
};

struct gpio_desc {
	const char *name;
	struct gpio *gpio;
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
struct gpio_desc *pl_gpio_desc_find(const char *name);

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
int pl_gpio_set_io_one(struct gpio_desc *desc, u16_t io_idx, u8_t set, u8_t value);

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
int pl_gpio_get_io_one(struct gpio_desc *desc, u16_t io_idx, u8_t get, u8_t *value);

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
int pl_gpio_set_io_grp(struct gpio_desc *desc, u16_t grp_idx, u8_t set, uintptr_t value);

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
int pl_gpio_get_io_grp(struct gpio_desc *desc, u16_t grp_idx, u8_t get, uintptr_t *value);

#endif /* __DRV_GPIO_H__ */
