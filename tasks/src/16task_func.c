#include "bool.h"
#include "eng_launcher.h"
#include "eng_runner.h"

static long fact(long x){
	long res = 1;
	while (x > 1)
		res *= x--;
	return res;
}


// target func 1 DIM
bool myboolfunc(int x){
   return  fact(x) == x * x * x - x;
}

// as external
union eng_un_int_func eng_un_int_func_api = (union eng_un_int_func) {.f_int_1dim_bool = myboolfunc};

struct eng_launcher_data eng_data = (struct eng_launcher_data) {
		.func_description = "x! = x3 - x", 
		.elogfile =  "log/16_func.log",
		.task_description = "task from zen 01.02.2025"
};

