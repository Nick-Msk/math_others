#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

int			find_values(double, double, double, double);	

int			main(int argc, const char *argv[]){
	if (argc < 4){
		fprintf(stderr, "Usage: %s from to step\n", *argv);
		return 1;
	}

	double dfrom = atof(argv[1]);
	double dto = atof(argv[2]);
	double step = atof(argv[3]);
	double result_val = 2025.0;	// default
							
	if (argc > 4)	// try to get result from input
		result_val = atof(argv[4]);

	if (step < 0 || dfrom > dto){
		fprintf(stderr, "'From' %f less than 'to' %f or step %f is negative\n", dfrom, dto, step);
		return 2;
	}

	printf("start with [%g - %g]/%g, checked values is %g\n", dfrom, dto, step, result_val);

	int cnt = find_values(dfrom, dto, step, result_val);

	if (cnt > 0)
		printf("Test's passed\n");
	else
		printf("Test's failed\n");

	return 0;
}

static inline double		checked_f1(double x){
	double r = floor(x + floor(2 * x + floor(3 * x)));
	return r;
}

static inline int			between(double v1, double v2){
	if (fabs(v1 - v2) < DBL_EPSILON)
		return 1;
	else
		return 0;
}

int							find_values(double from, double to, double step, double res){
	int 	cnt = 0;
	int		start_interval = 0;	// 1 - when interval is started
	int		stop_interval = 0;	// 1 - end of interval							
	int		in_interval = 0;
		
	for (double v = from; v <= to; v += step){
		double 	r = checked_f1(v);
		if (between(r, res) /*r == res*/){ // better to use epsilon 
			cnt++;
			if (!in_interval)
				in_interval = 1, start_interval = 1;
			//printf("f1(%g) == %g\n", v, res);	
		}
		else { 
			if (in_interval)
				in_interval = 0, stop_interval = 1;
			//	printf("FAILED: f1(%g) == %g, diff = %g\n", v, r,  r - res);
		}
		if (start_interval == 1){
			printf("f1(%g) == %g [%g - ", v, res, v);
			start_interval = 0;
		}
		if (stop_interval == 1){
			printf("%g]\n", v - step); // because now v + step
			stop_interval = 0;
		}
	}
	if (in_interval)
		printf(" AND MORE]\n");
	return cnt;
}	




