
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "check.h"
#include "eng_runner.h"
#include "log.h"



// 1/x + 1/y + 1/z = 1
static                  bool target_int_3dim_f(int x, int y, int z){
	if (x == 0 || y == 0 || z == 0)
		return false;
	else
		return x * y + y * z + x * z  == x * y * z;
}

int                     main(int argc, const char *argv[]){
    loginit("log/12task.log", false, 0, "Start");

    if (!check_arg(3, "Usage: %s 'from' 'to' for the x*y + y*z + x*z == x*y*z \n", *argv)){
    	return 1;
	}

	struct eng_int_interval	ru1;
	ru1.fromX = ru1.fromY = ru1.fromZ = atoi(argv[1]);
	ru1.toX = ru1.toY = ru1.toZ = atoi(argv[2]);
	ru1.targetValueFlag = false;	// bool function! 
	ru1.stopRun = false;
	ru1.printFlag = true;	// TODO: make a constructor!! to fill that stuff
	ru1.modLog = 100;
	ru1.f_int_3dim_bool = target_int_3dim_f;

	printf("Check x*y + y*z + x*z == x*y*z  for natural for x, y, z in [%d - %d]\n", ru1.fromX, ru1.toX);

	int cnt = eng_int_3dim(ru1);

    if (cnt > 0){
        printf("%d was/were found\n", cnt);
	}  else {
        printf("Not found for that interval...\n");
	}

	logclose("...");
	return 0;
}

