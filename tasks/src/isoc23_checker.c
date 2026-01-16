#include <limits.h>
#include <stdio.h>

int			main(int argc, const char *argv[]){
	printf("int bits=%d\n", __INT_WIDTH__);
//	printf("int bits=%d\n", INT_WIDTH); not working
	return 0;
}

