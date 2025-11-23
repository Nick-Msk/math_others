#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>

static int 		g_print_flag = 1;

int				check_natural(int, int);

int				main(int argc, const char *argv[]){
	int check_max = 1000000;
	int check_min = 1;

	if (argc > 1)
		check_max = atoi(argv[1]);

	if (argc > 2){
		check_min = check_max;
		check_max = atoi(argv[2]);
	}

	printf("Start checking from %d till %d\n", check_min, check_max);

	int cnt = 0;
    if (check_max > 1)
		cnt = check_natural(check_min, check_max);

	if (cnt > 0)
		printf("Found %d elements\n", cnt);
	else
		printf("Not found\n");

	return 0;
}

static inline int 	f(long n, long *val){
	if (val)
		*val = (13 * n - 1) / (3 * n + 5);
	return (int)
			((13 * n - 1) % (3 * n + 5));
}

int					check_natural(int from, int to){
	int total = 0;
	long res;

	for (int i = from; i <= to; i++){
		int mod;
		if ((mod = f(i, &res)) == 0){	// found!!
			total++;
			if (g_print_flag)
				printf("f(%d) = %ld\n", i, res);
		}
		//if (i == -1)
		//	printf("f(%d) = %ld mod=%d\n", i, res, mod);

	}

	return total;
}


