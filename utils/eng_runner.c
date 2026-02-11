#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "eng_runner.h"
#include "log.h"
#include "checker.h"

double                  		g_eng_eps = FLT_EPSILON;    // for now


#if defined(__clang__)
	static const int			ENG_PREV_RES_CNT = 100;
	static const int			ENG_MAXMSG 		 = 128; 
#else /* __GNUC__ */
	#define                  	ENG_PREV_RES_CNT   100
	#define						ENG_MAXMSG 		   128
#endif

struct eng_resultset {
	double 	val;
	char	msg[ENG_MAXMSG];
};

static struct eng_resultset     prev_results[ENG_PREV_RES_CNT];
static int                      pos_result = 0;

bool                            eng_check_previous(double val, const char *msg, int sz){
    for (int i = 0; i < pos_result; i++)
        if (fabs(val - prev_results[i].val) < g_eng_eps){
 	       	//logsimple("value %f is again", val);
            return true;
        }
    if (pos_result < ENG_PREV_RES_CNT){
		struct eng_resultset res = {.val = val};
		strncpy(res.msg, msg, sz);
        prev_results[pos_result++] = res;      // save the value
        logsimple("val = %.20e", val);
    } else
        fprintf(stderr, "Position is over %d\n", ENG_PREV_RES_CNT);
    return false;
}

void							eng_check_reset(void){
	pos_result = 0;
}

// interval
static void						skip_line(FILE *f){
	int c;
	while ( (c = fgetc(f)) != EOF && c != '\n')
		;
}

// construct from file
struct eng_int_interval        	eng_loadfromfile(const char *cfgname, bool strict /*now ignored */){
	logenter("from %s, strict [%s]", cfgname, bool_str(strict));

	// use defaults
	struct eng_int_interval st =  eng_create_int();

	FILE *f = fopen(cfgname, "r");
	if (!inv(f != 0, "unable to opern file") ){
		st.flags |= ENG_ERROR_FLAG;
		return logerr(st, "unable to opern file");
	}

	char	name[100], val[200];
	long	value;
	int		c;
	// very simple method
	while ( (c = fgetc(f))!= EOF){
		if (c == '#')
			skip_line(f);
		else
			ungetc(c, f);
		if (fscanf(f, "%s = %s\n", name, val) == EOF)
			continue; 
		value = atol(val);
		logmsg("name [%s], value [%ld], val [%s]", name, value, val);
		// TODO: replace to macros
		if (strcasecmp(name, "DIM") == 0)
			st.useDim = value;	
		else if (strcasecmp(name, "FROMX") == 0)
			st.fromX = value;
		else if (strcasecmp(name, "TOX") == 0)
			st.toX = value;
		else if (strcasecmp(name, "STEPX") == 0)
			st.stepX = value;
		else if (strcasecmp(name, "FROMY") == 0)
			st.fromY = value;
		else if (strcasecmp(name, "TOY") == 0)
			st.toY = value;
		else if (strcasecmp(name, "STEPY") == 0)
			st.stepY = value;
		else if (strcasecmp(name, "FROMZ") == 0)
			st.fromZ = value;
		else if (strcasecmp(name, "TOZ") == 0)
			st.toZ = value;
		else if (strcasecmp(name, "STEPZ") == 0)
			st.stepZ = value;
		else if (strcasecmp(name, "FROMZ1") == 0)
			st.fromZ1 = value;
		else if (strcasecmp(name, "TOZ1") == 0)
			st.toZ1 = value;
		else if (strcasecmp(name, "STEPZ1") == 0)
			st.stepZ1 = value;
		else if (strcasecmp(name, "MODLOG") == 0)
			st.modLog = value;
		else if (strcasecmp(name, "TARGETVALUE") == 0){
			st.targetValue = value;
			st.flags |= ENG_TARGET_VALUE_FLAG;
		}
		else if (strcasecmp(name, "STOPRUN") == 0){
			if (value == 0)
				st.flags &= ENG_STOP_RUN_FLAG;
			else
				st.flags |= ENG_STOP_RUN_FLAG;
		}
		else if (strcasecmp(name, "PRINTFLAG") == 0){
			if (value == 0)					
				st.flags &= ENG_PRINT_FLAG;
			else
				st.flags |= ENG_PRINT_FLAG;
		}
		else if (strcasecmp(name, "PRINT_MSG") == 0)
			strncpy(st.print_msg, val, sizeof(st.print_msg));
		else {
			st.flags |= ENG_ERROR_FLAG;
			fprintf(stderr, "Unsupported param %s\n", name);
		}
	}
	fclose(f);
	if (strict){
		// TODO: probable some checking here...
		if (st.useDim > 4 || st.useDim < 1){
			st.flags |= ENG_ERROR_FLAG;
			fprintf(stderr, "Dim %d is out of range (1-4)", st.useDim);
		}
	}
	eng_fautoprint(logfile, st);
	return logret(st, "... is_ok - %s", bool_str(eng_fl_error(st.flags)));
}

