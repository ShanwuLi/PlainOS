#include "stm32f10x_it.h"
#include <types.h>
#include <kernel/syslog.h>

void pl_panic_dump(u32_t panic_reason, void *arg);
/**
 * @brief 系统dump函数
 * 
 * 该函数在系统出现严重故障时被调用，用于将关键的寄存器值输出到日志系统，
 * 以便于故障分析。它打印了ARM Cortex处理器的几个重要寄存器的值，
 * 包括r0到r3的通用寄存器，r12、lr、pc和psr特殊寄存器。
 * 
 * @param fault_args 故障发生时的寄存器值数组，包含了r0到r7的值。
 */
void pl_sys_dump(u32_t *fault_args)
{
    /* 输出系统dump的标题 */
    pl_early_syslog_err("system dump:\r\n");
    
    /* 逐个打印通用寄存器r0到r3的值 */
    pl_early_syslog_err("r0:0x%x\r\n",  fault_args[0]);
    pl_early_syslog_err("r1:0x%x\r\n",  fault_args[1]);
    pl_early_syslog_err("r2:0x%x\r\n",  fault_args[2]);
    pl_early_syslog_err("r3:0x%x\r\n",  fault_args[3]);
    
    /* 打印通用寄存器r12的值 */
    pl_early_syslog_err("r12:0x%x\r\n", fault_args[4]);
    
    /* 打印链接寄存器lr的值 */
    pl_early_syslog_err("lr:0x%x\r\n",  fault_args[5]);
    
    /* 打印程序计数器pc的值 */
    pl_early_syslog_err("pc:0x%x\r\n",  fault_args[6]);
    
    /* 打印程序状态寄存器psr的值 */
    pl_early_syslog_err("psr:0x%x\r\n", fault_args[7]);
    
    /* 调用panic dump函数，传入一个魔数和NULL参数 */
    pl_panic_dump((u32_t)74827, NULL);
    
    /* 无限循环，确保系统不会继续执行 */
    while (true);
}
