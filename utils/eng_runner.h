#ifndef _ENG_RUNNGER_H
#define _ENG_RUNNGER_H			

#include <stdbool.h>
#include <stdio.h>

typedef bool    (*f_int2dim)(int, int, long);

struct eng_int_interval {
	int			fromX;	// for now only 2 dims are supported
	int			toX;
	int			fromY;
	int			toY;
	bool 		stopRun;
	bool		print_flag;
	f_int2dim	target;
};

int                             eng_int_2dim(struct eng_int_interval rt, long val);

int                             eng_check_int2dim_interval(struct eng_int_interval rt, long val_from, long val_to);

int								fautoprint(FILE *f, const struct eng_int_interval v);

static inline int				autoprint(const struct eng_int_interval v){
	return fautoprint(stdout, v);
}

#endif /* _ENG_RUNNGER_H */


