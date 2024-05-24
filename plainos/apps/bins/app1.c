
#include <appcall.h>
#include <kernel/syslog.h>

static int my_app_main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	int i;

	pl_syslog_info("test is a test\r\n");
	pl_syslog_info("argc:%d\r\n", argc);

	for (i = 0; i < argc; i++)
		pl_syslog_info("argv[%d]:%s\r\n", i, argv[i]);

	for (volatile int j = 0; j < 35; j++)
	;

	return -5;
}

pl_app_register(my_app_main, "app_test");
