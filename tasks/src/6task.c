#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int			main(int argc, const char *argv[]){
	
	double v = 0.0, p = 0.0;

	if (argc > 1)
		v = atof(argv[1]);
	
	if (argc > 2)
		p = atof(argv[2]);

	printf("Run %g ^ %g\n", v, p);

	printf("Res = %g\n", pow(v, p));

	return 0;
}
