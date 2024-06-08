
#include <port.h>
#include <types.h>
#include <appcall.h>
#include <kernel/syslog.h>

/**************************************************************************************
 * @brief 重启系统函数
 * 
 * 该函数用于执行系统的重启操作。它不关心命令行参数，仅用于触发重启流程。
 * 
 * @param argc 命令行参数的数量，未使用
 * @param argv 命令行参数的字符串数组，未使用
 * @return int 返回0，表示重启操作执行成功
 *************************************************************************************/
static int plsh_reboot(int argc, char *argv[])
{
    /* 声明参数argc和argv未使用，避免编译器警告 */
    USED(argc);
    USED(argv);

    /* 记录系统重启开始的日志 */
    pl_syslog("system reset start...\r\n");

    /* 触发系统重启操作 */
    pl_port_system_reset();

    /* 重启操作成功完成，返回0 */
    return 0;
}
pl_app_register(plsh_reboot, "reboot");
