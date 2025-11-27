// page 91

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#include "log.h"
#include "check.h"
#include "common.h"

struct op_sym{
	int		sz;
	char 	sym[sz + 1];
	char	op[sz];
};

bool			gen_strings(char *orig_buf, int orig_cnt, char *buf, int cnt, int cnt_oper, int dec_value);

int				calc_string(const op_sym *s, int elem);

int				main(int argc, const char *argv[]){
	loginit("log/91task.log", false, 0, "Start");

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

	// main part
	const int BUF_CNT = 9;
	char buf[BUF_CNT * 2 + 1];
	int found_cnt = 0;

	// setup buffer
	buf[BUF_CNT * 2 + 1] = '\0';
	for (int i = 0; i < BUF_CNT; i++){
		buf[2 *i] = ' '; 
		buf[2 * i + 1] = i + 1 + '0';
		//printf("i=%d, [%c][%c]\n", i, buf[i], buf[i + 1]);
	}
	printf("[%s]\n", buf);

	if ((found_cnt = gen_strings(buf, BUF_CNT, buf, BUF_CNT, cnt_oper, des_value)) > 0)
		printf("%d was found\n", found_cnt);
	else
		printf("Not found...\n");

	logclose("...");
	return 0;
}

int				calc_string(const op_sym *s, int elem){
	logenter("%s:[%d]", s, elem);
	
	int sum = 0, current = 0;
	char op = ' '; // + or -
	/*for (int i = 0; i < elem; i++){
		switch (s[2 * i]){
			case '+': case '-':
				// calculate last operation (if any)
				if (op == '+')
					sum += current;
				else if (op == '-')
					sum -= current;
				op = s[i];
				current = 0;
			break;
			case ' ':
				current = current * 10 + s[2 * i + 1] - '0';	// use type.h? 
			break;
		}
	}
	*/
	for (int i = 0; i < elem; i++){
		switch(s[2 * i]
	}
	
	if (op == '+' || op == ' ')
		sum += current;
	else if (op == '-')
		sum -= current;

	return logret(sum, "%d", sum);
}

// TODO: gen_strings via recursion 

bool			gen_strings(char *orig_buf, int orig_cnt, char *buf, int cnt, int cnt_oper, int des_value){
	logenter("buf [%s], cnt [%d], cnt_oper [%d]", buf, cnt, cnt_oper);	
	int res;

	if (cnt > 0){
	//for (i = 0; i < cnt; i+= 2){
			
		// exec calc_string
		if ((res = calc_string(orig_buf, orig_cnt)) == des_value)
			printf("FOUND: %s = %d\n", orig_buf, res);
		buf[0] = '+';
		if ((res = calc_string(orig_buf, orig_cnt)) == des_value)
			printf("FOUND: %s = %d\n", orig_buf, res);
	    buf[0] = '-';
		if ((res = calc_string(orig_buf, orig_cnt)) == des_value)
			printf("FOUND: %s = %d\n", orig_buf, res);
		gen_strings(orig_buf, orig_cnt, buf + 2, cnt - 1, cnt_oper, des_value);
	}
	return logret(false, "...");
}

