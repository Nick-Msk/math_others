
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "check.h"
#include "eng_runner.h"
#include "log.h"


// x + y ==  x * u
static                  bool target_flt_2dim_f(double x, double y){
    return (x + y) == (x * y);	// ?? DBL_EPSILON ?
}

int                     main(int argc, const char *argv[]){
    loginit("log/92task.log", false, 0, "Start");

    if (!check_arg(3, "Usage: %s 'from' 'to' 'step=0.000001' for the x+y == x*y\n", *argv)){
    	return 1;
	}

	struct eng_flt_interval	ru1;	// will use only toX, fromX
	ru1.fromX = ru1.fromY = atof(argv[1]);
	ru1.toX = ru1.toY = atof(argv[2]);
	if (argc > 3)
		ru1.stepX = atof(argv[3]);
	else
		ru1.stepX = 0.000001;	// default step
								
	ru1.targetValueFlag = false;	// bool function! 
	ru1.stopRun = false;
	ru1.printFlag = true;	// TODO: make a constructor!! to fill that stuff
	ru1.modLog = 10000;
	ru1.f_flt_2dim_bool = target_flt_2dim_f;

	printf("Check x+y == x*y for float in  [%g - %g]\n", ru1.fromX, ru1.toX);
	
	int cnt = eng_flt_2dim(ru1);

    if (cnt > 0){
        printf("%d was/were found\n", cnt);
	}  else {
        printf("Not found for that interval...\n");
	}

	logclose("...");
	return 0;
}



