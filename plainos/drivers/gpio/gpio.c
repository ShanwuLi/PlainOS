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

/**
 * pl_gpio_desc_register - 注册一个GPIO描述符
 * @desc: 指向待注册的GPIO描述符的指针
 *
 * 该函数用于在系统中注册一个GPIO描述符。注册成功返回OK，失败返回相应的错误码。
 * 
 * 参数desc是一个结构体指针，指向待注册的GPIO描述符。函数首先检查desc指针是否为空，
 * 如果为空，则返回-EFAULT错误码，表示无效的参数。
 * 
 * 接着，函数检查desc指向的结构体中的name、gpio和ops字段是否为空。这些字段是GPIO描述符的重要组成部分，
 * 如果任一字段为空，则返回-EACCES错误码，表示访问被拒绝。
 * 
 * 最后，如果所有检查都通过，函数将新注册的GPIO描述符添加到系统的GPIO描述符列表的尾部，
 * 然后返回OK，表示注册成功。
 */
int pl_gpio_desc_register(struct gpio_desc *desc)
{
    /* 检查desc指针是否为空 */
    if (desc == NULL)
        return -EFAULT;

    /* 检查desc的子字段是否为空 */
    if (desc->name == NULL || desc->gpio == NULL || desc->ops == NULL)
        return -EACCES;

    /* 将新的GPIO描述符添加到列表的尾部 */
    list_add_node_at_tail(&pl_gpio_desc_list, &desc->node);
    return OK;
}

/**
 * 解除注册GPIO描述符
 * 
 * 本函数用于从全局GPIO描述符列表中解除注册一个特定的GPIO描述符。它遍历列表，找到匹配的描述符后将其从列表中删除。
 * 
 * @param desc 指向待解除注册的GPIO描述符的指针。
 * @return 如果成功找到并删除了描述符，则返回OK；如果desc为NULL，返回-EFAULT；如果找不到匹配的描述符，返回-ENODEV。
 */
int pl_gpio_desc_unregister(struct gpio_desc *desc)
{
    struct gpio_desc *pos;  /* 列表遍历的当前位置 */
    struct gpio_desc *tmp;  /* 列表遍历的临时指针，用于安全的列表操作 */

    /* 检查待解除注册的描述符是否为空，如果为空则返回错误 */
    if (desc == NULL)
        return -EFAULT;

    /* 安全地遍历全局GPIO描述符列表 */
    list_for_each_entry_safe(pos, tmp, &pl_gpio_desc_list, struct gpio_desc, node) {
        /* 比较当前遍历到的描述符和待解除注册的描述符的名称 */
        if (pos->name == desc->name) {
            /* 如果名称匹配，则从列表中删除该描述符 */
            list_del_node(&desc->node);
            return OK;
        }
    }

    /* 如果遍历完整个列表都没有找到匹配的描述符，则返回错误 */
    return -ENODEV;
}

/**
 * 根据名称查找GPIO描述符
 * 
 * @param name 需要查找的GPIO描述符的名称
 * @return 返回匹配的GPIO描述符指针，如果没有找到则返回NULL
 * 
 * 该函数遍历gpio_desc_list链表，比较每个描述符的名称与输入的名称。
 * 如果找到匹配的名称，则返回该描述符的指针。如果链表为空或输入名称为NULL，
 * 函数直接返回NULL。如果链表中没有匹配的描述符，函数最终也返回NULL。
 */
struct gpio_desc *pl_gpio_desc_find_by_name(const char *name)
{
    struct gpio_desc *pos;

    /* 检查链表是否为空或输入名称是否为NULL */
    if (list_is_empty(&pl_gpio_desc_list) || name == NULL)
        return NULL;

    /* 遍历链表，寻找匹配名称的GPIO描述符 */
    list_for_each_entry(pos, &pl_gpio_desc_list, struct gpio_desc, node) {
        /* 比较描述符的名称与输入名称是否相同 */
        if (strcmp(pos->name, name) == 0) {
            /* 如果相同，返回当前描述符指针 */
            return pos;
        }
    }

