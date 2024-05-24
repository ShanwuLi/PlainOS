
#include "Calculator.h"
#include <kernel/syslog.h>
#include <appcall.h>

static int pl_calculator_main(int argc, char *argv[])
{
	int ret;
	float result;

	ret =  pl_calculator(argc, argv, &result);
	if (ret < 0) {
	 	pl_syslog_err("input err\r\n");
		return ret;
	}

	pl_syslog("%s = %d.%d\r\n", argv[1], (int)result, (int)(result*1000) - ((int)result) * 1000);
	return 0;
}
pl_app_register(pl_calculator_main, "calculator");
