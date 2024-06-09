
#include <types.h>
#include <appcall.h>
#include <port/port.h>
#include <kernel/syslog.h>

/**************************************************************************************
 * @brief: Searches for an application entry with the given name.
 *
 * @param name: The name of the application to find, a null-terminated string.
 * @return: A pointer to the found application entry; returns NULL if no match is found.
 *************************************************************************************/
static int plsh_reboot(int argc, char *argv[])
{
	USED(argc);
	USED(argv);

	pl_syslog("system reset start...\r\n");
	pl_port_system_reset();
	return 0;
}
pl_app_register(plsh_reboot, "reboot");
