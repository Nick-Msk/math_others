
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "check.h"
#include "eng_runner.h"
#include "log.h"



// n + 9 / n + 6
static                  bool target_int_1dim_f(int n){
	if (n == -6)
		return false;
	else
    	return (n + 9) % (n + 6) == 0;
}

int                     main(int argc, const char *argv[]){
    loginit("log/9task.log", false, 0, "Start");

    if (!check_arg(3, "Usage: %s 'from' 'to' for the n + 9 / n + 6\n", *argv)){
    	return 1;
	}

	struct eng_int_interval	ru1;	// will use only toX, fromX
	ru1.fromX = atoi(argv[1]);
	ru1.toX = atoi(argv[2]);
	ru1.targetValueFlag = false;	// bool function! 
	ru1.stopRun = false;
	ru1.printFlag = true;	// TODO: make a constructor!! to fill that stuff
	ru1.modLog = 100;
	ru1.f_int_1dim_bool = target_int_1dim_f;

	printf("Check n + 9 / n + 6 for natural for N in [%d - %d]\n", ru1.fromX, ru1.toX);
	
	int cnt = eng_int_1dim(ru1);

    if (cnt > 0){
        printf("%d was/were found\n", cnt);
	}  else {
        printf("Not found for that interval...\n");
	}

	logclose("...");
	return 0;
}

