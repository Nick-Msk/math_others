#include "bool.h"
#include "eng_launcher.h"
#include "eng_runner.h"

// target func 2 DIM
bool myboolfunc(int x, int y){
	if (x == 0 || y == 0)
		return false;
   	else
		return  3 * y + 4 * x == 5 * x * y;
}

// as external
union eng_un_int_func eng_un_int_func_api = (union eng_un_int_func) {.f_int_2dim_bool = myboolfunc};

struct eng_launcher_data eng_data = (struct eng_launcher_data) {
		.func_description = "3/x + 4/y = 5", 
		.elogfile =  "log/17_func.log",
		.task_description = "task from zen 06.02.2025"
};

