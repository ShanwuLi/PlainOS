
#include <appcall.h>
#include <kernel/syslog.h>

/**************************************************************************************
 * 程序的主入口函数。
 * 
 * 本函数演示了如何使用日志系统记录程序的启动参数，并展示了一个简单的循环延迟。
 * 
 * @param argc 命令行参数的数量。
 * @param argv 命令行参数的字符串数组。
 * @return 返回一个固定的负值-5。
 *************************************************************************************/
static int my_app_main(int argc, char *argv[])
{
    /* 忽略argc和argv参数，这里只是为了演示如何在 syslog 中记录它们 */
    (void)argc;
    (void)argv;

    /* 记录程序启动的信息 */
    pl_syslog_info("test is a test\r\n");
    pl_syslog_info("argc:%d\r\n", argc);

    /* 遍历命令行参数，并将它们记录到日志中 */
    for (int i = 0; i < argc; i++)
    {
        pl_syslog_info("argv[%d]:%s\r\n", i, argv[i]);
    }

    /* 一个空循环，用于延迟程序退出，这里使用 volatile 关键字以避免编译器优化 */
    for (volatile int j = 0; j < 35; j++)
    {
        /* 循环体为空，仅用于延迟 */
    }

    /* 返回一个固定的负值 */
    return -5;
}

pl_app_register(my_app_main, "app_test");