// utiversal wrapper for integer
static bool						wrap_int(struct eng_int_interval rt){
	bool					res = false;
	int						dim = rt.useDim;
	bool					targetValueFlag = eng_fl_targetValue(rt.flags);
	// TODO:
	switch (dim){
		case 1:
			if (targetValueFlag)
				res = rt.f.f_int_1dim(rt.iterX, rt.targetValue);
			else
				res = rt.f.f_int_1dim_bool(rt.iterX);
		break;
		case 2:	
			if (targetValueFlag)
				res = rt.f.f_int_2dim(rt.iterX, rt.iterY, rt.targetValue);
			else
				res = rt.f.f_int_2dim_bool(rt.iterX, rt.iterY);
		break;
		case 3:
			if (targetValueFlag)
				res = rt.f.f_int_3dim(rt.iterX, rt.iterY, rt.iterZ, rt.targetValue);
			else
				res = rt.f.f_int_3dim_bool(rt.iterX, rt.iterY, rt.iterZ);
		break;
		case 4:
			if (targetValueFlag)
				res = rt.f.f_int_4dim(rt.iterX, rt.iterY, rt.iterZ, rt.iterZ1, rt.targetValue);
			else
				res = rt.f.f_int_4dim_bool(rt.iterX, rt.iterY, rt.iterZ, rt.iterZ1);
		break;
		default:
			fprintf(stderr, "Unsupported DIM %d\n", dim);
			// userraiseerr() TODO:
		break;
	}
	return res;
}

// util, static
static int						find_greatest(struct eng_int_interval rt){
	int 	greatest = MAX(abs(rt.fromX), abs(rt.toX) );
	if (rt.useDim > 1)
		greatest = GREATEST(greatest, abs(rt.fromY), abs(rt.toY) );
	if (rt.useDim > 2)
		greatest = GREATEST(greatest, abs(rt.fromZ), abs(rt.toZ));
	if (rt.useDim)
		greatest = GREATEST(greatest, abs(rt.fromZ1), abs(rt.toZ1));
	return logautoret(greatest);  // logret(greatest, "gr: %d", greatest); // 
}

