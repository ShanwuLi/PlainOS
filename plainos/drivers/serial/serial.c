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
#include <kernel/kernel.h>
#include <kernel/list.h>
#include <kernel/mempool.h>
#include <kernel/initcall.h>
#include <kernel/syslog.h>
#include <drivers/serial/serial.h>

static struct list_node pl_serial_desc_list;

/**
 * @brief 串口接收回调工作函数
 *
 * 此函数是处理串口接收回调的工作项执行体，用于将接收回调的执行上下文与原始接收上下文解耦，
 * 允许在更灵活的环境中执行接收回调，如在工作线程中。
 *
 * @param work 工作结构指针。此结构用于在工作队列中管理工作项。
 *             通过此参数，函数可以获取串口接收回调的具体上下文。
 */
static void pl_serial_recv_cb_work_func(struct pl_work *work)
{
    /* 将工作结构转换为串口接收信息结构 */
    /* 假定工作结构位于串口接收信息结构的开头 */
    /* 接收回调函数不为空 */
    struct pl_serial_recv_info *recv_info;

    /* 从工作结构中获取串口接收信息结构指针 */
    recv_info = container_of(work, struct pl_serial_recv_info, cb_work);

    /* 执行接收回调函数 */
    /* 回调函数负责处理接收到的数据或执行其他必要的操作 */
    recv_info->callback(&recv_info->fifo);
}

/**
 * @brief 初始化串行描述符
 * 
 * 该函数用于初始化串行通信的描述符，配置描述符的基本信息，如端口号、操作函数指针等。
 * 这些配置为后续的串行通信提供了必要的参数和操作接口。
 * 
 * @param desc 串行通信描述符指针，用于存储配置信息。
 * @param port 端口号，标识串行通信的物理接口。
 * @param ops 操作函数指针结构体，包含串行通信的各种操作函数。
 */
static void serial_desc_init(struct pl_serial_desc *desc, u8_t port,
                             struct pl_serial_ops *ops)
{
    desc->port = port;       /* 设置串行通信的端口号 */
    desc->ops = ops;         /* 设置串行通信的操作函数指针 */
    desc->recv_info.callback = NULL; /* 初始化接收回调函数指针为NULL */
    desc->recv_info.process = NULL;  /* 初始化接收处理函数指针为NULL */
    list_init(&desc->node);        /* 初始化描述符在列表中的节点 */
}

/**
 * 初始化串行描述符
 * 
 * 该函数用于初始化一个串行通信的描述符，包括设置端口、操作函数指针、接收缓冲区以及相关的同步和队列机制。
 * 
 * @param desc 串行描述符指针，不能为空。
 * @param port 端口号，用于标识具体的串行通信端口。
 * @param ops 操作函数指针结构体，包含串行通信的各种操作函数。
 * @param recv_buffer 接收缓冲区指针，用于存放接收的串行数据，不能为空。
 * @param recv_buff_size 接收缓冲区大小，指定接收缓冲区可以存放的数据量。
 * 
 * @return 返回初始化结果，OK表示成功，-EFAULT表示参数错误，其他负值表示初始化某个部分失败。
 */
int pl_serial_desc_init(struct pl_serial_desc *desc, u8_t port,
                        struct pl_serial_ops *ops,
                        char *recv_buffer, uint_t recv_buff_size)
{
	int ret;
	// 检查参数是否有效，防止空指针导致的错误。
	if (desc == NULL || recv_buffer == NULL)
		return -EFAULT;

	// 初始化互斥锁，用于串行通信的同步控制。
	ret = pl_semaphore_init(&desc->sem, 1);
	if (ret < 0)
		return ret;

	// 初始化工作队列，用于在后台线程中处理接收的数据。
	ret = pl_work_init(&desc->recv_info.cb_work, pl_serial_recv_cb_work_func, desc);
	if (ret < 0)
		return ret;

	// 初始化FIFO队列，用于存放待处理的接收数据。
	ret = pl_kfifo_init(&desc->recv_info.fifo, recv_buffer, recv_buff_size);
	if (ret < 0)
		return ret;

	// 调用具体的串行描述符初始化函数，设置端口和操作函数。
	serial_desc_init(desc, port, ops);
	return OK;
}

/**
 * 注册一个串行设备描述符
 * 
 * 该函数用于在系统中注册一个串行设备描述符。注册过程中，会检查描述符是否为空以及其操作是否为空。
 * 如果描述符或其操作为空，则返回相应的错误码。如果一切检查通过，则将该描述符添加到串行设备的描述符列表中。
 * 
 * @param desc 指向要注册的串行设备描述符的指针。
 * @return 如果注册成功，返回OK；如果desc为空，返回-EFAULT；如果desc的操作为空，返回-EACCES。
 */
