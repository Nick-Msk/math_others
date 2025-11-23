#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static int 		g_print_all = 1;

typedef bool (*f_int2dim)(long, long);

// sample function ( 3x2 - 16xy - 35y2 + 17 == 0)
static bool		f1(long x, long y);

int				eng_int_2dim(int fromx, int fromy, int tox, int toy, bool stoprun, f_int2dim target);


int				main(int argc, const char *argv[]){
	int fromx, fromy, tox, toy;

	if (argc < 3){
		fprintf(stderr, "Usage: %s from to\n", *argv);
		return 1;	
	}

	fromx = fromy = atoi(argv[1]);
	tox = toy = atoi(argv[2]);

	printf("Run from %d to %d (x and x)\n", fromx, tox);
	
	int cnt = eng_int_2dim(fromx, fromy, tox, toy, false, f1);

	if (cnt > 0)
		printf("%d was/were found\n", cnt);
	else
		printf("Not found for that interval...\n");

	return 0;
}

int				eng_int_2dim(int fromx, int fromy, int tox, int toy, bool stoprun, f_int2dim target){
	int		total = 0;
	// iterator
	for (int x = fromx; x < tox; x++)
		for (int y = fromy; y < toy; y++){
		if (target(x, y)){
			if (g_print_all)
				printf("target function(%d, %d) is true)\n", x, y);
			if (stoprun)
				return 1;
			else
				total++;
		}
	}	
	return total;
}

// sample function ( 3x2 - 16xy - 35y2 + 17 == 0)
static bool		f1(long x, long y){
	return 3 *  x * x - 16 * x * y - 35 * y * y + 17 == 0;
}