// general int runner
int								eng_int_run(struct eng_int_interval rt){
	logenter("DIM %d", rt.useDim);

	int 					total = 0;
	static unsigned long	cnt = 0;
	bool					printFlag 		= eng_fl_print(rt.flags);
	bool					stopRun 		= eng_fl_stopRun(rt.flags);
	bool					targetValueFlag = eng_fl_targetValue(rt.flags);
	int greatest = find_greatest(rt);
	logmsg("radius till %d", greatest);

	if (rt.useDim < 2)
		rt.fromY = rt.toY = 0;	// to make ONLY 1 cycle
		
	if (rt.useDim < 3)
		rt.fromZ = rt.toZ = 0;	// to make ONLY 1 cycle
	
	if (rt.useDim < 4)
		rt.fromZ1 = rt.toZ1 = 0;	// to make ONLY 1 cycle

	// iterator need to be here
	for (int radius = 0; radius <= greatest; radius += rt.stepX){	// 1 step for ALL dums!
		if (rt.modLog > 0 && printFlag && cnt++ % rt.modLog == 0)
			logsimple("cnt=%lu, radius %d", cnt, radius);
		// go throws dimensions																				
		// iterator need to be here
		for (rt.iterX = MAX(-radius, rt.fromX); rt.iterX <= MIN(radius, rt.toX); rt.iterX += rt.stepX)
		{
			for (rt.iterY = MAX(-radius, rt.fromY); rt.iterY <= MIN(radius, rt.toY); rt.iterY += rt.stepY)
			{
				for (rt.iterZ = MAX(-radius, rt.fromZ); rt.iterZ <= MIN(radius, rt.toZ); rt.iterZ += rt.stepZ)
				{
					for (rt.iterZ1 = MAX(-radius, rt.fromZ1); rt.iterZ1 <= MIN(radius, rt.toZ1); rt.iterZ1 += rt.stepZ1)
					{
						// exec wrapper here!
						// for ALL dims
						bool res = wrap_int(rt);
						if (res){
							if (printFlag){
								logmsg("total %d, pos_result %d", total, pos_result);
								if (total == pos_result - 1)
									printf("%s\n", 	prev_results[pos_result - 1].msg);
								if (strlen(rt.print_msg) > 0) { // that's a bit stupid, my understanding all messages must be via prev_result[].msg 
									printf("Target function(%d, ", rt.iterX);
									if (rt.useDim > 1)
										printf("%d, ", rt.iterY);
									if (rt.useDim > 2)
										printf("%d, ", rt.iterZ);
									if (rt.useDim > 3)
										printf("%d ", rt.iterZ1);
									printf(" is true ");
								}
								if (targetValueFlag)
									printf("(for %ld)\n", rt.targetValue);
								else
									printf("\n");
							}
							if (stopRun)
		    	            	return 1;
        			   		else
            		   			logauto(total++);
						}
					}
				}
			}
		}
	}	

	return logret(total, "Total found %d", total);
}

// INTERVAL version, TODO: rework to normal version with decreasing stepX when neccessary
int                             eng_flt_1dim(struct eng_flt_interval rt){
	int						total = 0;
	static unsigned long	cnt = 0;
	bool					start = false;	//true when interval is started
	bool					stop = false;	//true when interval is ended
	bool					in = false;

	bool					targetValueFlag = eng_fl_targetValue(rt.flags);
	bool					printFlag 		= eng_fl_print(rt.flags);
	bool					stopRun 		= eng_fl_stopRun(rt.flags);

	// 1 dim iterator
	for (double x = rt.fromX; x <= rt.toX; x += rt.stepX){
		// just regular logging
		if (rt.modLog > 0 && printFlag && cnt++ % rt.modLog == 0)
			logsimple("cnt=%lu x=%g interval=%d", cnt - 1, x, in);
		
        if (
			(targetValueFlag && rt.f_flt_1dim(x, rt.targetValue)) ||
			(!targetValueFlag && rt.f_flt_1dim_bool(x))
			)
			{
				if (!in)
					in = true, start = true;	// start interval	
			}
        else 	// run is failed
		    if (in)
				in = false, stop = true;

		if (start){
			start = false;
	        if (stopRun)
               	return 1;
			else
				total++;
			// 
	        if (printFlag){
    	       	printf("Func is true");
				if (targetValueFlag)
					printf(" for %g\n", rt.targetValue);
				printf("[%g - \n", x);
			}
			start = false;	// 1 time per interval
		}
		if (stop){
			stop = false;
			if (printFlag)
				printf(" %g]\n", x - rt.stepX);	// when stepX is constant
		}
	}

	if (in && printFlag)
		printf(" AND MORE]\n");
	return total;
}

