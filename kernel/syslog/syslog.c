
#include <kernel/syslog.h>
#include <pl_cfg.h>

int pl_early_syslog_info(char *fmt, char **log)
{
	
}

int pl_early_syslog_warn(char *fmt, char **log)
{
	
}

int pl_early_syslog_err(char *fmt, char **log)
{
#ifdef PL_CFG_SYSLOG_ANSI_COLOR

#endif /* PL_CFG_SYSLOG_ANSI_COLOR */
	return 0;
}

