#ifndef _ENG_RUNNGER_H
#define _ENG_RUNNGER_H			

#include <stdbool.h>
#include <stdio.h>

typedef bool    (*tf_int2dim)(int, int, long);
typedef bool	(*tf_int2dim_bool)(int, int);	//  no long here!

typedef bool	(*tf_int1dim)(int, long);
typedef bool	(*tf_int1dim_bool)(int);		// no long here!

struct eng_int_interval {
	int			fromX;	// for now only 2 dims are supported
	int			toX;
	int			fromY;
	int			toY;
	bool		targetValueFlag;	// TODO: put all the flags into 1 var via | op
	long		targetValue;	// if NOT bool function
	bool 		stopRun;
	bool		printFlag;
	int			modLog;		// for printing logs cnt % modLog == 0
	union {
		tf_int2dim		f_int_2dim;
		tf_int2dim_bool	f_int_2dim_bool;
		tf_int1dim		f_int_1dim;
		tf_int1dim_bool	f_int_1_bool;
	}
};

int								eng_int_1dim_bool(struct eng_int_interval rt);

int                             eng_int_2dim(struct eng_int_interval rt, long val);

int                             eng_check_int2dim_interval(struct eng_int_interval rt, long val_from, long val_to);

int								eng_fautoprint(FILE *f, struct eng_int_interval v);

static inline int				eng_autoprint(struct eng_int_interval v){
	return eng_fautoprint(stdout, v);
}

#endif /* _ENG_RUNNGER_H */


