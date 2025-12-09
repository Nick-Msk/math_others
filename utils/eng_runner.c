#include <stdio.h>

#include "eng_runner.h"
#include "log.h"

// for 1 dim int
int								eng_int_1dim(struct eng_int_interval){
	// TODO:
	int						total = 0;
	static unsigned long	cnt = 0;

    // iterator need to be here
    for (int x = rt.fromX; x <= rt.toX; x++){
		if (rt.modLog > 0 && rt.printFlag && cnt++ % rt.modLog == 0)
			logsimple("cnt=%lu", cnt);
        if (rt.targetValueFlag && rt.f_int_1dim(x, rt.targetValue) ||
			!rt.targetValueFlag && rt.f_int_1dim_bool(x)
				)
		{
            if (rt.printFlag)
                printf("Target function(%d, %d) is true", x, y);
			if (rt.targetValueFlag)
				printf("for %ld)\n", rt.targetValue);
			else
				printf("\n");
			if (rt.stopRun)
                return 1;
            else
                total++;
        }
	}
	return total;
}

int                             eng_int_2dim(struct eng_int_interval rt){
	int             		total = 0;
	static unsigned long	cnt = 0;

    // iterator need to be here
    for (int x = rt.fromX; x <= rt.toX; x++)
    	for (int y = rt.fromY; y <= rt.toY; y++){
			if (rt.modLog > 0 && rt.printFlag && cnt++ % rt.modLog == 0)
				logsimple("cnt=%lu", cnt);
        	if (rt.targetValueFlag && rt.f_int_2dim(x, y, rt.targetValue) ||
					!rt.targetValueFlag && rt.f_int_2dim_bool(x, y)
				)
			{
            	if (rt.printFlag)
                	printf("Target function(%d, %d) is true", x, y);
				if (rt.targetValueFlag)
					printf("for %ld)\n", rt.targetValue);
				else
					printf("\n");
                if (rt.stopRun)
                   	return 1;
                else
                    total++;
            }
        }
    return total;
}

int                             eng_check_int2dim_interval(struct eng_int_interval rt, long val_from, long val_to){
	logenter("from %ld, to %ld", val_from, val_to);
	eng_fautoprint(logfile, rt);
	int total = 0;
    for (long i = val_from; i <= val_to; i++){
		rt.targetValue = i;	
    	total += eng_int_2dim(rt);
	}
    return logret(total, "%d", total);
}

int                             eng_fautoprint(FILE *f, struct eng_int_interval v){
	int cnt = 0;
	// use my bool.h instead of standard for bool_str()
	cnt += fprintf(f, 
		"%*cfromX=%d, toX=%d, fromY=%d, toY=%d, isTargetValue=%d, stopRun=%d, printFlag=%d, modLog=%d f_int1dim=%p f_int2dim=%p\n",
		logoffset, '|', v.fromX, v.toX, v.fromY, v.toY, v.targetValueFlag,	
		v.stopRun, v.printFlag, v.modLog, v.f_int_1dim, v.f_int_2dim);	// logoffset must return 0 if log isn't enabled
	if (v.targetValueFlag)
		cnt += fprintf(f, "%*c targetValue=%ld\n", logoffset, '|', v.targetValue);	
	return cnt;
}
