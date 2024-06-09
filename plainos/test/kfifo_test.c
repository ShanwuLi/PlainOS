
#include <port/port.h>
#include <kernel/assert.h>
#include <kernel/initcall.h>
#include <kernel/kfifo.h>
#include <kernel/syslog.h>
#include <kernel/task.h>


static char read_data[100] = {0};

static int kfifo_test(void)
{
	uint_t data_num;
	struct pl_kfifo *kfifo = pl_kfifo_request(128);

	if (kfifo == NULL) {
		pl_syslog_err("pl_kfifo_request failed\r\n");
		return 0;
	}

	pl_port_enter_critical();
	pl_syslog_info("%%%%%%%%%%%%%%%%%% KFIFO TEST %%%%%%%%%%%%%%%%%%%%%%\r\n");
	pl_syslog_info("get3:%s", "dwdejkfekvev\r\n");
	pl_port_exit_critical();

	pl_kfifo_put(kfifo, (char *)"123", 3);
	pl_kfifo_put(kfifo, (char *)"1234", 4);
	pl_kfifo_put(kfifo, (char *)"123456", 6);
	pl_kfifo_put(kfifo, (char *)"12345689", 7);
	pl_kfifo_put(kfifo, (char *)"12345", 5);

	data_num = pl_kfifo_get(kfifo, read_data, 50);

	pl_port_enter_critical();
	pl_syslog_info("data_num:%d, get3:%s end\r\n", data_num, read_data);
	pl_syslog_info("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\r\n");
	pl_port_exit_critical();

	return 0;
}

pl_late_initcall(kfifo_test);