int pl_serial_desc_register(struct pl_serial_desc *desc)
{
    /* 检查描述符指针是否为空 */
    if (desc == NULL)
        return -EFAULT;

    /* 检查描述符的操作指针是否为空 */
    if (desc->ops == NULL)
        return -EACCES;

    /* 将新描述符添加到串行设备描述符列表的尾部 */
    list_add_node_at_tail(&pl_serial_desc_list, &desc->node);
    return OK;
}

/**
 * pl_serial_callee_recv_handler - 处理串行接收字符
 * @desc: 指向串口描述结构的指针
 * @chars: 用于接收字符的缓冲区指针
 * @chars_len: 字符缓冲区长度
 * 
 * 此函数用于处理串行接收到的字符。首先检查是否设置了处理函数，
 * 如已设置，则调用该函数处理字符。如果处理失败或设置了回调函数，
 * 则可能安排回调工作在高优先级工作队列上执行。
 * 
 * 返回值：
 *   - EFAULT：描述指针无效
 *   - OK：处理成功或未设置处理函数
 *   - 添加工作到工作队列的结果：如果调度了回调
 */
int pl_serial_callee_recv_handler(struct pl_serial_desc *desc,
                                char *chars, uint_t chars_len)
{
	int ret;
	pl_serial_recv_process_t process;

	/* 检查描述指针是否为NULL */
	if (desc == NULL)
		return -EFAULT;

	/* 获取字符处理函数 */
	/* 设置接收字符的处理流程 */
	process = desc->recv_info.process;
	if (process != NULL) {
		/* 调用字符处理函数 */
		if (process(&desc->recv_info.fifo, chars, chars_len) == 0)
			return OK;

		/* 若处理后存在回调函数 */
		/* 检查回调调用 */
		if (desc->recv_info.callback == NULL)
			return OK;

		/* 安排回调工作执行 */
		/* 调用回调 */
		ret = pl_work_add(g_pl_sys_hiwq_handle, &desc->recv_info.cb_work);
		return ret;
	}

	/* 如果未设置字符处理函数，返回OK */
	return OK;
}

/**
 * 查找并返回指定端口的串行描述符
 * 
 * @param port 要查找的串行端口号
 * @return 指向找到的串行描述符的指针，如果没有找到则返回NULL
 */
struct pl_serial_desc *pl_serial_find_desc(u8_t port)
{
    struct pl_serial_desc *pos;

    /* 如果串行描述符列表为空，则直接返回NULL */
    if (list_is_empty(&pl_serial_desc_list))
        return NULL;

    /* 遍历串行描述符列表，查找匹配端口号的描述符 */
    list_for_each_entry(pos, &pl_serial_desc_list, struct pl_serial_desc, node) {
        /* 如果找到匹配的端口号，则返回该描述符 */
        if (port == pos->port)
            return pos;
    }

    /* 如果遍历完列表都没有找到匹配的描述符，则返回NULL */
    return NULL;
}

/**
 * 设置串行通信的波特率
 * 
 * 本函数用于通过提供的描述符设置串行通信设备的波特率。它首先检查描述符是否有效，
 * 然后确保描述符的操作集合中包含了设置波特率的操作。如果一切就绪，它会获取一个互斥锁，
 * 调用实际的设置函数，然后释放互斥锁。这个函数在多线程环境下确保了对设备设置的线程安全。
 * 
 * @param desc 指向pl_serial_desc结构的指针，包含了串行设备的相关描述。
 * @param baud_rate 要设置的波特率。
 * @return 返回设置波特率的操作结果。如果desc为空，返回-EFAULT；如果操作集合不包含设置波特率的操作，
 *         返回-EACCES；否则返回实际设置操作的结果。
 */
int pl_serial_set_baud_rate(struct pl_serial_desc *desc, u32_t baud_rate)
{
	int ret;

	// 检查描述符是否为空，如果为空，则返回错误码-EFAULT
	if (desc == NULL)
		return -EFAULT;

	// 检查描述符的操作集合是否为空，或者是否不包含设置波特率的操作，
	// 如果是，则返回错误码-EACCES
	if (desc->ops == NULL || desc->ops->set_baud_rate == NULL)
		return -EACCES;

	// 获取互斥锁，以确保对设备的线程安全访问
	pl_semaphore_wait(&desc->sem);
	// 调用实际的设置波特率操作，并将结果保存在ret中
	ret = desc->ops->set_baud_rate(desc, baud_rate);
	// 释放互斥锁
	pl_semaphore_post(&desc->sem);
	// 返回设置波特率操作的结果
	return ret;
}

