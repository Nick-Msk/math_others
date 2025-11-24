// page 91

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#include "log.h"
#include "check.h"
#include "common.h"

bool			gen_strings(char *buf, int cnt, int cnt_oper, int dec_value);

int				calc_string(const char *s, int elem);

int				main(int argc, const char *argv[]){
	loginit("91task.log", false, 0, "Start");

	int cnt_oper = 3;		// as per task
	int des_value = 100;	// as per task
	if (argc > 1)
		cnt_oper = atoi(argv[1]);

	if (argc > 2)
		des_value = atoi(argv[2]);
	
	if (cnt_oper > 0 && cnt_oper <= 10)
		printf("Start with %d operations and check %d\n", cnt_oper, des_value);
	else { 
		printf("Incorrent, shound be between 1 and 10\n");
		return 1;
	}

	const int BUF_CNT = 10;
	char buf[BUF_CNT * 2 + 1];
	int res; 

	buf[sizeof(buf) - 1] = '\0';
	gen_strings(buf, BUF_CNT, cnt_oper, des_value);

/*	if ((res = calc_string(buf, BUF_CNT)) == dec_value)
		printf("%s = %d\n", buf, res);
	else 
		printf("%s = %d n*/

	logclose("...");
	return 0;
}

int				calc_string(const char *s, int elem){
	int sum = 0, current = 0;
	for (int i = 0; i < elem; i += 2){
		switch (s[i]){
			case '+':
				sum += current;
				current = 0;
			break;
			case '-':
				sum -= current;
				current = 0;
			break;
			case ' ':
				current = current * 10 + s[i + 1] - '0';	// user type.h? 
			break;
		}
	}
	return sum;
}

// TODO: gen_strings via recursion 

bool			gen_strings(char *buf, int cnt, int cnt_oper, int dec_value){

	return false;
}

