
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "check.h"
#include "eng_runner.h"
#include "log.h"

// check  n / a + b == a + b
static                  bool target_int_1dim_f(int n){
	int tmp = n, sum = 0;
	while (tmp){
		sum += tmp % 10;
		tmp /= 10;
	}
	return n == sum * sum;
}

int                     main(int argc, const char *argv[]){
    loginit("log/97task.log", false, 0, "Start");

    if (!check_arg(1, "Usage: %s 'to int'\n", *argv)){
    	return 1;
	}

	struct eng_int_interval	ru1;	// will use only toX, fromX
	ru1.fromX = 1;
	ru1.toX = atoi(argv[1]);
								
	ru1.targetValueFlag = false;	// bool function! 
	ru1.stopRun = false;
	ru1.printFlag = true;	// TODO: make a constructor!! to fill that stuff
	ru1.modLog = 10000;
	ru1.f_int_1dim_bool = target_int_1dim_f;

	printf("Check 'ab' / a + b == a + b  [%d - %d]\n", ru1.fromX, ru1.toX);
	
	int cnt = eng_int_1dim(ru1);

    if (cnt > 0){
        printf("%d was/were found\n", cnt);
	}  else {
        printf("Not found for that interval...\n");
	}

	logclose("...");
	return 0;
}



