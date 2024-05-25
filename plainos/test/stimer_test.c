#include <kernel/initcall.h>
#include <kernel/softtimer.h>
#include <kernel/syslog.h>
#include <kernel/task.h>
#include <drivers/gpio/gpio.h>

//static int times = 0;
static struct pl_stimer *stimer;
static struct pl_stimer *stimer2;
static struct gpio_desc *gpio_desc;

static void stimer_callback(struct pl_stimer *timer)
{
	int ret;
	void *data;
	USED(timer);

	pl_softtimer_get_private_data(timer, &data);
	pl_port_putc('A');
	ret = pl_softtimer_reload(timer, true, stimer_callback, 20, data);
	if (ret < 0) {
		pl_syslog_err("pl_softtimer_add failed, ret:%d\r\n", ret);
	}
}

static void stimer_callback2(struct pl_stimer *timer)
{
	int ret;
	static u8_t cnt = 0;
	void *data;

	pl_gpio_set_io_one(gpio_desc, 15, GPIO_SET_VALUE, cnt);

	cnt = ~cnt;

	pl_softtimer_get_private_data(timer, &data);
	pl_port_putc('B');
	ret = pl_softtimer_reload(timer, true, stimer_callback2, 99, data);
	if (ret < 0) {
		pl_syslog_err("pl_softtimer_add failed, ret:%d\r\n", ret);
		return;
	}
}

static int stimer_test(void)
{
	int ret;

	pl_syslog_info("stimer_test\r\n");
	stimer = pl_softtimer_request("timer1");
	if (stimer == NULL) {
		pl_syslog_err("pl_softtimer_request failed\r\n");
		return -1;
	}

	pl_syslog_info("pl_softtimer_add\r\n");
	pl_softtimer_timer_init(stimer, stimer_callback, 90, (void *)"111111111111111111111");
	ret = pl_softtimer_start(stimer);
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
	pl_softtimer_timer_init(stimer2, stimer_callback2, 100, (void *)"111111111111111111111");
	ret = pl_softtimer_start(stimer2);
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
	int ret;

	pl_syslog_info("softtimer test\r\n");
	pl_tid_t timer_task = 
	pl_task_create("softtimer_task", softtimer_test_task, PL_CFG_TASK_PRIORITIES_MAX - 1, 512, 0, NULL);
	if (timer_task == NULL) {
		pl_syslog_err("softtimer test task failed\r\n");
	}

	gpio_desc = pl_gpio_desc_find_by_no(0);
	if (gpio_desc == NULL) {
		pl_syslog_err("xxxxxxxxxxxxxxxxxxxnot found gpioxxxxxxxxxxxxxxxxxxxxx\r\n");
		return 0;
	}

	/* set direction of GPIOB7 */
	ret = pl_gpio_set_io_one(gpio_desc, 15, GPIO_SET_DIRECT, 1);
	if (ret < 0) {
		pl_syslog_err("gpio direction set failed, ret:%d\r\n", ret);
		return 0;
	}

	pl_syslog_info("softtimer test done\r\n");
	return 0;
}
pl_late_initcall(softtimer_test);
