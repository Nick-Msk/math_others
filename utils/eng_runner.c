#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "eng_runner.h"
#include "log.h"
#include "checker.h"

double                  		g_eng_eps = FLT_EPSILON;    // for now


#if defined(__clang__)
	static const int			ENG_MAX_PROBE_PER_DIM 	= 16;
	static const int			ENG_PREV_RES_CNT 		= 100;
	static const int			ENG_MAXMSG 		 		= 256; 
#else /* __GNUC__ */
	#define                  	ENG_PREV_RES_CNT   		100
	#define						ENG_MAXMSG 		   		256
	#define						ENG_MAX_PROBE_PER_DIM 	16
#endif

// PROBE FUNCS
// particular probe func
typedef struct eng_probe {
	int				dim;	// level
	bool			(* func)(eng_int_interval v);	// point to probe wrapper   for ex for 2 dim - f(rt) = x1 + sqrt(x2);
	char			text[ENG_MAXMSG];	// text of func with dims of %d, for ex for 2 dim - "%d + sqrt(%d)"
} eng_probe;

// TODO: iterate probe for all function on particular level
typedef struct eng_dim_probe {
	int				dim;	// dim for all probe functions
	eng_probe		funcs[ENG_MAX_PROBE_PER_DIM];	// 16
} eng_dim_probe;


// ------------------------ RESULTSET ---------------------------------------------------
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

// ------------------------------- LOAD CONFIG (FROM FILE NOW, SHOULD BE FROM  SQLITE -------------------
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
	struct eng_int_interval st =  eng_create_int(0);

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
		// for NOW only 4 dims are supported from config file
		if (strcasecmp(name, "DIM") == 0)
			st.useDim = value;	
		else if (strcasecmp(name, "FROMX") == 0)
			st.dim[0].from = value;
		else if (strcasecmp(name, "TOX") == 0)
			st.dim[0].to = value;
		else if (strcasecmp(name, "STEPX") == 0)
			st.dim[0].step = value;
		else if (strcasecmp(name, "FROMY") == 0)
			st.dim[1].from = value;
		else if (strcasecmp(name, "TOY") == 0)
			st.dim[1].to = value;
		else if (strcasecmp(name, "STEPY") == 0)
			st.dim[1].step = value;
		else if (strcasecmp(name, "FROMZ") == 0)
			st.dim[2].from = value;
		else if (strcasecmp(name, "TOZ") == 0)
			st.dim[2].to = value;
		else if (strcasecmp(name, "STEPZ") == 0)
			st.dim[2].step = value;
		else if (strcasecmp(name, "FROMZ1") == 0)
			st.dim[3].from = value;
		else if (strcasecmp(name, "TOZ1") == 0)
			st.dim[3].to = value;
		else if (strcasecmp(name, "STEPZ1") == 0)
			st.dim[3].step = value;
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
		if (st.useDim > 10 || st.useDim < 1){
			st.flags |= ENG_ERROR_FLAG;
			fprintf(stderr, "Dim %d is out of range (1-4)", st.useDim);
		}
	}
	eng_fautoprint(logfile, st);
	return logret(st, "... is_ok - %s", bool_str(eng_fl_error(st.flags)));
}

// -------------------------------- ENGINE --------------------------------------------
//
// utiversal wrapper for integer