/**
 * 设置串口数据位数
 * 
 * 本函数用于设置串口通信的数据位数。它首先检查描述符是否为空，然后检查操作集合和具体的数据位设置函数是否可用。
 * 如果一切就绪，它会获取一个信号量以确保串口操作的互斥，然后调用具体的操作函数进行设置，并在操作完成后释放信号量。
 * 
 * @param desc 串口设备描述符，包含操作集合和状态信息。
 * @param data_bits 要设置的数据位数。
 * @return 设置操作的结果。如果返回负值，则表示操作失败。
 */
int pl_serial_set_data_bits(struct pl_serial_desc *desc, u8_t data_bits)
{
	int ret;

	// 检查描述符是否为空，如果为空，则返回错误码 -EFAULT。
	if (desc == NULL)
		return -EFAULT;

	// 检查操作集合和设置数据位的操作是否可用，如果不可用，则返回错误码 -EACCES。
	if (desc->ops == NULL || desc->ops->set_data_bits == NULL)
		return -EACCES;

	// 获取信号量，以确保串口操作的互斥。
	pl_semaphore_wait(&desc->sem);
	// 调用具体的操作函数来设置数据位数。
	ret = desc->ops->set_data_bits(desc, data_bits);
	// 释放信号量，允许其他操作继续。
	pl_semaphore_post(&desc->sem);
	// 返回设置操作的结果。
	return ret;
}

/**
 * 设置串口的校验位
 * 
 * 本函数用于通过操作描述符来设置串口设备的校验位。这需要设备的操作接口中存在相应的设置函数。
 * 
 * @param desc 串口设备的描述符，包含设备的操作集合和同步机制。
 * @param parity_bit 需要设置的校验位。
 * @return 如果操作成功，返回0；如果desc为空，返回-EFAULT；如果操作接口未定义或设置函数为空，返回-EACCES。
 */
int pl_serial_set_parity_bit(struct pl_serial_desc *desc, u8_t parity_bit)
{
	int ret;

	// 检查描述符是否为空，如果为空，则返回错误码-EFAULT
	if (desc == NULL)
		return -EFAULT;

	// 检查描述符的操作集合是否为空，或者设置校验位的操作是否未定义，如果满足任一条件，则返回错误码-EACCES
	if (desc->ops == NULL || desc->ops->set_parity_bit == NULL)
		return -EACCES;

	// 等待串口设备的访问权限
	pl_semaphore_wait(&desc->sem);
	// 调用串口设备的操作集合中的设置校验位函数，并保存返回值
	ret = desc->ops->set_parity_bit(desc, parity_bit);
	// 释放串口设备的访问权限
	pl_semaphore_post(&desc->sem);
	// 返回设置校验位的操作结果
	return ret;
}

/**
 * 设置串口的停止位
 * 
 * 此函数用于设置串口设备的停止位数。它通过操作描述符上的特定操作函数来实现这一设置。
 * 
 * @param desc 串口设备的描述符，包含操作函数指针和同步机制等信息。
 * @param stop_bits 指定的停止位数。
 * @return 返回操作的结果。如果desc为NULL，返回-EFAULT；如果操作函数不存在，返回-EACCES；
 *         否则，返回操作的实际结果。
 */
int pl_serial_set_stop_bits(struct pl_serial_desc *desc, u8_t stop_bits)
{
	int ret;
	/* 检查描述符是否为空，如果是，则返回错误码-EFAULT */
	if (desc == NULL)
		return -EFAULT;

	/* 检查操作描述符中的操作函数指针是否为空，如果是，则返回错误码-EACCES */
	if (desc->ops == NULL || desc->ops->set_stop_bits == NULL)
		return -EACCES;

	/* 等待串口设备的互斥访问权限 */
	pl_semaphore_wait(&desc->sem);
	/* 调用操作函数来设置停止位数 */
	ret = desc->ops->set_stop_bits(desc, stop_bits);
	/* 释放串口设备的互斥访问权限 */
	pl_semaphore_post(&desc->sem);
	/* 返回操作的结果 */
	return ret;
}

