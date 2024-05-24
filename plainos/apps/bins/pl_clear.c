
#include <types.h>
#include <appcall.h>
#include <kernel/syslog.h>
#include <limits.h>

extern struct pl_app_entry __appcall_start[];
extern struct pl_app_entry __appcall_end[];

/**************************************************************************************
 * @brief: Searches for an application entry with the given name.
 *
 * @param name: The name of the application to find, a null-terminated string.
 * @return: A pointer to the found application entry; returns NULL if no match is found.
 *************************************************************************************/
static int plsh_clear(int argc, char *argv[])
{
	USED(argc);
	USED(argv);

	pl_syslog("\033c");
	return 0;
}

pl_app_register(plsh_clear, "clear");
