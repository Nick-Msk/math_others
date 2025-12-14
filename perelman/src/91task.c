// page 91

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#include "log.h"
#include "check.h"
#include "common.h"

#define 			OP_SYM_SZ	9

struct op_sym{
	int		sz;
	int		pos;
	char 	sym[OP_SYM_SZ + 1]; // for final '\0'
	char	op[OP_SYM_SZ + 1];	// for final '\0'
};

// util
int				op_sym_flog(FILE *f, const struct op_sym *s);
int				op_sym_strinit(struct op_sym *s, const char *str);

//bool			gen_strings(char *orig_buf, int orig_cnt, char *buf, int cnt, int cnt_oper, int dec_value);
int				gen_strings(struct op_sym *buf, int cnt_oper, int value);

int				calc_string(const struct op_sym *s);

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
	int found_cnt = 0;
	struct op_sym buf;	

	op_sym_strinit(&buf,  "123456789");
	op_sym_flog(stdout, &buf);
	
	//int sum = calc_string(&buf);
   	//printf("TEST sum = %d\n", sum);	
	

	if ((found_cnt = gen_strings(&buf, cnt_oper, des_value)) > 0)
		printf("%d was found\n", found_cnt);
	else
		printf("Not found...\n");
	

	logclose("...");
	return 0;
}

int				calc_string(const struct op_sym *s){
	logenter("Elem %d", s->sz);
	op_sym_flog(logfile, s);
	
	int sum = 0, current = 0;
	char op = '+'; // + or -
	
	for (int i = 0; i < s->sz; i++){
		char c = s->op[i];
		logsimple("c=[%c], i=%d op=%c sum=%d curr=%d sym=%c", c, i, op, sum, current, s->sym[i]);
		switch(c){
			case '+':
			case '-':
				// calculate previous
				if (op == '+')
					sum += current;
			   	else if (op == '-')
					sum -= current;
				current = 0; // reset 	
				op = c;	// for the next or last
			//break;
			default:
				current = current * 10 + (s->sym[i] - '0');
			break;
		}
	}
	// last
	if (op == '+')
		sum += current;
	else if (op == '-')
		sum -= current;

	return logret(sum, "%d", sum);
}

int				op_sym_flog(FILE *f, const struct op_sym* s){
	int cnt = 0;
	for (int i = 0; i < OP_SYM_SZ; i++){
		if (s->op[i] != ' ')
			fputc(s->op[i], f);
		fputc(s->sym[i], f);
		cnt += 2;
	}
	fputc('\n', f);
	return ++cnt;
}

int				op_sym_strinit(struct op_sym *s, const char *str){
	int i = 0, pos = 0;
	while (pos < OP_SYM_SZ && str[i] != '\0'){
		char c = str[i];
		if (c == '+' || c == '-'){
			logsimple("in+-: pos=%d i=%d", pos, i); 
			s->op[pos] = c;
			i++;
		} else {
			s->op[pos] = ' ';
			s->sym[pos] = c;
			i++;
			pos++;
		}
	}
	logsimple("pos=%d i=%d", pos, i);
	s->sym[pos] = s->op[pos] = '\0';
	s->pos = 0;	// for gen_strings
	return s->sz = pos;
}


int				gen_strings(struct op_sym *buf, int cnt_oper, int value){
	logenter("cnt_oper %d, value %d, pos=%d sz=%d", cnt_oper, value, buf->pos, buf->sz);
	int res = 0;

	// check current
	if (calc_string(buf) == value){
		//printf("FOUND: with cnt=%d pos=%d\n", cnt_oper, buf->pos);
		printf("FOUND: ");
		op_sym_flog(stdout, buf);
		res++;
	}
	if (cnt_oper > 0 && buf->pos < buf->sz){	// if we have free operation and free symbols are remains
		// check no op
		struct op_sym tmp = *buf;
		tmp.op[tmp.pos] = ' ';
		tmp.pos++;
		res += gen_strings(&tmp, cnt_oper, value);
	
		// check +
		tmp.op[tmp.pos -1] = '+';
		res += gen_strings(&tmp, cnt_oper - 1, value);
		
		// check -
		tmp.op[tmp.pos - 1] = '-';	
		res += gen_strings(&tmp, cnt_oper - 1, value);
	} 

	return logret(res, "res=%d", res);
}

// TODO: gen_strings via recursion 

/*
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
*/
