#include <stdio.h>
#include <string.h>

#include "eng_runner.h"
#include "log.h"
#include "checker.h"

// interval
static void						skip_line(FILE *f){
	int c;
	while ( (c = fgetc(f)) != EOF && c != '\n')
		;
}

// construct from file
struct eng_int_interval        	*eng_loadfromfile(const char *cfgname, struct eng_int_interval *st,  bool strict /*now ignored */){
	logenter("from %s, st [%p]", cfgname, st);

	// TODO: replace to inv from checker
	/*if (st == 0){
		return logerr(st, "st is null");
	}*/
	if (!inv(st != 0, "st must be not null"))
		return logret(st, "");

	FILE *f = fopen(cfgname, "r");
	if (f == 0){
		st->flags |= ENG_ERROR_FLAG;
		return logerr(st, "unable to opern file");
	}
	char	name[100];
	long	value;
	int		c;
	// very simple method
	while ( (c = fgetc(f))!= EOF){
		if (c == '#')
			skip_line(f);
		else
			ungetc(c, f);
		if (fscanf(f, "%s = %ld\n", name, &value) == EOF)
			continue; 
		logmsg("name [%s], val [%ld]", name, value);
		// TODO: replace to macros
		if (strcasecmp(name, "DIM") == 0)
			st->useDim = value;	
		else if (strcasecmp(name, "FROMX") == 0)
			st->fromX = value;
		else if (strcasecmp(name, "TOX") == 0)
			st->toX = value;
		else if (strcasecmp(name, "STEPX") == 0)
			st->stepX = value;
		else if (strcasecmp(name, "FROMY") == 0)
			st->fromY = value;
		else if (strcasecmp(name, "TOY") == 0)
			st->toY = value;
		else if (strcasecmp(name, "STEPY") == 0)
			st->stepY = value;
		else if (strcasecmp(name, "FROMZ") == 0)
			st->fromZ = value;
		else if (strcasecmp(name, "TOZ") == 0)
			st->toZ = value;
		else if (strcasecmp(name, "STEPZ") == 0)
			st->stepZ = value;
		else if (strcasecmp(name, "FROMZ1") == 0)
			st->fromZ1 = value;
		else if (strcasecmp(name, "TOZ1") == 0)
			st->toZ1 = value;
		else if (strcasecmp(name, "STEPZ1") == 0)
			st->stepZ1 = value;
		else if (strcasecmp(name, "MODLOG") == 0)
			st->modLog = value;
		else if (strcasecmp(name, "TARGETVALUE") == 0){
			st->targetValue = value;
			st->flags |= ENG_TARGET_VALUE_FLAG;
		}
		else if (strcasecmp(name, "STOPRUN") == 0){
			if (value == 0)
				st -> flags &= ENG_STOP_RUN_FLAG;
			else
				st->flags |= ENG_STOP_RUN_FLAG;
		}
		else if (strcasecmp(name, "PRINTFLAG") == 0){
			if (value == 0)					
				st->flags &= ENG_PRINT_FLAG;
			else
				st->flags |= ENG_PRINT_FLAG;
		}
		else {
			st->flags |= ENG_ERROR_FLAG;
			fprintf(stderr, "Unsupported param %s\n", name);
		}
	}
	fclose(f);
	if (strict){
		// TODO: probable some checking here...
		if (st->useDim > 4 || st->useDim < 1){
			st->flags |= ENG_ERROR_FLAG;
			fprintf(stderr, "Dim %d is out of range (1-4)", st->useDim);
		}
	}
	eng_fautoprint(logfile, *st);
	return logret(st, "... is_ok - %s", bool_str(eng_fl_error(st->flags)));
}

// general int runner
int								eng_int_run(struct eng_int_interval rt){
	logenter("DIM %d", rt.useDim);

	int ret = 0;

	switch (rt.useDim){
		case 1:
			ret = eng_int_1dim(rt);
		break;
		case 2:
			ret = eng_int_2dim(rt);
		break;
		case 3:
			ret = eng_int_3dim(rt);
		break;
		case 4:
			ret = eng_int_4dim(rt);
		break;
		default:
			fprintf(stderr, "%s : unsupported DIM %d for integer runner\n", __func__, rt.useDim);
	}

	return logret(ret, "Total found %d", ret);
}

// for 1 dim int
int								eng_int_1dim(struct eng_int_interval rt){
	int						total = 0;
	static unsigned long	cnt = 0;
	bool					targetValueFlag = eng_fl_targetValue(rt.flags);
	bool					printFlag 		= eng_fl_print(rt.flags);
	bool					stopRun 		= eng_fl_stopRun(rt.flags);

    // iterator need to be here
    for (int x = rt.fromX; x <= rt.toX; x += rt.stepX){
		if (rt.modLog > 0 && printFlag && cnt++ % rt.modLog == 0)
			logsimple("cnt=%lu", cnt);
        if (
			(targetValueFlag && rt.f_int_1dim(x, rt.targetValue)) ||
			(!targetValueFlag && rt.f_int_1dim_bool(x))
		)
		{
            if (printFlag)
                printf("Target function(%d) is true ", x);
			if (targetValueFlag)
				printf("(for %ld)\n", rt.targetValue);
			else
				printf("\n");
			if (stopRun)
                return 1;
            else
                total++;
        }
	}
	return total;
}

