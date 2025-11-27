#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "check.h"

static  		bool target_f(int x, int y, long checked_val){
	long res = (long) x * x * x - (long) y * y * y;
	return res == checked_val;
}

static int 		g_print_flag = 1;

typedef bool 	(*f_int2dim)(int, int, long);

int				eng_int_2dim(int fromx, int fromy, int tox, int toy, long val, bool stoprun, f_int2dim target);

int				check_interval(int fromx, int fromy, int tox, int toy, long val_from, long val_to, bool stoprun, f_int2dim target);


int				main(int argc, const char *argv[]){
	int 	val_curr = 37;	// as per task
	int 	val_high;
	int 	low, high;

	if (!check_arg(3, "Usage: %s from to\n", *argv))
        return 1;

	low = atoi(argv[1]);
	high = atoi(argv[2]);

	if (argc > 3)
		val_curr = atoi(argv[3]);

	if (argc > 4)
		val_high = atoi(argv[4]);
	else
		val_high = val_curr;

	printf("Start check value %d-%d for interval [%d - %d]\n", val_curr, val_high, low, high);

	int cnt = check_interval(low, low, high, high, (long)val_curr, (long)val_high, false, target_f);

	if (cnt > 0)
		printf("%d was/were found\n", cnt);
	else
		printf("Not found for that interval...\n");

	return 0;
}

int				check_interval(int fromx, int fromy, int tox, int toy, long val_from, long val_to, bool stoprun, f_int2dim target){
	int total = 0;
	for (long i = val_from; i <= val_to; i++)
		total += eng_int_2dim(fromx, fromy, tox, toy, i, stoprun, target);
	return total;
}

int				eng_int_2dim(int fromx, int fromy, int tox, int toy, long val, bool stoprun, f_int2dim target){
	int		total = 0;
	// iterator need to be here
	for (int x = fromx; x < tox; x++)
		for (int y = fromy; y < toy; y++){
		if (target(x, y, val)){
			if (g_print_flag)
				printf("target function(%d, %d) is true for %ld)\n", x, y, val);
			if (stoprun)
				return 1;
			else
				total++;
		}
	}	
	return total;

}
