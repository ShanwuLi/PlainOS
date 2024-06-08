
#include <types.h>
#include <appcall.h>
#include <kernel/syslog.h>

extern struct pl_app_entry __appcall_start[];
extern struct pl_app_entry __appcall_end[];

/**************************************************************************************
 * 列出所有应用程序的静态函数。
 * 
 * 该函数遍历一个内部定义的数组，该数组包含了所有可用的应用程序的入口点。
 * 它通过调用pl_syslog函数将每个应用程序的名称打印到日志中。
 * 
 * @param argc 传递给应用程序的参数数量，此函数中未使用。
 * @param argv 传递给应用程序的参数数组，此函数中未使用。
 * @return 总是返回0，表示函数执行成功。
 *************************************************************************************/
static int plsh_list_all_apps(int argc, char *argv[])
{
    /* 忽略argc和argv参数，因为此函数不需要它们 */
    USED(argc);
    USED(argv);

    struct pl_app_entry *entry;

    /* 遍历从__appcall_start到__appcall_end定义的应用程序列表 */
    for (entry = __appcall_start; entry < __appcall_end; entry++) {
        /* 打印每个应用程序的名称 */
        pl_syslog("%s  ", entry->name);
    }

    /* 打印换行符，以整洁地结束应用程序列表的输出 */
    pl_syslog("\r\n");
    return 0;
}

pl_app_register(plsh_list_all_apps, "ls");
