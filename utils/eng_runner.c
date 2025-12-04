#include <stdio.h>

#include "eng_runner.h"
#include "log.h"

int                             eng_int_2dim(struct eng_int_interval rt, long val){
	int             		total = 0;
	static unsigned long	cnt = 0;
    // iterator need to be here
    for (int x = rt.fromX; x <= rt.toX; x++)
    	for (int y = rt.fromY; y <= rt.toY; y++){
			if (rt.modLog > 0 && rt.printFlag && cnt++ % rt.modLog == 0)
				logsimple("cnt=%lu", cnt);
        	if (rt.target(x, y, val)){
            	if (rt.printFlag)
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
	logenter("from %ld, to %ld", val_from, val_to);
	eng_fautoprint(logfile, rt);
	int total = 0;
    for (long i = val_from; i <= val_to; i++)
    	total += eng_int_2dim(rt, i);
    return logret(total, "%d", total);
}

int                             eng_fautoprint(FILE *f, struct eng_int_interval v){
	int cnt = 0;
	// use my bool.h instead of standard
	fprintf(f, "%*cfromX=%d, toX=%d, fromY=%d, toY=%d, stopRun=%d, printFlag=%d, modLog=%d target=%p\n",
		logoffset, '|', v.fromX, v.toX, v.fromY, v.toY, v.stopRun, v.printFlag, v.modLog, v.target);	// logoffset must return 0 if log isn't enabled
	return cnt;
}
