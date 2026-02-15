#ifndef _ENG_RUNNGER_H
#define _ENG_RUNNGER_H			

#include <stdbool.h>
#include <stdio.h>
#include <float.h>

#define							ENG_DIM_CAP 16

// as external
extern union eng_un_int_func 	eng_un_int_func_api;

static const int 				ENG_FLT_COMRARE_DEF_VALUE 	= FLT_EPSILON * 10;	// to use in target fuction to compart, not sure about value
static const int 				ENG_PRINT_FLAG 				= 0x1;
static const int 				ENG_STOP_RUN_FLAG			= 0x2;
static const int 				ENG_TARGET_VALUE_FLAG		= 0x4;
static const int				ENG_ERROR_FLAG				= 0x8;

// common
typedef bool	(*tf_general)(bool);	// just for common run purpose

// integer prototypes
typedef bool	(*tf_int1dim)(int, long);
typedef bool	(*tf_int1dim_bool)(int);		// no long here!

typedef bool    (*tf_int2dim)(int, int, long);
typedef bool	(*tf_int2dim_bool)(int, int);	//  no long here!

typedef bool    (*tf_int3dim)(int, int, int, long);
typedef bool	(*tf_int3dim_bool)(int, int, int);	//  no long here!
													
typedef bool    (*tf_int4dim)(int, int, int, int, long);
typedef bool	(*tf_int4dim_bool)(int, int, int, int);	//  no long here!

typedef bool    (*tf_int5dim)(int, int, int, int, int, long);
typedef bool	(*tf_int5dim_bool)(int, int, int, int, int);	//  no long here!
														//
														//
typedef bool    (*tf_int6dim)(int, int, int, int, int, int, long);
typedef bool	(*tf_int6dim_bool)(int, int, int, int, int, int);	//  no long here!


typedef bool    (*tf_int7dim)(int, int, int, int, int, int, int, long);
typedef bool	(*tf_int7dim_bool)(int, int, int, int, int, int, int);	//  no long here!


typedef bool    (*tf_int8dim)(int, int, int, int, int, int, int, int, long);
typedef bool	(*tf_int8dim_bool)(int, int, int, int, int, int, int, int);	//  no long here!


typedef bool    (*tf_int9dim)(int, int, int, int, int, int, int, int, int, long);
typedef bool	(*tf_int9dim_bool)(int, int, int, int, int, int, int, int, int);	//  no long here!


typedef bool    (*tf_int10dim)(int, int, int, int, int, int, int, int, int, int, long);
typedef bool	(*tf_int10dim_bool)(int, int, int, int, int, int, int, int, int, int);	//  no long here!
															
// float
typedef bool	(*tf_flt2dim)(double, double, double);
typedef bool	(*tf_flt2dim_bool)(double, double);

typedef bool	(*tf_flt1dim)(double, double);
typedef bool	(*tf_flt1dim_bool)(double);


union eng_un_int_func {
		tf_general			f_gen;
		tf_int1dim			f_int_1dim;
		tf_int1dim_bool		f_int_1dim_bool;
		tf_int2dim			f_int_2dim;
		tf_int2dim_bool		f_int_2dim_bool;
		tf_int3dim			f_int_3dim;
		tf_int3dim_bool		f_int_3dim_bool;
		tf_int4dim			f_int_4dim;
		tf_int4dim_bool		f_int_4dim_bool;
		tf_int5dim			f_int_5dim;
		tf_int5dim_bool		f_int_5dim_bool;
		tf_int6dim			f_int_6dim;
		tf_int6dim_bool		f_int_6dim_bool;
		tf_int7dim			f_int_7dim;
		tf_int7dim_bool		f_int_7dim_bool;
		tf_int8dim			f_int_8dim;
		tf_int8dim_bool		f_int_8dim_bool;
		tf_int9dim			f_int_9dim;
		tf_int9dim_bool		f_int_9dim_bool;
		tf_int10dim			f_int_10dim;
		tf_int10dim_bool	f_int_10dim_bool;
};

typedef struct eng_int_dim {
	int		from;
	int		to;
	int		step;
	int		iter;
} eng_int_dim;

// integer structure
typedef struct eng_int_interval {
	int					useDim;	// now is limited to 10
	eng_int_dim			dim[ENG_DIM_CAP - 1];	// 0..9
	//------------------------------------------------------
	int			flags;	// 0x1 , 0x2, 0x3, 0x4
	long		targetValue;	// if NOT bool function
	int			modLog;		// for printing logs cnt % modLog == 0
	char 		print_msg[128];	// for primiting functional inertation TODO:remove, obsolete
	union		eng_un_int_func f;
} eng_int_interval;


typedef struct eng_flt_interval {
	double		fromX;	// for now only 2 dims are supported
	double		toX;
	double		stepX;	// add initStepX ? 
	double		fromY;
	double		toY;
	double		stepY;	// probably initStepY
	int			flags;
	double		targetValue;	// if NOT bool function
	int			modLog;		// for printing logs cnt % modLog == 0
	union {
		tf_general		f_gen;
		tf_flt2dim		f_flt_2dim;
		tf_flt2dim_bool	f_flt_2dim_bool;
		tf_flt1dim		f_flt_1dim;
		tf_flt1dim_bool	f_flt_1dim_bool;
	};
} eng_flt_interval;

static bool						eng_fl_targetValue(int flags){
	return flags & ENG_TARGET_VALUE_FLAG;
}

static bool						eng_fl_stopRun(int flags){
	return flags & ENG_STOP_RUN_FLAG;
}

static bool						eng_fl_print(int flags){
	return flags & ENG_PRINT_FLAG;
}

static bool						eng_fl_error(int flags){
	return flags & ENG_ERROR_FLAG;
}

// constructor (with filling f_gen()
#define			eng_dim(...)(eng_int_dim){.from = 0, .to = 0, .step = 1, .iter = 0, __VA_ARGS__}
#define			eng_init_dims(...)(eng_int_dim[]){__VA_ARGS__}

#define 		eng_create_int(...) (struct eng_int_interval)\
{.useDim = 4,\
\
 .flags = ENG_PRINT_FLAG, .modLog = 0,\
 .f = eng_un_int_func_api, .print_msg="", __VA_ARGS__};	

// construct from file
struct eng_int_interval 		eng_loadfromfile(const char *cfgname, bool strict);

// runners (just true/false or == targetValue)
// inteter
// Common Integer runner!
int								eng_int_run(struct eng_int_interval rt);

// check to avoid the same result
bool							eng_check_previous(double val, const char *msg, int sz);
void							eng_check_reset(void);
// float
int								eng_flt_1dim(struct eng_flt_interval rt);

int								eng_flt_2dim(struct eng_flt_interval rt);

// checkers (for the range of values)
// TODO: replace to user eng_int_run(...);
int                             eng_check_int1dim_interval(struct eng_int_interval rt, long val_from, long val_to);

int                             eng_check_int2dim_interval(struct eng_int_interval rt, long val_from, long val_to);

int                             eng_check_int3dim_interval(struct eng_int_interval rt, long val_from, long val_to);

int                             eng_check_int4dim_interval(struct eng_int_interval rt, long val_from, long val_to);

// float
// TODO: eng_float_run(struct eng_flt_interval rt);
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


