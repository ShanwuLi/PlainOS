
#include <kernel/syslog.h>
#include <pl_cfg.h>
#include "../common/ansi_color.h"
#include <pl_port.h>
#include <errno.h>

int early_put_string(char *str)
{
	int ret;

	while (*str) {
		ret = pl_early_port_putc(*str);
		if (ret < 0)
			return ERROR;
		str++;
	}

	return OK;
}

#if 0
int pl_early_syslog_info(char *fmt, char **log)
{
	return OK;
}

int pl_early_syslog_warn(char *fmt, char **log)
{
#ifdef PL_CFG_SYSLOG_ANSI_COLOR
	
#endif /* PL_CFG_SYSLOG_ANSI_COLOR */

	return OK;
}

int pl_early_syslog_err(char *fmt, char **log)
{
#ifdef PL_CFG_SYSLOG_ANSI_COLOR

#endif /* PL_CFG_SYSLOG_ANSI_COLOR */
	return OK;
}

#endif