/**
 * 发送一个字符到串口
 * 
 * 该函数通过串口描述符向串口发送一个字符。它首先检查描述符是否有效，然后检查是否有发送字符的操作函数。
 * 如果描述符或操作函数无效，函数将返回错误。如果一切有效，函数将获取一个信号量以确保串口操作的互斥性，
 * 然后调用操作函数发送字符，最后释放信号量。
 * 
 * @param desc 串口描述符，包含串口操作的相关信息。
 * @param c 待发送的字符。
 * @return 如果发送成功，返回0；如果发生错误，返回相应的错误码。
 */
int pl_serial_send_char(struct pl_serial_desc *desc, char c)
{
	int ret;

	// 检查描述符是否为NULL，如果是，返回错误码-EFAULT。
	if (desc == NULL)
		return -EFAULT;

	// 检查描述符的操作函数指针是否为空，或者发送字符的操作函数是否为空，如果是，返回错误码-EACCES。
	if (desc->ops == NULL || desc->ops->send_char == NULL)
		return -EACCES;

	// 获取信号量，以确保串口操作的互斥性。
	pl_semaphore_wait(&desc->sem);
	// 调用串口操作函数发送字符。
	ret = desc->ops->send_char(desc, c);
	// 释放信号量，允许其他进程或线程访问串口。
	pl_semaphore_post(&desc->sem);
	// 返回发送操作的结果。
	return ret;
}

/**
 * 函数名称：pl_serial_send_string
 * 函数功能：发送一个字符串通过串行接口
 * 参数说明：
 *   - desc: 串行设备描述符，包含操作指针和其他必要信息
 *   - str: 待发送的字符串
 * 返回值：
 *   - 发送成功返回OK，发送失败返回错误码
 * 
 * 函数实现的逻辑：
 *   - 遍历字符串中的每个字符，直到遇到字符串结束符
 *   - 对每个字符调用串行发送函数，发送失败则立即返回错误码
 *   - 如果所有字符都成功发送，则返回OK
 */
static int pl_serial_send_string(struct pl_serial_desc *desc, char *str)
{
	int ret;

	/* 遍历字符串中的每个字符并发送 */
	while (*str) {
		/* 调用串行发送函数发送当前字符，返回值为发送结果 */
		ret = desc->ops->send_char(desc, *str);
		/* 如果发送失败，则返回错误码 */
		if (ret < 0)
			return ret;

		/* 移动到字符串的下一个字符 */
		str++;
	}

	/* 所有字符都成功发送，返回OK */
	return OK;
}

/**
 * 通过串口发送字符串
 * 
 * @param desc 串口描述符，包含串口操作的句柄和状态信息
 * @param str 要发送的字符串
 * @return 发送成功返回0，否则返回错误码
 * 
 * 该函数用于向指定的串口设备发送一个字符串。它首先检查串口描述符是否有效，
 * 然后检查是否有可用的发送函数。如果描述符有效并有可用的发送函数，它将获取
 * 一个信号量以确保串口操作的互斥性，然后调用相应的发送字符串函数。如果描述符
 * 中只定义了发送字符函数而没有定义发送字符串函数，则使用内部的发送字符串函数。
 * 发送完成后，释放信号量。最后，函数返回发送操作的结果。
 */
int pl_serial_send_str(struct pl_serial_desc *desc, char *str)
{
	int ret;

	/* 检查串口描述符是否为空 */
	if (desc == NULL)
		return -EFAULT;

	/* 检查是否有有效的操作函数指针 */
	if (desc->ops == NULL || (desc->ops->send_char == NULL &&
		desc->ops->send_str == NULL))
		return -EACCES;

	/* 获取信号量以确保串口操作的互斥 */
	pl_semaphore_wait(&desc->sem);
	
	/* 根据是否有send_str函数，选择发送方式 */
	if (desc->ops->send_str == NULL)
		ret = pl_serial_send_string(desc, str);
	else
		ret = desc->ops->send_str(desc, str);

	/* 发送完成后释放信号量 */
	pl_semaphore_post(&desc->sem);

	/* 返回发送操作的结果 */
	return ret;
}

/**
 * 注册串口接收处理函数
 * 
 * 本函数用于在串口描述符中注册一个接收处理函数。当有数据接收时，系统将调用这个注册的处理函数。
 * 
 * @param desc 指向串口描述符的指针，描述符包含了串口的相关信息。
 * @param process 接收处理函数的指针。这个函数将被调用来处理接收到的串口数据。
 * 
 * @return 返回状态码，OK表示成功注册，-EFAULT表示desc指针无效，-EBUSY表示接收处理函数已经注册。
 */
