#include <stdio.h>


int main(int argc, char **argv)
{
	if (argc == 0)
		return 0;

	printf("hello cfgparse: argv:%s\r\n", argv[1]);
	return 0;
}
