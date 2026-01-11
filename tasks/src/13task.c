
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "check.h"
#include "eng_runner.h"
#include "log.h"


static const char		*FUNCTION = "6 * p * (p - 1) == 5 * (s + r) * (p + s + r -1)";

static                  bool target_int_3dim_f(int p, int s, int r){
		return 6 * p * (p - 1) == 5 * (s + r) * (p + s + r -1);
}

int                     main(int argc, const char *argv[]){
    loginit("log/13task.log", false, 0, "Start");

	if (argc > 1){
        if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0){
            printf("%s task from Oleg(Lim)\nUsage:%s 'from' 'to'  %s \n", __FILE__, *argv, FUNCTION); // Usage here??
            return 0;
        }
    }

    if (!check_arg(3, "Usage: %s 'from' 'to' for the %s\n", *argv, FUNCTION)){
    	return 1;
	}
	
	int from = atoi(argv[1]), 
		to = atoi(argv[2]);
	struct eng_int_interval	ru1 = eng_create_int(
				.useDim = 3, .fromX = from, .fromY = from, .fromZ = from,
				.toX = to, .toY = to, .toZ = to, .modLog = 1000, .f_int_3dim_bool = target_int_3dim_f
	);

	eng_autoprint(ru1);
	/*
	ru1.fromX = ru1.fromY = ru1.fromZ = atoi(argv[1]);
	ru1.toX = ru1.toY = ru1.toZ = atoi(argv[2]);
	ru1.targetValueFlag = false;	// bool function! 
	ru1.stopRun = false;
	ru1.printFlag = true;	// TODO: make a constructor!! to fill that stuff
	ru1.modLog = 100;
	ru1.f_int_3dim_bool = target_int_3dim_f; */

	printf("Check %s for natural for r, s, r in [%d - %d]\n", FUNCTION, ru1.fromX, ru1.toX);

	int cnt = eng_int_3dim(ru1);

    if (cnt > 0){
        printf("%d was/were found\n", cnt);
	}  else {
        printf("Not found for that interval...\n");
	}

	logclose("...");
	return 0;
}

