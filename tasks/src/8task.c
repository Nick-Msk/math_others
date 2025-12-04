
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "check.h"
#include "eng_runner.h"
#include "log.h"



// x * x
static                  bool target_int_1dim_f(int x, int cnt_low, int cnt_high){
        long res1 = 0;
		for (int i = 0; i < cnt_low; i++){
			res1 += (long) x * x;
			x++;
		}
		long res2 = 0;
		for (int i = 0; i < cnt_high; i++){
	  		res1 += (long) x * x;
			x++;
		}
        return res1 == res2;
}


static					bool wrapper_int_1dim(int x, long v){
	return target_int_1dim_f(x, 3, 2);	// for now just a wrapper
}

int						custom_int_1dim(struct eng_int_interval ru);
//int 					custom_check_int1dim(struct eng_int_interval ru1);

int                     main(int argc, const char *argv[]){
    loginit("log/7task.log", false, 0, "Start");

    if (!check_arg(3, "Usage: %s from to [min value (2023) - max value]\n", *argv)){
    	return 1;
	}

	struct eng_int_interval	ru1;	// will use only toX, fromX
	ru1.fromX = atoi(argv[1]);
	ru1.toX = atoi(argv[2]);
	ru1.stopRun = false;
	ru1.printFlag = true;	// TODO: make a constructor!! to fill that stuff
	ru1.modLog = 0;	// don't print logs
	ru1.f_int_1dim = wrapper_int_1dim;

	printf("Check x2 + (x+1)2 + (x+2)2 vs (x+4)2 + (x+5)2 for x in [%d - %d]\n", ru1.fromX, ru1.toX);
	
	//int cnt = custom_check_int1dim_interval(ru1, (long)val_curr, (long)val_to);
	int cnt = custom_int_1dim(ru1);

    if (cnt > 0){
        printf("%d was/were found\n", cnt);
	}  else {
        printf("Not found for that interval...\n");
	}

	logclose("...");
	return 0;
}

int						custom_int_1dim(struct eng_int_interval ru){
	static unsigned long 	cnt = 0;
	int 					total = 0;
	// check through ru.fromX to ru.toX the wrapper
	for (int x = ru.fromX; x <= ru.toX; x++){
		if (ru.modLog > 0 && ru.printFlag && cnt++ % ru.modLog == 0)
			logsimple("Cnt=%ld", cnt);
		if (ru.f_int_1dim(x, 0)){	// TODO:  think if add &val
			if (ru.printFlag)
				printf("Target (%d) is true\n", x);
			if (ru.stopRun)
				return 1;
			else
			total++;
		}
	}
	return total;
}

/*
int 					custom_check_int1dim_interval(struct eng_int_interval ru, int val_curr, int val_to){
	logenter("from %ld to %ld", val_curr, val_to);
	eng_fautoprint(logfile, ru);
	int cnt;

	for (long i = val_curr; i <= val_to; i++){
		// check i2 + (i+1)2 + i(i+2)2 = (i+3)2 + (i+4)2
		cnt += custom_check_int_1dim(ru, i);
	}

	return logret(cnt, "%d", cnt);
}
*/