int pl_serial_register_recv_process(struct pl_serial_desc *desc,
                                    pl_serial_recv_process_t process)
{
    /* 检查描述符指针是否为NULL，如果是则返回错误码 */
    if (desc == NULL)
        return -EFAULT;

    /* 进入临界区，以保护接收处理函数的注册过程 */
    pl_port_enter_critical();
    /* 如果当前已经有接收处理函数注册，则退出临界区并返回忙错误码 */
    if (desc->recv_info.process != NULL) {
        pl_port_exit_critical();
        return -EBUSY;
    }

    /* 注册新的接收处理函数，并退出临界区 */
    desc->recv_info.process = process;
    pl_port_exit_critical();

    /* 注册成功，返回OK */
    return OK;
}

/**
 * 卸载串口接收处理程序
 * 
 * 本函数用于卸载指定串口描述符的接收处理程序。在多任务环境下，确保了对串口描述符修改的原子性。
 * 
 * @param desc 指向串口描述符的指针，描述符包含了串口的配置和状态信息。
 * 
 * @return 如果描述符为空，则返回-EFAULT；否则成功卸载接收处理程序并返回OK。
 */
int pl_serial_unregister_recv_process(struct pl_serial_desc *desc)
{
    /* 检查描述符是否为空，如果是，则返回错误 */
    if (desc == NULL)
        return -EFAULT;

    /* 进入临界区，确保对desc的修改不会被中断 */
    pl_port_enter_critical();
    /* 将接收处理程序指针设置为NULL，表示不再有接收处理程序 */
    desc->recv_info.process = NULL;
    /* 离开临界区，允许其他任务访问desc */
    pl_port_exit_critical();

    /* 成功完成，返回OK */
    return OK;
}

/**
 * 注册串行接收回调函数
 * 
 * 本函数用于在串行设备上注册一个接收回调函数。当有数据接收时，设备会调用这个回调函数。
 * 
 * @param desc 指向串行设备描述符的指针。
 * @param recv_callback 接收回调函数的指针。当有数据接收时，设备会调用这个函数。
 * 
 * @return 返回状态码。如果成功注册回调函数，返回OK；如果desc为NULL，返回-EFAULT；如果已经有回调函数注册，返回-EBUSY。
 */
int pl_serial_register_recv_callback(struct pl_serial_desc *desc,
                         pl_serial_recv_callback_t recv_callback)
{
    /* 检查描述符是否为空 */
    if (desc == NULL)
        return -EFAULT;

    /* 禁止任务调度，以确保对回调函数指针的修改是原子的 */
    pl_task_schedule_lock();
    /* 如果已经有回调函数注册，则取消任务调度锁并返回-EBUSY */
    if (desc->recv_info.callback != NULL) {
        pl_task_schedule_unlock();
        return -EBUSY;
    }

    /* 注册新的接收回调函数 */
    desc->recv_info.callback = recv_callback;
    /* 解除任务调度锁 */
    pl_task_schedule_unlock();

    return OK;
}

/**
 * 取消串行接收回调函数的注册
 * 
 * 本函数用于取消之前注册的串行接收回调函数。当不再需要接收回调时，可以通过调用此函数来解除注册。
 * 
 * @param desc 指向pl_serial_desc结构体的指针，该结构体描述了串行端口的属性和状态。
 * 
 * @return 如果desc为空指针，则返回-EFAULT；否则返回OK，表示取消注册成功。
 */
int pl_serial_unregister_recv_callback(struct pl_serial_desc *desc)
{
    /* 检查参数desc是否为空，如果为空则返回错误码-EFAULT */
    if (desc == NULL)
        return -EFAULT;

    /* 加锁以保护调度过程，避免并发访问带来的竞争条件 */
    pl_task_schedule_lock();
    /* 将desc中的接收回调函数指针设置为NULL，表示取消注册 */
    desc->recv_info.callback = NULL;
    /* 解锁，允许其他任务或线程继续执行 */
    pl_task_schedule_unlock();

    /* 返回成功码OK，表示取消注册完成 */
    return OK;
}

/**
 * 初始化串行通信核心模块
 * 
 * 该函数负责初始化串行通信的核心结构体列表，并通过早期系统日志功能记录初始化完成的信息。
 * 它是静态链接的，意味着它只在本模块内可见，不供其他模块调用。
 * 
 * @return 返回0，表示初始化成功。
 */
static int pl_serial_core_init(void)
{
    /* 初始化串行设备描述符列表 */
    list_init(&pl_serial_desc_list);
    
    /* 通过早期系统日志记录串行核心初始化完成 */
    pl_early_syslog_info("serial core init done\r\n");
    
    /* 初始化成功，返回0 */
    return 0;
}
pl_core_initcall(pl_serial_core_init);
