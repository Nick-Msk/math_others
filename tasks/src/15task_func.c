#include "bool.h"
#include "eng_launcher.h"
#include "eng_runner.h"


// 1. target func
bool myboolfunc(int x, int y){
   return x + x * y + y == 7;
}

// as external
union eng_un_int_func eng_un_int_func_api = (union eng_un_int_func) {.f_int_2dim_bool = myboolfunc};

struct eng_launcher_data eng_data = (struct eng_launcher_data) {
		.func_description = "x+xy+y==7", 
		.elogfile =  "log/15_func.log",
		.task_description = "task from zen 20.01.2025"
};

/*
// 2. func desc
const char              *eng_func_description = "x+xy+y==7";

// 3. usage
//const char 				*eng_usage = " conf_file, target";

// 4. logfile 
const char				*eng_logfile = "log/15_func.log"; // very dirty, log.h should be changed!! TODO:

// 5. task description
const char 				*eng_task_description = "task from zen 20.01.2025";
*/

