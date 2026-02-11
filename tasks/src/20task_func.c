#include <math.h>
#include <float.h>

#include "bool.h"
#include "eng_launcher.h"
#include "eng_runner.h"
#include "log.h"

static inline bool 		checkfunc(double x){
	double res = x * x * x + 3 * x + 5 * sqrt(2);
	return (fabs(res)) < DBL_EPSILON * 10; // g_eps / 1000000;
}

// target func 4 DIM
bool 					myboolfunc(int x, int y, int z, int z1){
	char 		buf[100];	// more that enought

	if (z1 == 0 || z < 0)	// scope of definition
		return false;
	// calculation
	double val = ( (double) x + y * sqrt(z)) / (double) z1;

	bool res = checkfunc(val);
	if (res){
		sprintf(buf, "(%d + %d*sqrt(%d)) / %d", x, y, z, z1);
		if (!eng_check_previous(val, buf, sizeof(buf) ) ){
			logsimple("(%d + %d*sqrt(%d)) / %d", x, y, z , z1);
			return true;
		}
	}
	return false;	// no need to recalculate again
}

// as external
union eng_un_int_func eng_un_int_func_api = (union eng_un_int_func) {.f_int_4dim_bool = myboolfunc};

struct eng_launcher_data eng_data = (struct eng_launcher_data) {
		.func_description = "x3 +3x + 5sqrt(2) = 0, when x=(a + b*sqrt(c))/d !, a,b,c,d: int", 
		.elogfile =  "log/20_func.log",
		.task_description = "task from zen 11.02.2025"
};

