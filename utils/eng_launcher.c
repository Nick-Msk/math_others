
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "checker.h"
#include "eng_runner.h"
#include "log.h"
#include "eng_launcher.h"

int                     main(int argc, const char *argv[]){
    loginit(eng_data.elogfile, false, 0, "Start");
	
	if (argc > 1){
        if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0){
            printf("%s: ", __FILE__); 
			printf("%s: \n", eng_data.task_description);
			printf("Usage: %s <config_file>\n", *argv);
			printf("%s \n", eng_data.func_description);																					  
            return 0;
        }
    }

    if (!check_arg(2, "Usage: %s <config_file> (%s)\n", *argv, eng_data.func_description) ){
    	return 1;
	}
	
	// nut for now it's here
	//(.f_int_2dim_bool = target_int_2dim_f);

	const char *configname = argv[1];	// TODO: what if to use eng_launcher_data? 
	struct eng_int_interval	ru1 = eng_loadfromfile(configname, false);

	if (ru1.flags & ENG_ERROR_FLAG){
	 	fprintf(stderr, "Failed to load config\n"); // no function name there! Always  <N>task_func.c
	 	return 2;
	}

	eng_autoprint(ru1);

	printf("Check %s\n", eng_data.func_description);

	int cnt = eng_int_run(ru1);

    if (cnt > 0){
        printf("%d was/were found\n", cnt);
	}  else {
        printf("Not found for that interval...\n");
	}

	logclose("...");
	return 0;
}

