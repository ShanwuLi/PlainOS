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
 * 结构体名称: gpio
 * 功能描述:
 *   该结构体用于表示通用输入输出（GPIO）的相关信息，常用于配置和控制硬件上的GPIO引脚。
 * 
 * 成员说明:
 *   @nr: IO编号，指示具体的GPIO引脚编号。
 *   @some: 特定用途标记，表示这些GPIO引脚的特殊功能或用途。
 *
 * 注意事项:
 *   - 组-1: GPIOA [0 0 0 ... 0]，表示GPIOA组的所有引脚状态默认为0（未激活或低电平）。
 *   - 组-2: GPIOB [0 0 0 ... 0]，同理，表示GPIOB组的所有引脚状态默认也为0。
 *   - ... 其他GPIO组可能存在，遵循类似的初始化模式或定义方式。
 ************************************************************************************/
struct gpio_grp {
	u16_t nr;
	void *some;
};

/*************************************************************************************
 * 结构体名称: gpio
 * 功能描述:
 *   该结构体用于描述GPIO组的信息，方便管理和操作具有相似特性和功能的一组GPIO引脚。
 * 
 * 成员变量说明:
 *   @grp_nr: 组编号，用来标识GPIO组的序号，有助于区分不同的GPIO组。
 *   @grp: GPIO组别，直接表示如GPIOA、GPIOB等具体的GPIO组，涉及硬件上的物理分区。
 *
 * 注意事项:
 *   - 组-1: GPIOA [0 0 0 ... 0]，示意GPIOA组中各引脚的初始状态或默认配置，每个'0'代表一个引脚，默认为未激活或低电平状态。
 *   - 组-2: GPIOB [0 0 0 ... 0]，同上，说明GPIOB组的引脚初始化情况，模式与GPIOA组相同。
 *   - ... 表示可能还存在更多组（如GPIOC, GPIOD等），均遵循类似的结构和初始化模式。
 ************************************************************************************/
struct gpio {
	u16_t io_nr;
	u16_t grp_nr;
	struct gpio_grp *grp;
};

/*************************************************************************************
 * 结构体名称: gpio_ops
 * 功能描述:
 *   该结构体定义了一组操作GPIO的函数指针，用于灵活地控制单个GPIO引脚或整个GPIO组的状态和读取其信息，
 *   支持对GPIO的高级抽象和编程接口封装。
 *
 * 成员函数说明:
 *   @set_one: 设置某个特定GPIO引脚的状态。允许用户配置单个引脚为高电平或低电平等状态。
 *   @get_one: 获取某个特定GPIO引脚的状态。用于读取单个引脚当前的实际电平状态（高电平或低电平）。
 *   @set_grp: 设置整个GPIO组的状态。此操作可同时配置一组GPIO引脚到相同的状态，适用于批量控制需求。
 *   @get_grp: 获取整个GPIO组的状态。获取一组GPIO引脚的当前状态集合，便于了解或监控多个引脚的状况。
 *
 ************************************************************************************/
struct gpio_ops {
	int (*set_one)(struct gpio *gpio, u16_t io_idx, u8_t set, u8_t value);
	int (*get_one)(struct gpio *gpio, u16_t io_idx, u8_t get, u8_t *value);
	int (*set_grp)(struct gpio *gpio, u16_t grp_idx, u8_t set, uintptr_t value);
	int (*get_grp)(struct gpio *gpio, u16_t grp_idx, u8_t get, uintptr_t *value);
};

struct gpio_desc {
	uint16_t no;
	const char *name;
	struct gpio *gpio;
	struct gpio_ops *ops;
	struct list_node node;
};

/*************************************************************************************
 * 函数名称: pl_gpio_desc_register
 * 功能描述:
 *   注册GPIO描述信息。此函数用于将GPIO的配置信息注册到系统中，以便后续的访问和管理GPIO资源.
 *
 * 参数:
 *   @desc: GPIO描述结构体，包含了关于GPIO的详细信息，如引脚编号、方向、初始状态等.
 *
 * 返回值:
 *   若注册成功，返回大于等于0的值；若注册失败，返回小于0的值.
 ************************************************************************************/
int pl_gpio_desc_register(struct gpio_desc *desc);

/*************************************************************************************
 * 函数名称: pl_gpio_desc_unregister
 * 功能描述:
 *   反注册GPIO描述信息。此函数用于从系统中移除之前注册的GPIO配置信息，释放相关资源，
 *   以避免资源泄露或冲突，常在不再需要使用特定GPIO配置时调用.
 *
 * 参数:
 *   @desc: 需要反注册的GPIO描述结构体，应与之前注册时使用的描述信息一致.
 *
 * 返回值:
 *   成功执行反注册操作，返回大于等于0的值；若操作失败，返回小于0的值.
 ************************************************************************************/
int pl_gpio_desc_unregister(struct gpio_desc *desc);

