#ifndef _ENG_RUNNGER_H
#define _ENG_RUNNGER_H			

#include <stdbool.h>
#include <stdio.h>

// integer
typedef bool    (*tf_int2dim)(int, int, long);
typedef bool	(*tf_int2dim_bool)(int, int);	//  no long here!

typedef bool	(*tf_int1dim)(int, long);
typedef bool	(*tf_int1dim_bool)(int);		// no long here!

// float
typedef bool	(*tf_flt2dim)(double, double, double);
typedef bool	(*tf_flt2dim_bool)(double, double);

typedef bool	(*tf_flt1dim)(double, double);
typedef bool	(*tf_flt1dim_bool)(double);

// integer structure
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
		tf_int1dim_bool	f_int_1dim_bool;
	};
};


struct eng_flt_interval {
	double		fromX;	// for now only 2 dims are supported
	double		toX;
	double		fromY;
	double		toY;
	bool		targetValueFlag;	// TODO: put all the flags into 1 var via | op
	double		targetValue;	// if NOT bool function
	bool 		stopRun;
	bool		printFlag;
	int			modLog;		// for printing logs cnt % modLog == 0
	union {
		tf_flt2dim		f_flt_2dim;
		tf_flt2dim_bool	f_flt_2dim_bool;
		tf_flt1dim		f_flt_1dim;
		tf_flt1dim_bool	f_flt_1dim_bool;
	};
};

// runners
// inteter
int								eng_int_1dim(struct eng_int_interval rt);

int                             eng_int_2dim(struct eng_int_interval rt);

// TODO:
int                             eng_check_int1dim_interval(struct eng_int_interval rt, long val_from, long val_to);

int                             eng_check_int2dim_interval(struct eng_int_interval rt, long val_from, long val_to);

// float
int                             eng_check_flt2dim_interval(struct eng_flt_interval rt, double val_from, double val_to);	// TODO:

// printers
int								eng_fautoprint_flt(FILE *f, struct eng_flt_interval v);

static inline int				eng_autoprint_flt(struct eng_flt_interval v){
	return eng_fautoprint_flt(stdout, v);
}

int								eng_fautoprint(FILE *f, struct eng_int_interval v);

static inline int				eng_autoprint(struct eng_int_interval v){
	return eng_fautoprint(stdout, v);
}

#endif /* _ENG_RUNNGER_H */


