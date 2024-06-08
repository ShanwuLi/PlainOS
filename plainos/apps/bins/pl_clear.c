
#include <types.h>
#include <appcall.h>
#include <kernel/syslog.h>
#include <limits.h>

/**************************************************************************************
 * 函数用途：清除终端屏幕。
 * 
 * 参数说明：
 * argc - 命令行参数的数量，本函数中未使用。
 * argv - 命令行参数的字符串数组，本函数中未使用。
 * 
 * 返回值：始终返回0，表示操作成功。
 * 
 * 注意：本函数通过调用pl_syslog函数发送一个清除屏幕的控制序列到日志系统，
 * 从而实现清除终端屏幕的效果。USED宏用于告诉编译器参数argc和argv虽然未在函数体内使用，
 * 但它们的引入是故意的，以满足某些编译器的警告消除或代码规范要求。
 *************************************************************************************/
static int plsh_clear(int argc, char *argv[])
{
    USED(argc);
    USED(argv);

    pl_syslog("\033c");
    return 0;
}

pl_app_register(plsh_clear, "clear");