static bool						wrap_int(struct eng_int_interval rt){
	bool					res = false;
	int						dim = rt.useDim;
	bool					targetValueFlag = eng_fl_targetValue(rt.flags);
	// TODO:
	switch (dim){
		case 1:
			if (targetValueFlag)
				res = rt.f.f_int_1dim(rt.dim[0].iter, rt.targetValue);
			else
				res = rt.f.f_int_1dim_bool(rt.dim[0].iter);
		break;
		case 2:	
			if (targetValueFlag)
				res = rt.f.f_int_2dim(rt.dim[0].iter, rt.dim[1].iter, rt.targetValue);
			else
				res = rt.f.f_int_2dim_bool(rt.dim[0].iter, rt.dim[1].iter);
		break;
		case 3:
			if (targetValueFlag)
				res = rt.f.f_int_3dim(rt.dim[0].iter, rt.dim[1].iter, rt.dim[2].iter, rt.targetValue);
			else
				res = rt.f.f_int_3dim_bool(rt.dim[0].iter, rt.dim[1].iter, rt.dim[2].iter);
		break;
		case 4:
			if (targetValueFlag)
				res = rt.f.f_int_4dim(rt.dim[0].iter, rt.dim[1].iter, rt.dim[2].iter, rt.dim[3].iter, rt.targetValue);
			else
				res = rt.f.f_int_4dim_bool(rt.dim[0].iter, rt.dim[1].iter, rt.dim[2].iter, rt.dim[3].iter);
		break;
		case 5:
			if (targetValueFlag)
				res = rt.f.f_int_5dim(rt.dim[0].iter, rt.dim[1].iter, rt.dim[2].iter, rt.dim[3].iter, rt.dim[4].iter, rt.targetValue);
			else
				res = rt.f.f_int_5dim_bool(rt.dim[0].iter, rt.dim[1].iter, rt.dim[2].iter, rt.dim[3].iter, rt.dim[4].iter);
		break;
		case 6:
			if (targetValueFlag)
				res = rt.f.f_int_6dim(rt.dim[0].iter, rt.dim[1].iter, rt.dim[2].iter, 
									rt.dim[3].iter, rt.dim[4].iter, rt.dim[5].iter, rt.targetValue);
			else
				res = rt.f.f_int_6dim_bool(rt.dim[0].iter, rt.dim[1].iter, rt.dim[2].iter, 
									rt.dim[3].iter, rt.dim[4].iter, rt.dim[5].iter);
		break;
		case 7:
			if (targetValueFlag)
				res = rt.f.f_int_7dim(rt.dim[0].iter, rt.dim[1].iter, rt.dim[2].iter, 
									rt.dim[3].iter, rt.dim[4].iter, rt.dim[5].iter, rt.dim[6].iter, rt.targetValue);
			else
				res = rt.f.f_int_7dim_bool(rt.dim[0].iter, rt.dim[1].iter, rt.dim[2].iter, 
									rt.dim[3].iter, rt.dim[4].iter, rt.dim[5].iter, rt.dim[6].iter);
		break;
		case 8:
			if (targetValueFlag)
				res = rt.f.f_int_8dim(rt.dim[0].iter, rt.dim[1].iter, rt.dim[2].iter, 
									rt.dim[3].iter, rt.dim[4].iter, rt.dim[5].iter, rt.dim[6].iter, rt.dim[7].iter, rt.targetValue);
			else
				res = rt.f.f_int_8dim_bool(rt.dim[0].iter, rt.dim[1].iter, rt.dim[2].iter, 
									rt.dim[3].iter, rt.dim[4].iter, rt.dim[5].iter, rt.dim[6].iter, rt.dim[7].iter);
		break;
		case 9:
			if (targetValueFlag)
				res = rt.f.f_int_9dim(rt.dim[0].iter, rt.dim[1].iter, rt.dim[2].iter, 
									rt.dim[3].iter, rt.dim[4].iter, rt.dim[5].iter, rt.dim[6].iter, rt.dim[7].iter, rt.dim[8].iter, rt.targetValue);
			else
				res = rt.f.f_int_9dim_bool(rt.dim[0].iter, rt.dim[1].iter, rt.dim[2].iter, 
									rt.dim[3].iter, rt.dim[4].iter, rt.dim[5].iter, rt.dim[6].iter, rt.dim[7].iter, rt.dim[8].iter);
		break;
		case 10:
			if (targetValueFlag)
				res = rt.f.f_int_10dim(rt.dim[0].iter, rt.dim[1].iter, rt.dim[2].iter, 
									rt.dim[3].iter, rt.dim[4].iter, rt.dim[5].iter, rt.dim[6].iter, 
									rt.dim[7].iter, rt.dim[8].iter, rt.dim[9].iter, rt.targetValue);
			else
				res = rt.f.f_int_10dim_bool(rt.dim[0].iter, rt.dim[1].iter, rt.dim[2].iter, 
									rt.dim[3].iter, rt.dim[4].iter, rt.dim[5].iter, rt.dim[6].iter, 
									rt.dim[7].iter, rt.dim[8].iter, rt.dim[9].iter);
		break;
		default:
			fprintf(stderr, "Unsupported DIM %d\n", dim);
			userraiseint(11, "Unsupported DIM %d\n", dim);
		break;
	}
	return res;
}

