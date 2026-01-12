
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "check.h"
#include "eng_runner.h"
#include "log.h"


// TODO: remove??
static const char		*FUNCTION = "x+xy+y==5";

// from 13task_test_func.c
extern					bool target_int_2dim_f(int x, int y);

int                     main(int argc, const char *argv[]){
    loginit("log/14task.log", false, 0, "Start");

	if (argc > 1){
        if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0){
            printf("%s task from zen 12.01.2026\nUsage:%s 'from' 'to'  %s \n", __FILE__, *argv, FUNCTION); // Usage here??
            return 0;
        }
    }

    if (!check_arg(2, "Usage: %s 'from' 'to' for the %s\n", *argv, FUNCTION)){
    	return 1;
	}
	
	int from = atoi(argv[1]), 
	to = atoi(argv[2]);

	struct eng_int_interval	ru1 = eng_create_int(
				.useDim = 2, .fromX = from, .fromY = from, .fromZ = from,
				.toX = to, .toY = to, .toZ = to, .modLog = 1000, .f_int_2dim_bool = target_int_2dim_f
	);

	// TODO: 
	// const chat *confname = argv[1];
	// ru1 = eng_loadfromfile(confname, false);
	// if (ru1.flags &  ENG_ERROR_FLAG){
	// 	fprintf(stderr, "Failed to load config\n"); // no function name there! Always  <N>task_func.c
	// 	return 2;
	// }

	eng_autoprint(ru1);

	printf("Check %s for natural for x, y in [%d - %d]\n", FUNCTION, ru1.fromX, ru1.toX);

	int cnt = eng_int_2dim(ru1);

    if (cnt > 0){
        printf("%d was/were found\n", cnt);
	}  else {
        printf("Not found for that interval...\n");
	}

	logclose("...");
	return 0;
}

