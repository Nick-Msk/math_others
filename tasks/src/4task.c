#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static  		bool target_f(int x, int y, long checked_val){
	long res = (long) x * x * x - (long) y * y * y;
	return res == checked_val;
}

static int 		g_print_flag = 1;

typedef bool 	(*f_int2dim)(int, int, long);

int				eng_int_2dim(int fromx, int fromy, int tox, int toy, long val, bool stoprun, f_int2dim target);

int				main(int argc, const char *argv[]){
	int 	val = 37;	// as per task
	int 	low, high;

	if (argc < 3){ // user checker.h
		return 1;
	}	

	low = atoi(argv[1]);
	high = atoi(argv[2]);

	if (argc > 4)
		val = atoi(argv[3]);

	printf("Start check for interval [%d - %d] %d\n", val, low, high);

	int cnt = eng_int_2dim(low, low, high, high, (long)val, false, target_f);

	if (cnt > 0)
		printf("%d was/were found\n", cnt);
	else
		printf("Not found for that interval...\n");

	return 0;
}

int				eng_int_2dim(int fromx, int fromy, int tox, int toy, long val, bool stoprun, f_int2dim target){
	int		total = 0;
	// iterator need to be here
	for (int x = fromx; x < tox; x++)
		for (int y = fromy; y < toy; y++){
		if (target(x, y, val)){
			if (g_print_flag)
				printf("target function(%d, %d) is true)\n", x, y);
			if (stoprun)
				return 1;
			else
				total++;
		}
	}	
	return total;

}
