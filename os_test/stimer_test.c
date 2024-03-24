#include <kernel/initcall.h>
#include <kernel/softtimer.h>
#include <kernel/syslog.h>
#include <kernel/task.h>


static void stimer_callback(stimer_handle_t timer)
{
	int ret;
	USED(timer);
	struct count c = {.hi32 = 0, .lo32 = 2000};

	//pl_softtimer_get_private_data(timer, &data);
	pl_syslog_info("stimer_callback1111111111111111111\r\n");
	ret = pl_softtimer_start(timer, stimer_callback, &c, NULL);
	if (ret < 0) {
		pl_syslog_err("pl_softtimer_add failed, ret:%d\r\n", ret);
	}
}

static void stimer_callback2(stimer_handle_t timer)
{
	int ret;
	USED(timer);
	struct count c = {.hi32 = 0, .lo32 = 1000};

	//pl_softtimer_get_private_data(timer, &data);
	pl_syslog_info("stimer2_callback2222222222222222\r\n");
	ret = pl_softtimer_start(timer, stimer_callback2, &c, NULL);
	if (ret < 0) {
		pl_syslog_err("pl_softtimer_add failed, ret:%d\r\n", ret);
	}
}

static int stimer_test(void)
{
	int ret;
	struct count c = {.hi32 = 0, .lo32 = 2000};
	stimer_handle_t stimer;
	stimer_handle_t stimer2;

	pl_syslog_info("stimer_test\r\n");
	stimer = pl_softtimer_request();
	if (stimer == NULL) {
		pl_syslog_err("pl_softtimer_request failed\r\n");
		return -1;
	}

	pl_syslog_info("pl_softtimer_set_private_data\r\n");
	//(void)pl_softtimer_set_private_data(stimer, (char *)"soft timer test");

	pl_syslog_info("pl_softtimer_add\r\n");
	ret = pl_softtimer_start(stimer, stimer_callback, &c, NULL);
	if (ret < 0) {
		pl_syslog_err("pl_softtimer_add failed, ret:%d\r\n", ret);
		return -1;
	}

	stimer2 = pl_softtimer_request();
	if (stimer2 == NULL) {
		pl_syslog_err("pl_softtimer_request failed\r\n");
		return -1;
	}

	pl_syslog_info("pl_softtimer_set_private_data\r\n");
	//(void)pl_softtimer_set_private_data(stimer, (char *)"soft timer test");

	pl_syslog_info("pl_softtimer_add\r\n");
	ret = pl_softtimer_start(stimer2, stimer_callback2, &c, NULL);
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
	pl_task_create("softtimer_task", softtimer_test_task, PL_CFG_PRIORITIES_MAX - 1, 512, 0, NULL);

	return 0;
}
core_initcall(softtimer_test);