int                             eng_int_2dim(struct eng_int_interval rt){
	int             		total = 0;
	static unsigned long	cnt = 0;
	bool					targetValueFlag = eng_fl_targetValue(rt.flags);
	bool					printFlag 		= eng_fl_print(rt.flags);
	bool					stopRun 		= eng_fl_stopRun(rt.flags);

    // iterator need to be here
    for (int x = rt.fromX; x <= rt.toX; x += rt.stepX)
    	for (int y = rt.fromY; y <= rt.toY; y += rt.stepY){
			if (rt.modLog > 0 && printFlag && cnt++ % rt.modLog == 0)
				logsimple("cnt=%lu", cnt);
        	if (
				(targetValueFlag && rt.f_int_2dim(x, y, rt.targetValue)) ||
				(!targetValueFlag && rt.f_int_2dim_bool(x, y))
			)
			{
            	if (printFlag)
                	printf("Target function(%d, %d) is true", x, y);
				if (targetValueFlag)
					printf("for %ld)\n", rt.targetValue);
				else
					printf("\n");
                if (stopRun)
                   	return 1;
                else
                    total++;
            }
        }
    return total;
}


int                             eng_int_3dim(struct eng_int_interval rt){
	int             		total = 0;
	static unsigned long	cnt = 0;
	bool					targetValueFlag = eng_fl_targetValue(rt.flags);
	bool					printFlag 		= eng_fl_print(rt.flags);
	bool					stopRun 		= eng_fl_stopRun(rt.flags);

    // iterator need to be here
    for (int x = rt.fromX; x <= rt.toX; x += rt.stepX)
    	for (int y = rt.fromY; y <= rt.toY; y += rt.stepY)
			for (int z = rt.fromZ; z <= rt.toZ; z += rt.stepZ){
					if (rt.modLog > 0 && printFlag && cnt++ % rt.modLog == 0)
						logsimple("cnt=%lu", cnt);
					if (
						(targetValueFlag && rt.f_int_3dim(x, y, z, rt.targetValue)) ||
						(!targetValueFlag && rt.f_int_3dim_bool(x, y, z))
					)
					{
						if (printFlag)
							printf("Target function(%d, %d, %d) is true", x, y, z);
						if (targetValueFlag)
							printf("for %ld)\n", rt.targetValue);
						else
							printf("\n");
						if (stopRun)
							return 1;
						else
							total++;
            		}
        	}
    return total;
}

int                             eng_int_4dim(struct eng_int_interval rt){
	int             		total = 0;
	static unsigned long	cnt = 0;
	bool					targetValueFlag = eng_fl_targetValue(rt.flags);
	bool					printFlag 		= eng_fl_print(rt.flags);
	bool					stopRun 		= eng_fl_stopRun(rt.flags);

    // iterator need to be here
    for (int x = rt.fromX; x <= rt.toX; x += rt.stepX)
    	for (int y = rt.fromY; y <= rt.toY; y += rt.stepZ)
			for (int z = rt.fromZ; z <= rt.toZ; z += rt.stepZ)
				for (int z1 = rt.fromZ1; z1 <= rt.toZ1; z1 += rt.stepZ1)
				{
							if (rt.modLog > 0 && printFlag && cnt++ % rt.modLog == 0)
								logsimple("cnt=%lu", cnt);
							if (
								(targetValueFlag && rt.f_int_4dim(x, y, z, z1, rt.targetValue)) ||
								(!targetValueFlag && rt.f_int_4dim_bool(x, y, z, z1))
							)
							{
								if (printFlag)
									printf("Target function(%d, %d, %d) is true", x, y, z);
								if (targetValueFlag)
									printf("for %ld)\n", rt.targetValue);
								else
									printf("\n");
								if (stopRun)
									return 1;
								else
									total++;
							}
				
        		}
    return total;
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
    	total += eng_int_2dim(rt);
	}
    return logret(total, "FOUND: %d", total);
}

int                             eng_fautoprint(FILE *f, struct eng_int_interval v){

	int 					cnt = 0;
	bool					targetValueFlag = eng_fl_targetValue(v.flags);
	bool					printFlag 		= eng_fl_print(v.flags);
	bool					stopRun 		= eng_fl_stopRun(v.flags);
	bool					errorFlag		= eng_fl_error(v.flags);
	
	cnt += fprintf(f, "%*cINT DIM=%d, targetValueFlag=%s, stopRun=%s, printFlag=%s, errorFlag=%s modLog=%d\n",
		f == logfile? logoffset : 0, '|',
		v.useDim, bool_str(targetValueFlag), bool_str(stopRun), bool_str(printFlag), bool_str(errorFlag), v.modLog);
	
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
		f == logfile? logoffset : 0, '|', v.useDim, targetValueFlag? "": "_bool", v.f_int_1dim /* anyway... */);
	
	if (targetValueFlag)
		cnt += fprintf(f, "%*c targetValue=%ld\n", logoffset, '|', v.targetValue);

	return cnt;
}
