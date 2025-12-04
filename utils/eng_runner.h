#ifndef _ENG_RUNNGER_H
#define _ENG_RUNNGER_H			

#include <stdbool.h>
#include <stdio.h>

typedef bool    (*tf_int2dim)(int, int, long);
typedef bool	(*tf_int1dim)(int, long);

struct eng_int_interval {
	int			fromX;	// for now only 2 dims are supported
	int			toX;
	int			fromY;
	int			toY;
	bool 		stopRun;
	bool		printFlag;
	int			modLog;		// for printing logs cnt % modLog == 0
	tf_int2dim	f_int_2dim;
	tf_int1dim	f_int_1dim;
};

int                             eng_int_2dim(struct eng_int_interval rt, long val);

int                             eng_check_int2dim_interval(struct eng_int_interval rt, long val_from, long val_to);

int								eng_fautoprint(FILE *f, struct eng_int_interval v);

static inline int				eng_autoprint(struct eng_int_interval v){
	return eng_fautoprint(stdout, v);
}

#endif /* _ENG_RUNNGER_H */


