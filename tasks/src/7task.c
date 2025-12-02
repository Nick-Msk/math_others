#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "check.h"
#include "eng_runner.h"

static  		bool target_f(int x, int y, long checked_val){
	long res = (long) x * x * x * y - (long) y * y * y * x;
	return res == checked_val;
}


int			main(int argc, const char *argv[]){
	if (!check_arg(3, "Usage: %s from to [min value (2023) - max value]\n", *argv))
        return 1;

	struct eng_int_interval ru1;
	ru1.fromX = ru1.fromY = atoi(argv[1]);
	ru1.toX = ru1.toY = atoi(argv[2]);
	ru1.target = target_f;
	ru1.printFlag = true;
	ru1.stopRun = false;

	int	val_curr = 2023;	// default as per task
	int	val_to = val_curr;
	if (argc > 3)
		val_curr = atoi(argv[3]);
	
	if (argc > 4)
		val_to = atoi(argv[4]);

	printf("Check in integer x^3*y - x*y^3 = %d - %d\nInterval from %d till %d\n", val_curr, val_to, ru1.fromX, ru1.toX);

	// check_int_interval(struct int_interval *);

	int cnt = eng_check_int2dim_interval(ru1, (long)val_curr, (long)val_to);

	if (cnt > 0)
		printf("%d was/were found\n", cnt);
	else
		printf("Not found for that interval...\n");


	return 0;
}