    /* 如果没有找到匹配的描述符，返回NULL */
    return NULL;
}

/**
 * 根据GPIO编号查找并返回对应的GPIO描述符。
 * 
 * @param no 要查找的GPIO编号。
 * @return 如果找到匹配的GPIO描述符，则返回指向该描述符的指针；如果列表为空或未找到匹配项，则返回NULL。
 */
struct gpio_desc *pl_gpio_desc_find_by_no(uint16_t no)
{
    struct gpio_desc *pos;

    /* 检查GPIO描述符列表是否为空，如果为空则直接返回NULL */
    if (list_is_empty(&pl_gpio_desc_list))
        return NULL;

    /* 遍历GPIO描述符列表，寻找编号匹配的描述符 */
    list_for_each_entry(pos, &pl_gpio_desc_list, struct gpio_desc, node) {
        /* 如果找到匹配的描述符，则返回该描述符的指针 */
        if (no == pos->no) {
            return pos;
        }
    }

    /* 如果遍历完列表仍未找到匹配的描述符，则返回NULL */
    return NULL;
}

/**
 * @brief 设置GPIO口的指定引脚为高电平或低电平
 * 
 * 该函数用于通过提供的GPIO描述符，设置指定GPIO口的某个引脚为高电平或低电平。
 * 它首先验证描述符及其相关指针的有效性，然后检查引脚索引是否在有效范围内。
 * 最后，如果设置了操作函数指针，它将调用该函数来实际设置引脚的电平。
 * 
 * @param desc GPIO描述符，包含GPIO口和操作函数指针等信息
 * @param io_idx 引脚索引，表示要设置的引脚在GPIO口中的位置
 * @param set 设置操作的标志，通常用于决定是设置引脚为高电平还是低电平
 * @param value 要设置的电平值，通常为0（低电平）或1（高电平）
 * @return int 返回操作的结果，成功则返回0，否则返回错误码
 */
int pl_gpio_set_io_one(struct gpio_desc *desc, u16_t io_idx, u8_t set, u8_t value)
{
    int ret;
	/* 检查描述符及其组件的有效性 */
	if (desc == NULL || desc->gpio == NULL || desc->ops == NULL)
		return -EFAULT;

	/* 检查引脚索引是否在有效范围内 */
	if (io_idx >= desc->gpio->io_nr)
		return -ERANGE;

	/* 检查是否提供了设置单个引脚的操作函数 */
	if (desc->ops->set_one == NULL)
		return -ENOSUPPORT;

	/* 调用操作函数来设置引脚电平，并返回操作结果 */
	ret = desc->ops->set_one(desc->gpio, io_idx, set, value);
	return ret;
}

/**
 * @brief 从GPIO描述符获取单个IO值
 * 
 * 该函数通过提供的GPIO描述符和IO索引，从指定的GPIO控制器中获取指定IO口的值。
 * 如果需要，可以设置IO口的值。函数返回操作的结果。
 * 
 * @param desc 指向GPIO描述符的指针，描述了GPIO控制器和操作。
 * @param io_idx IO索引，指定要操作的GPIO口。
 * @param get 指示是否需要获取（读取）IO口的值。
 * @param value 指向用于存储IO口值的变量的指针。
 * @return int 返回操作的结果。成功返回0，错误返回负数错误码。
 */
int pl_gpio_get_io_one(struct gpio_desc *desc, u16_t io_idx, u8_t get, u8_t *value)
{
    int ret;
	/* 检查参数是否有效，防止空指针引用 */
	if (desc == NULL || value == NULL || desc->gpio == NULL || desc->ops == NULL)
		return -EFAULT;

	/* 检查IO索引是否超出范围 */
	if (io_idx >= desc->gpio->io_nr)
		return -ERANGE;

	/* 检查是否支持单个IO获取操作 */
	if (desc->ops->get_one == NULL)
		return -ENOSUPPORT;

	/* 调用操作函数获取单个IO的值 */
	ret = desc->ops->get_one(desc->gpio, io_idx, get, value);
	/* 返回操作结果 */
	return ret;
}