// util, static
static int						find_greatest(struct eng_int_interval rt){
	int 	greatest = 0;
							
	for (int i = 1; i <= rt.useDim; i++)
		greatest = GREATEST(greatest, abs(rt.dim[i - 1].from), abs(rt.dim[i - 1].to) );	

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
	int 					greatest = find_greatest(rt);
	int 					to[ENG_DIM_CAP - 1];
	int 					from[ENG_DIM_CAP - 1];
	logmsg("radius till %d", greatest);

	// setup DIMS:
	for (int i = 1; i <= ENG_DIM_CAP; i++){ // 1 till 10
		if (i > rt.useDim)
			rt.dim[i - 1].from = rt.dim[i - 1].to = 0;	// cleanup!
	}


	// iterator need to be here, go up to the radius
	for (int radius = 0; radius <= greatest; radius += 1){	// 1 step for ALL dums!
		if (rt.modLog > 0 && printFlag && cnt++ % rt.modLog == 0)
			logmsg("cnt=%lu, radius %d, gr %d", cnt, radius, greatest);

		for (int i = 0; i < ENG_DIM_CAP; i++){ // 1 till 10
			// setup limits
			to[i] 	= MIN(radius, rt.dim[i].to);
			from[i] = MAX(-radius, rt.dim[i].from);
			logmsg("to[%d] = %d, from[%d] = %d", i, to[i], i, from[i]);
		}

#define iterdim(d) rt.dim[d].iter = from[d]; rt.dim[d].iter <= to[d]; rt.dim[d].iter += rt.dim[d].step

		// go throws dimensions																				
		// iterator need to be here
		// DIM == 1
		for (iterdim(0) )
			// DIM == 2
			for (iterdim(1))
				for (iterdim(2))
					for (iterdim(3))
						for (iterdim(4))
							for (iterdim(5))
								for (iterdim(6))
									for (iterdim(7))
										for (iterdim(8))
											for (iterdim(9))
											{	// remove that logging after												
												if (rt.modLog > 0 && printFlag && cnt++ % rt.modLog == 0)
													logsimple("cnt=%lu, x0 %d x1 %d x2 %d x3 %d x4 %d x5 %d x6 %d x7 %d x8 %d x9 %d", cnt, 
																		rt.dim[0].iter, 
																		rt.dim[1].iter,
																		rt.dim[2].iter, 
																		rt.dim[3].iter, 
																		rt.dim[4].iter, 
																		rt.dim[5].iter, 
																		rt.dim[6].iter,
																		rt.dim[7].iter,
																		rt.dim[8].iter,
																		rt.dim[9].iter);
												// exec wrapper here!
												// for ALL dims
												bool res = wrap_int(rt);
												if (res){
													if (printFlag){
														// TODO:  refactor that!
														logmsg("total %d, pos_result %d", total, pos_result);
														if (total == pos_result - 1)
															printf("%s\n", 	prev_results[pos_result - 1].msg);
														if (strlen(rt.print_msg) > 0) { // that's a bit stupid, my understanding all messages must be via prev_result[].msg 
															printf("Target function(");
															for (int i = 0; i < ENG_DIM_CAP; i++)
																printf("%d, ", rt.dim[i].iter);	// current iter pos
															printf(") is true ");
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

	for (int i = 0; i < v.useDim; i++)
		cnt += fprintf(f, "%*d: from=%d, to=%d, step=%d iter=%d\n",
			f == logfile? logoffset : 0, i + 1, v.dim[i].from, v.dim[i].to, v.dim[i].step, v.dim[i].iter);
	
	cnt += fprintf(f, "%*cf_int%ddim%s=%p\n",
		f == logfile? logoffset : 0, '|', v.useDim, targetValueFlag? "": "_bool", v.f.f_int_1dim /* anyway... */);
	
	if (targetValueFlag)
		cnt += fprintf(f, "%*c targetValue=%ld\n", 
		f == logfile? logoffset : 0, '|', v.targetValue);

	return cnt;
}
