#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "check.h"

static int g_cnt = 0;

static  		bool target_f(int x, int y, long checked_val){
	long res = (long) x * x * x * y - (long) y * y * y * x;
	//static int cnt = 0;
	g_cnt++;
	if (g_cnt % 10000 == 0){
	//	printf("x = %d, y = %d, res = %ld\n", x, y, res);
		g_cnt = 0;
	}
	return res == checked_val;
}

static int 		g_eng_print_flag = 1;

typedef bool 	(*f_int2dim)(int, int, long);

// TODO: move to enging_test.h
int				eng_int_2dim(int fromx, int fromy, int tox, int toy, long val, bool stoprun, f_int2dim target);

// TODO: make a struct int_interval {}
int				eng_check_int2dim_interval(int fromx, int fromy, int tox, int toy, long val_from, long val_to, bool stoprun, f_int2dim target);

int			main(int argc, const char *argv[]){
	if (!check_arg(3, "Usage: %s from to [value]\n", *argv))
        return 1;

	int low, high;
	low = atoi(argv[1]);
	high = atoi(argv[2]);
	
	int	val_curr = 2023;	// default as per task
	if (argc > 3)
		val_curr = atoi(argv[3]);

	printf("Check in integer x^3*y - x*y^3 = %d\nInterval from %d till %d\n", val_curr, val_curr, val_curr);

	// check_int_interval(struct int_interval *);
	int cnt = eng_check_int2dim_interval(low, low, high, high, (long)val_curr, (long)val_curr, false, target_f);

	if (cnt > 0)
		printf("%d was/were found\n", cnt);
	else
		printf("Not found for that interval...\n");


	return 0;
}

int				eng_check_int2dim_interval(int fromx, int fromy, int tox, int toy, long val_from, long val_to, bool stoprun, f_int2dim target){
	int total = 0;
	for (long i = val_from; i <= val_to; i++)
		total += eng_int_2dim(fromx, fromy, tox, toy, i, stoprun, target);
	return total;
}

int				eng_int_2dim(int fromx, int fromy, int tox, int toy, long val, bool stoprun, f_int2dim target){
	int		total = 0;
	// iterator need to be here
	for (int x = fromx; x <= tox; x++)
		for (int y = fromy; y <= toy; y++){
			if (target(x, y, val)){
				if (g_eng_print_flag)
					printf("target function(%d, %d) is true for %ld)\n", x, y, val);
				if (stoprun)
					return 1;
				else
					total++;
		}
	}	
	return total;

}



