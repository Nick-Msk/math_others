#include <math.h>
#include <float.h>

#include "bool.h"
#include "eng_launcher.h"
#include "eng_runner.h"
#include "log.h"

// must be equal to 0
static inline bool 		checkfunc(double x){
	double res = x * x * x * x - 8 * x * x * x + 17 * x * x - 8 * x + 1;
	return (fabs(res)) < DBL_EPSILON * 1000; // g_eps / 1000000;
}

// target func 4 DIM
bool 					myboolfunc(int x, int y, int z){
	char 		buf[200];	// more that enought

	if (z == 0 || y < 0)	// scope of definition
		return false;
	// calculation
	double val = ( (double) x + sqrt(y)) / (double) z;

	bool res = checkfunc(val);
	if (res){
		snprintf(buf, sizeof(buf), "(%d + sqrt(%d)) / %d", x, y, z);		// TODO: rework that to avoid multiple useless sprintf
		if (!eng_check_previous(val, buf, sizeof(buf) ) ){
			logsimple("(%d + sqrt(%d)) / %d", x, y, z);
			return true;
		}
	}
	return false;	// no need to recalculate again
}

// as external
union eng_un_int_func eng_un_int_func_api = (union eng_un_int_func) {.f_int_3dim_bool = myboolfunc};

struct eng_launcher_data eng_data = (struct eng_launcher_data) {
		.func_description = "x4 - 8x3 + 17x2 - 8x + 1 = 0, when x=(a + sqrt(b))/c !, a,b,c: int", 
		.elogfile =  "log/22_func.log",
		.task_description = "task 22 from zen 15.02.2025"
};

