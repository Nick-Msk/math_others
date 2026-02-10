#include <math.h>
#include <float.h>

#include "bool.h"
#include "eng_launcher.h"
#include "eng_runner.h"
#include "log.h"

static inline bool 		checkfunc(double x){
	double res = x;
	if (res < -2)	// scope of definition
		return false;
	res = 2 - sqrt(2 + x);
	if (res < 0)
		return false;
	res = sqrt(2 - sqrt(2 + x) ) - x;
	return (fabs(res)) < DBL_EPSILON; // g_eps / 1000000;
}

// target func 3 DIM
bool 					myboolfunc(int x, int y, int z){
	if (z == 0 || y < 0)	// scope of definition
		return false;
	double val = ( (double) x + sqrt(y)) / (double) z;
	bool res = checkfunc(val);
	if (res && !eng_check_previous(val) ){
		logsimple("x = %d, y = %d, z = %d", x, y, z);
		return true;
	}
	else
		return false;	// no need to recalculate again
}

// as external
union eng_un_int_func eng_un_int_func_api = (union eng_un_int_func) {.f_int_3dim_bool = myboolfunc};

struct eng_launcher_data eng_data = (struct eng_launcher_data) {
		.func_description = "sqrt( 2 - sqrt(2+x) ) = x, when x=(a + sqrt(b))/c!, a,b,c: int", 
		.elogfile =  "log/19_func.log",
		.task_description = "task from zen 10.02.2025"
};