int                             eng_flt_2dim(struct eng_flt_interval rt){
	int						total = 0;
	static unsigned long	cnt = 0;

	bool					targetValueFlag = eng_fl_targetValue(rt.flags);
	bool					printFlag 		= eng_fl_print(rt.flags);
	bool					stopRun 		= eng_fl_stopRun(rt.flags);

	// 2 dim iterator
	for (double x = rt.fromX; x <= rt.toX; x += rt.stepX)
		for (double y = rt.fromY; y < rt.toY; y += rt.stepY)
			{
				// just regular logging
				if (rt.modLog > 0 && printFlag && cnt++ % rt.modLog == 0)
					logsimple("cnt=%lu, x=%g y=%g", cnt - 1, x, y);
				
				if (
					(targetValueFlag && rt.f_flt_2dim(x, y, rt.targetValue)) ||
					(!targetValueFlag && rt.f_flt_2dim_bool(x, y))
					)
					{
						if (printFlag){
							printf("Func is true");
	
							if (targetValueFlag)
								printf(" for %g", rt.targetValue);
							printf(": x=%g y=%g\n", x, y);
						}

						if (stopRun)
							return 1;
						else
							total++;
					}
			}

	return total;
}

int                             eng_check_int2dim_interval(struct eng_int_interval rt, long val_from, long val_to){
	logenter("from %ld, to %ld", val_from, val_to);
	eng_fautoprint(logfile, rt);
	int total = 0;
    for (long i = val_from; i <= val_to; i++){
		rt.targetValue = i;	
    	total += eng_int_run(rt);
	}
    return logret(total, "FOUND: %d", total);
}

int                             eng_fautoprint(FILE *f, struct eng_int_interval v){

	int 					cnt = 0;
	bool					targetValueFlag = eng_fl_targetValue(v.flags);
	bool					printFlag 		= eng_fl_print(v.flags);
	bool					stopRun 		= eng_fl_stopRun(v.flags);
	bool					errorFlag		= eng_fl_error(v.flags);
	
	cnt += fprintf(f, "%*cINT DIM=%d, targetValueFlag=%s, stopRun=%s, printFlag=%s, errorFlag=%s, modLog=%d, print_msg=%s\n",
		f == logfile? logoffset : 0, '|',
		v.useDim, bool_str(targetValueFlag), bool_str(stopRun), bool_str(printFlag), bool_str(errorFlag), v.modLog, v.print_msg);
	
	cnt += fprintf(f, "%*cfromX=%d, toX=%d, stepX=%d\n",
		f == logfile? logoffset : 0, '|', v.fromX, v.toX, v.stepX);
	
	if (v.useDim > 1)
		cnt += fprintf(f, "%*cfromY=%d, toY=%d, stepY=%d\n",
			f == logfile? logoffset : 0, '|', v.fromY, v.toY, v.stepY);
		
	if (v.useDim > 2)
		cnt += fprintf(f, "%*cfromZ=%d, toZ=%d, stepZ=%d\n",
			f == logfile? logoffset : 0, '|', v.fromZ, v.toZ, v.stepZ);

	if (v.useDim > 3)
		cnt += fprintf(f, "%*cfromZ1=%d, toZ1=%d, stepZ1=%d\n",
			f == logfile? logoffset : 0, '|', v.fromZ1, v.toZ1, v.stepZ1);
	cnt += fprintf(f, "%*cf_int%ddim%s=%p\n",
		f == logfile? logoffset : 0, '|', v.useDim, targetValueFlag? "": "_bool", v.f.f_int_1dim /* anyway... */);
	
	if (targetValueFlag)
		cnt += fprintf(f, "%*c targetValue=%ld\n", logoffset, '|', v.targetValue);

	return cnt;
}
