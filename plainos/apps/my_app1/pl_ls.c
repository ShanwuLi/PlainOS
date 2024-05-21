
#include <types.h>
#include <appcall.h>
#include <kernel/syslog.h>

extern struct pl_app_entry __appcall_start[];
extern struct pl_app_entry __appcall_end[];

/**************************************************************************************
 * @brief: Searches for an application entry with the given name.
 *
 * @param name: The name of the application to find, a null-terminated string.
 * @return: A pointer to the found application entry; returns NULL if no match is found.
 *************************************************************************************/
static int plsh_list_all_apps(int argc, char *argv[])
{
	USED(argc);
	USED(argv);
	struct pl_app_entry *entry;

	for (entry = __appcall_start; entry < __appcall_end; entry++) {
		pl_syslog("%s  ", entry->name);
	}

	pl_syslog("\r\n");
	return 0;
}

pl_app_register(plsh_list_all_apps, "ls");
