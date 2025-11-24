#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "check.h"
#include "common.h"
#include "compl_alg.h"

// Complexity checker API

// TODO: better to use static const int here!
#define				COMPL_MAX_METRIC		10


void				compl_printall(Compl *st, const char *msg){
	if (*msg)		
		fprintf(st->out, "%s:\nTotla [%s]: %d\n", msg, st->desc, st->total);
	else
		fprintf(st->out, "Total [%s]: %d\n", st->desc, st->total);

	for (int i = 0; i < st->mcount; i++)
		fprintf(st->out, "%s: %d\n", st->metrics_desc[i], st->metrics[i]);
	printf("-------------\n");
}

void				compl_free(Compl *st){
	for (int i = 0; i < st->mcount; i++){ // TODO: what if to use iterator?
		free(st->metrics_desc[i]);
		st->metrics_desc[i] = 0;
	}
	free(st->desc);
	st->desc = 0;
}

Compl				compl_newst(const char *desc, FILE *out, FILE *err){
	Compl st;
	st.total = 0;
	st.out = out ? out : stdout;
	st.err = err ? err : stderr;
	if ((st.desc = strdup(desc)) == 0){
		fprintf(st.err, "Unable to dup [%s] string\n", desc);
		//return false; TODO: how to handle that??? throw ?
	}
	return st;
}

int					compl_setmetric(Compl *st, const char *mdesc, int initval){
	// search if metric exists, if found - override, else - create new one
	int		pos = 0;
	while (pos < st->mcount){
		if (strcmp(st->metrics_desc[pos], mdesc) == 0){
			fprintf(st->out, "Metric [%s] is already created\n", mdesc);
			return pos;
		}
		pos++;
	}
	if (pos == st->mcount){	// not found and out of space!!
		fprintf(st->err, "Out of metric! Unable to add new one [%s]\n", mdesc);
		return -1;
	} else {	// use pos as empty
		st->metrics[pos] = initval;
		if ((st->metrics_desc[pos] = strdup(mdesc)) == 0){
			fprintf(st->err, "Unable to clone metric [%s]\n", mdesc);
			return -1;
		}
		st->mcount++;
	}
	return pos;
}

// ------------------------------------------------------------------------------------------------------
// End of Complexity checker

#ifdef COMPLALGTESTING
/*
        TESTING
        1. int
        2. interruption macro test
*/

int                                     run_test_init(int num){
	printf("Test %d (overflow test %s)\n", num, __func__);

	Compl co = compl_new("Test metric 1");

	compl_printall(&co, "message");

	compl_free(&co);

	printf("Test %d is passed (%s)\n", num,  __func__);
	return 0;
}


int                                     run_tests(void){
        int             num = 0;

        if (run_test_init(++num))
                return num;

        printf("%s: Total %d tests are passed\n", __func__, num);
        return 0;
}


int					main(int argc, const char *argv[]){
 	// that is without test engine
	int             err_test = run_tests();

 	if (err_test == 0)
        printf("Passed!\n");
    else
		printf("Error in %d test, check logs for the details\n", err_test);

	return 0;
}

#endif /* COMLALGTESTING */

