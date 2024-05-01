
#include <appcall.h>

static int my_app_main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	return 0;
}

pl_app_register(my_app_main, "app_test");
