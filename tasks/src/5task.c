#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>

#include "check.h"
#include "log.h"

static  		double f_target_dbl(double x){
	double res = sqrt(sqrt(97 - x)) + sqrt(sqrt(x));
	return res; 	// == checked_val;	// 5
}

static bool		g_print_flag = true;

typedef 		double (*f_double1dim)(double);

// engine
int				eng_double_1dim(double fromx, double tox, double step, double val, bool stoprun, f_double1dim trg);

int				main(int argc, const char *argv[]){
	loginit("log/5task.log", false, 0, "Start");

	if (!check_arg(4, "Usage: %s from to step [value]\n", *argv))
        return 1;

	double fromx = atof(argv[1]);
	double tox = atof(argv[2]);
	double step = atof(argv[3]);
	double checked_val = 5.0;	// as per task

	if (argc > 4)
		checked_val = atof(argv[4]);

	printf("Start checking from %g to %g by %g f = %g, EPS = %g\n", 
					fromx, tox, step, checked_val, DBL_EPSILON);

	int cnt = eng_double_1dim(fromx, tox, step, checked_val, false, f_target_dbl);

	if (cnt > 0)
		printf("Found %d\n", cnt);
	else
		printf("Not found\n");

	logclose("...");
	return 0;
}

static inline int			d_between(double v1, double v2){
	if (fabs(v1 - v2) < DBL_EPSILON * 1.0e+6)
		return 1;
	else
		return 0;
}

// 1-dim double iterator
int				eng_double_1dim(double fromx, double tox, double step, double checked_val, bool stoprun, f_double1dim trg){
	
	logsimple("Starting...");
	
	int count = 0;
	double prev_i = fromx, i;

	//  iterator : TODO:
	for (i = fromx; i <= tox; i += step){
		
		double r = trg(i);
		if (trunc(prev_i) < trunc(i)){
			logsimple("i = %g f() = [%g] checked [%g]", i, r, checked_val);
			prev_i = i;
		}

		if (d_between(r, checked_val)){
			count++;
			if (g_print_flag)
				printf("f(%g) == %g (%g)\n", i, r, checked_val);
			if (stoprun)
				return count;
		}
	}

	return count;
}


