
#include <appcall.h>

static int my_app_main(int argc, char *argv[])
{
	return 0;
}

system_app_register(my_app_main, 0, NULL);
