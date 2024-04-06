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
	u8_t value;
	u8_t dirction;
};

struct gpio_ops {
	int (*set_direction)(struct gpio *gpio, u8_t direction);
	int (*get_direction)(struct gpio *gpio, u8_t *direction);
	int (*set_value)(struct gpio *gpio, u8_t value);
	int (*get_value)(struct gpio *gpio, u8_t *value);
};

struct gpio_desc {
	const char *name;
	u16_t id;
	u16_t gpio_nr;
	u8_t *gpio;
	struct gpio_ops *ops;
	struct list_node node;
};




#endif /* __DRV_GPIO_H__ */