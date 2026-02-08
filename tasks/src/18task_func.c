#include <math.h>
#include <float.h>

#include "bool.h"
#include "eng_launcher.h"
#include "eng_runner.h"
#include "log.h"

double			g_eps = FLT_EPSILON;	// for now

// TODO: use separate module
#define			 PREV_RES_CNT 	100
double			 prev_results[PREV_RES_CNT];
int				 pos = 0;

bool 					eng_check_previous(double val){
	for (int i = 0; i < pos; i++)
		if (fabs(val - prev_results[i]) < g_eps){
			//logsimple("value %f is again", val);
			return true;
		}
	if (pos < PREV_RES_CNT){
		prev_results[pos++] = val;	// save the value
		logsimple("val = %.20e", val);
	} else 
		fprintf(stderr, "Position is over %d\n", PREV_RES_CNT);
	return false;
}

// x2 - 2 = sqrt(x)
static inline bool 		checkfunc(double x){
	double res = x * x - 2 - sqrt(x + 2);
	//logsimple("ret = %f", res);
	return (fabs(res)) < DBL_EPSILON; // g_eps / 1000000;
}

// target func 3 DIM
// uble)
bool 					myboolfunc(int x, int y, int z){
	if (z == 0 || y < 0)	// scope of definition
		return false;
	double val = ( (double) x + sqrt(y)) / (double) z;
	bool res = checkfunc(val);
	if (res && !eng_check_previous(val) )
		return true;
	else
		return false;	// no need to recalculate again
}

// as external
union eng_un_int_func eng_un_int_func_api = (union eng_un_int_func) {.f_int_3dim_bool = myboolfunc};

struct eng_launcher_data eng_data = (struct eng_launcher_data) {
		.func_description = "x2 - 2 = sqrt(x+2), when x=(a + sqrt(b))/c!, a,b,c: int", 
		.elogfile =  "log/18_func.log",
		.task_description = "task from zen 08.02.2025"
};