/**
 * @brief 设置GPIO组的输出值
 * 
 * 该函数用于通过提供的GPIO描述符设置指定GPIO组的输出状态。它首先验证描述符及其组件的有效性，
 * 然后检查指定的组索引是否在有效范围内。如果一切有效，它将调用相应的操作函数来设置GPIO组的值。
 * 
 * @param desc GPIO描述符，包含GPIO信息和操作函数指针
 * @param grp_idx GPIO组索引，用于指定要设置的组
 * @param set 设置标志，决定是设置还是清除GPIO组的输出值
 * @param value 要设置的值，具体含义取决于GPIO控制器的实现
 * @return int 返回操作的结果，0表示成功，负值表示错误
 */
int pl_gpio_set_io_grp(struct gpio_desc *desc, u16_t grp_idx, u8_t set, uintptr_t value)
{
    int ret;
	/* 检查描述符及其组件是否为空，防止无效访问 */
	if (desc == NULL || desc->gpio == NULL || desc->ops == NULL)
		return -EFAULT;

	/* 检查指定的组索引是否超出范围 */
	if (grp_idx >= desc->gpio->grp_nr)
		return -ERANGE;
	
	/* 检查是否支持设置GPIO组的功能 */
	if (desc->ops->set_grp == NULL)
		return -ENOSUPPORT;

	/* 调用操作函数来设置GPIO组的值 */
	ret = desc->ops->set_grp(desc->gpio, grp_idx, set, value);
	/* 返回操作结果 */
	return ret;
}

/**
 * @brief 获取GPIO组的值
 * 
 * 该函数用于根据提供的GPIO描述符获取指定GPIO组的值。它首先验证输入参数的有效性，
 * 然后检查所请求的组索引是否在有效范围内。如果一切有效，它将调用操作函数指针来
 * 实际获取组的值。
 * 
 * @param desc GPIO描述符，包含GPIO信息和操作函数指针
 * @param grp_idx GPIO组索引
 * @param get 指示是否获取组的值，1为获取，0为其他操作（如设置值）
 * @param value 指向用于存储组值的变量的指针
 * @return int 返回操作的结果，0表示成功，负值表示错误
 */
int pl_gpio_get_io_grp(struct gpio_desc *desc, u16_t grp_idx, u8_t get, uintptr_t *value)
{
    int ret;
	/* 检查输入参数是否有效 */
	if (desc == NULL || value == NULL || desc->gpio == NULL || desc->ops == NULL)
		return -EFAULT;

	/* 检查请求的组索引是否在有效范围内 */
	if (grp_idx >= desc->gpio->grp_nr)
		return -ERANGE;

	/* 检查是否支持获取组的操作 */
	if (desc->ops->get_grp == NULL)
		return -ENOSUPPORT;

	/* 调用操作函数指针来获取组的值 */
	ret = desc->ops->get_grp(desc->gpio, grp_idx, get, value);
	/* 返回操作结果 */
	return ret;
}

/**
 * @brief 初始化GPIO描述符列表
 * 
 * 该函数负责初始化GPIO描述符列表，为后续的GPIO操作做好准备。
 * 它是GPIO核心初始化的一部分，确保在使用GPIO功能前，相关的数据结构和状态被正确设置。
 * 
 * @return int 返回状态码，OK表示初始化成功。
 */
static int pl_gpio_init(void)
{
    /* 初始化GPIO描述符列表的链接结构 */
    list_init(&pl_gpio_desc_list);

    /* 通过日志系统记录GPIO核心初始化完成的信息 */
    pl_early_syslog_info("gpio core init done\r\n");

    /* 返回初始化成功的状态码 */
    return OK;
}
pl_core_initcall(pl_gpio_init);
