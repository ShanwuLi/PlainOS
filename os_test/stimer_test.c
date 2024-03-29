#include <kernel/initcall.h>
#include <kernel/softtimer.h>
#include <kernel/syslog.h>
#include <kernel/task.h>

static int times = 0;
static stimer_handle_t stimer;
static stimer_handle_t stimer2;

static void stimer_callback(stimer_handle_t timer)
{
	int ret;
	void *data;
	USED(timer);
	struct count c = {.hi32 = 0, .lo32 = 1};

	pl_softtimer_get_private_data(timer, &data);
	pl_early_syslog_info("stimer_callback %s\r\n", (char *)data);
	ret = pl_softtimer_start(timer, stimer_callback, &c, data);
	if (ret < 0) {
		pl_syslog_err("pl_softtimer_add failed, ret:%d\r\n", ret);
	}
}


static void stimer_callback2(stimer_handle_t timer)
{
	int ret;
	void *data;
	struct count c = {.hi32 = 0, .lo32 = 1};

	pl_softtimer_get_private_data(timer, &data);
	pl_early_syslog_info("stimer2_callback %s\r\n", (char *)data);
	ret = pl_softtimer_start(timer, stimer_callback2, &c, data);
	if (ret < 0) {
		pl_syslog_err("pl_softtimer_add failed, ret:%d\r\n", ret);
		return;
	}

	if (times++ < 1000) {
		return;
	}

	ret = pl_softtimer_cancel(stimer);
	if (ret < 0) {
		pl_syslog_err("pl_softtimer_cancel failed, ret:%d\r\n", ret);
	}

	pl_early_syslog_info("timer1 canceled!!!!!\r\n");

}


static int stimer_test(void)
{
	int ret;
	struct count c = {.hi32 = 0, .lo32 = 1};

	pl_syslog_info("stimer_test\r\n");
	stimer = pl_softtimer_request("timer1");
	if (stimer == NULL) {
		pl_syslog_err("pl_softtimer_request failed\r\n");
		return -1;
	}

	pl_syslog_info("pl_softtimer_add\r\n");
	ret = pl_softtimer_start(stimer, stimer_callback, &c, (void *)"111111111111111111111");
	if (ret < 0) {
		pl_syslog_err("pl_softtimer_add failed, ret:%d\r\n", ret);
		return -1;
	}

	stimer2 = pl_softtimer_request("timer2");
	if (stimer2 == NULL) {
		pl_syslog_err("pl_softtimer_request failed\r\n");
		return -1;
	}

	pl_syslog_info("pl_softtimer_add\r\n");
	ret = pl_softtimer_start(stimer2, stimer_callback2, &c, (void *)"222222222222222222222");
	if (ret < 0) {
		pl_syslog_err("pl_softtimer_add failed, ret:%d\r\n", ret);
		return -1;
	}

	pl_syslog_err("softtimer add success");
	return 0;
}

static int softtimer_test_task(int argc, char *argv[])
{
	USED(argc);
	USED(argv);
	stimer_test();
	return 900;
}

static int softtimer_test(void)
{
	pl_task_create("softtimer_task", softtimer_test_task, PL_CFG_TASK_PRIORITIES_MAX - 1, 512, 0, NULL);

	return 0;
}
core_initcall(softtimer_test);