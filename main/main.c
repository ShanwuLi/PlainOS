#include "../plain_os_core/core.h"
#include "../plain_os_core/list.h"
#include <stdio.h>


struct adef {
	int a;
	int b;
	int c;
};

int main(void)
{
	struct adef defe;
	printf("a:0x%x\n", &defe.a);
	printf("a:0x%x\n", container_of(&defe.c, struct adef, c));
	return 0;
}
