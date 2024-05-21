
#include "Calculator.h"
#include <kernel/syslog.h>
#include <appcall.h>

static int pl_calculator(int argc, char **argv)
{
	if (argc != 2)
		return -1;

    double result = 0.0;
	char str[100]={0};

    INFIX_INTO_SUFFIX InfixIntoSuffix;
    CALCULATE_SUFFIX  cal;

    if(!InfixIntoSuffix.CheckLegal(argv[1]))
       pl_syslog("input error\r\n");
    else
    {
	  pl_syslog("statr convert\r\n");
      InfixIntoSuffix.Convert(argv[1], str);
	   pl_syslog("statr calculate\r\n");
      result = cal.calculate(str);
      pl_syslog("%s = %d\r\n", argv[1], (int)result);
    }

    return 0;
}
pl_app_register(pl_calculator, "calculator");