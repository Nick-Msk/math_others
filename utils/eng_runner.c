#include <stdio.h>

// #include "log.h" Stip for now to avoid reconf
#include "eng_runner.h"

int                             eng_int_2dim(struct eng_int_interval rt, long val){
	int             total = 0;
    // iterator need to be here
    for (int x = rt.fromX; x <= rt.toX; x++)
    	for (int y = rt.fromY; y <= rt.toY; y++){
        	if (rt.target(x, y, val)){
            	if (rt.print_flag)
                	printf("Target function(%d, %d) is true for %ld)\n", x, y, val);
                if (rt.stopRun)
                   	return 1;
                else
                    total++;
            }
        }
    return total;
}

int                             eng_check_int2dim_interval(struct eng_int_interval rt, long val_from, long val_to){
	int total = 0;	// TODO: add logging here
    for (long i = val_from; i <= val_to; i++)
    	total += eng_int_2dim(rt, i);
    return total;
}


