#include <config.h>
#include <errno.h>
#include <types.h>
#include <kernel/initcall.h>
#include <port/port.h>
#include <kernel/syslog.h>
#include <kernel/workqueue.h>

static void work_fun(struct pl_work *work)
{
	USED(work);
	pl_syslog_info("%s\r\n", (char *)pl_work_get_private_data(work));
}

static struct pl_work hiwork;
static struct pl_work lowork;

static int workqueue_test(void)
{
	int ret;

	pl_syslog_info("workqueue test\r\n");
	ret = pl_work_init(&hiwork, work_fun, (char *)"##############################################################");
	if (ret < 0) {
		pl_syslog_err("work init failed in %d, ret:%d\r\n", __LINE__, ret);
		return ret;
	}

	ret = pl_work_add(g_pl_sys_hiwq_handle, &hiwork);
	if (ret < 0) {
		pl_syslog_err("work add failed in %d, ret:%d\r\n", __LINE__, ret);
		return ret;
	}

	ret = pl_work_init(&lowork, work_fun, (char *)"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	if (ret < 0) {
		pl_syslog_err("work init failed in %d, ret:%d\r\n", __LINE__, ret);
		return ret;
	}

	ret = pl_work_add(g_pl_sys_lowq_handle, &lowork);
	if (ret < 0) {
		pl_syslog_err("work add failed in %d, ret:%d\r\n", __LINE__, ret);
		return ret;
	}

	pl_syslog_info("workqueue test done\r\n");
	return 0;
}
pl_late_initcall(workqueue_test);