/*************************************************************************************
 * 函数名称: pl_gpio_desc_find_by_name
 * 功能描述:
 *   通过名称查找GPIO描述信息。此函数根据提供的GPIO名称，在已注册的GPIO描述中进行搜索，
 *   旨在获取与之匹配的GPIO详细配置信息.
 *
 * 参数:
 *   @name: GPIO的名称，作为查找的关键依据，应与注册时使用的名称一致.
 *
 * 返回值:
 *   查找到的GPIO描述结构体指针，若找到则返回对应的描述信息；若未找到匹配项，则返回NULL或具有特定含义的错误值.
 ************************************************************************************/
struct gpio_desc *pl_gpio_desc_find_by_name(const char *name);

/*************************************************************************************
 * 函数名称: pl_gpio_desc_find_by_no
 * 功能描述:
 *   通过编号查找GPIO描述信息。此函数依据给定的GPIO编号，在系统中检索相应的GPIO描述信息，
 *   用于获取指定GPIO的配置详情.
 *
 * 参数:
 *   @no: GPIO的编号，作为唯一标识符，用于直接定位到特定的GPIO描述.
 *
 * 返回值:
 *   匹配的GPIO描述结构体指针，如果根据编号找到相应的GPIO描述，则返回该描述信息；
 *   否则，可能返回NULL或错误代码指示未找到匹配项.
 ************************************************************************************/
struct gpio_desc *pl_gpio_desc_find_by_no(uint16_t no);

/*************************************************************************************
 * 函数名称: pl_gpio_set_io_one
 * 功能描述:
 *   设置单个GPIO引脚的状态。根据提供的GPIO描述及引脚索引，设定该引脚的具体值，实现对硬件GPIO的精确控制.
 *
 * 参数:
 *   @desc: GPIO的描述信息，包含了操作GPIO所需的上下文环境.
 *   @io_idx: 要设置的GPIO引脚的索引，指出目标引脚在GPIO组中的位置.
 *   @value: 要设置的GPIO引脚的值，一般为高电平(1)或低电平(0)，具体取决于应用需求.
 *
 * 返回值:
 *   成功设置GPIO引脚状态后，返回大于等于0的值；若设置过程中发生错误，则返回小于0的值.
 ************************************************************************************/
int pl_gpio_set_io_one(struct gpio_desc *desc, u16_t io_idx, u8_t set, u8_t value);

/*************************************************************************************
 * 函数名称: pl_gpio_get_io_one
 * 功能描述:
 *   读取单个GPIO引脚的状态。根据提供的GPIO描述及引脚索引，获取该引脚当前的值，用于诊断或监控.
 *
 * 参数:
 *   @desc: GPIO的描述信息，包含操作GPIO所需的上下文环境.
 *   @io_idx: 要读取的GPIO引脚的索引，指明目标引脚在GPIO组中的位置.
 *   @value: 输出参数，用于存储读取到的GPIO引脚的当前值.
 *
 * 返回值:
 *   成功读取GPIO引脚状态后，返回大于等于0的值；若读取过程中发生错误，则返回小于0的值.
 ************************************************************************************/
int pl_gpio_get_io_one(struct gpio_desc *desc, u16_t io_idx, u8_t get, u8_t *value);

/*************************************************************************************
 * 函数名称: pl_gpio_set_io_grp
 * 功能描述:
 *   设置整个GPIO组的状态。此函数允许一次性配置GPIO组内所有引脚的状态，提高批量操作的效率.
 *
 * 参数:
 *   @desc: GPIO的描述信息，提供了操作GPIO组的必要上下文.
 *   @grp_idx: 要设置的GPIO组的索引，用于定位到特定的GPIO组.
 *   @set: 设置命令，可能包含具体的操作类型或模式，如设置为输出、输入等.
 *   @value: 组值，表示要设置给整个GPIO组的状态值，可以是具体电平值或模式位.
 *
 * 返回值:
 *   成功设置GPIO组状态后，返回大于等于0的值；若设置失败，则返回小于0的错误代码.
 ************************************************************************************/
int pl_gpio_set_io_grp(struct gpio_desc *desc, u16_t grp_idx, u8_t set, uintptr_t value);

/*************************************************************************************
 * 函数名称: pl_gpio_get_io_grp
 * 功能描述:
 *   读取整个GPIO组的状态。此函数用于获取指定GPIO组内所有引脚的当前状态或值，便于进行状态监控或集体查询.
 *
 * 参数:
 *   @desc: GPIO的描述信息，提供必要的组操作上下文.
 *   @grp_idx: 要读取的GPIO组的索引，用于指向特定的GPIO组.
 *   @get: 获取命令，可能指定获取的具体类型，如读取电平状态、模式等.
 *   @value: 输出参数，用于存储读取到的GPIO组的整体状态或值.
 *
 * 返回值:
 *   成功读取GPIO组状态后，返回大于等于0的值；若读取失败，则返回小于0的错误代码.
 ************************************************************************************/
int pl_gpio_get_io_grp(struct gpio_desc *desc, u16_t grp_idx, u8_t get, uintptr_t *value);

#endif /* __DRV_GPIO_H__ */
