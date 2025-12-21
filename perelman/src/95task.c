// page 91

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "log.h"
#include "check.h"
#include "common.h"

#define 			OP_SYM_SZ	99

struct op_sym{
	int		sz;
	int		pos;
	int 	oper_total;
	char 	sym[OP_SYM_SZ + 1]; // for final '\0'
	char	op[OP_SYM_SZ + 1];	// for final '\0'
};

// util
int				op_sym_flog(FILE *f, const struct op_sym *s);
int				op_sym_strinit(struct op_sym *s, const char *str, int oper_total);

int				gen_strings(struct op_sym *buf, int cnt_oper, int value, char sym);
int				calc_string(const struct op_sym *s);

int				main(int argc, const char *argv[]){
	loginit("log/95task.log", false, 0, "Start");

	int cnt_oper = 5;		// as per task
	int value = 100;	// as per task
	const char *sample = "33333";							
	struct op_sym buf;	
	int found_cnt;
	char sym = ' ';

	if (argc > 1)
		sample = argv[1];

	if (argc > 2)
		cnt_oper = atoi(argv[2]);
	
	if (argc > 3)
		value = atoi(argv[3]);

	if (argc > 4)
		sym = *argv[4];
	
	if (cnt_oper == 0){
		op_sym_strinit(&buf, sample, cnt_oper);
		op_sym_flog(stdout, &buf);
		value = calc_string(&buf);
		printf("Res = %d\n", value);	
	}
	else {
		printf("Start with %d operations and check values %d for [%s] using op [%c]\n", cnt_oper, value, sample, sym);

		// main part
		op_sym_strinit(&buf,  sample, cnt_oper);
		
		if ((found_cnt = gen_strings(&buf, cnt_oper, value, sym)) > 0)
			printf("%d was found\n", found_cnt);
		else
		printf("Not found...\n");
	}

	logclose("...");
	return 0;
}

// note - INT_MAX is marker for undefined value
int				calc_string(const struct op_sym *s){
	logenter("Elem %d", s->sz);
	op_sym_flog(logfile, s);
	
	int sum = 0, summul = 1, current = 0;
	char op = '+'; // for + -
	char opmul = ' ';	// for * /
	
	for (int i = 0; i < s->sz; i++){
		char c = s->op[i];
		char sym = s->sym[i];
		logsimple("c=[%c], i=%d op=%c sum=%d curr=%d sym=%c opmul=%c summul=%d", 
					c, i, op, sum, current, s->sym[i], opmul, summul);
		switch(c){
			case '+':
			case '-':
				if (opmul == '*' || opmul == '/'){
					if (opmul == '*')
						summul *= current;
					else if (opmul == '/')
						summul /= current;
					current = summul;
					summul = 1;
					opmul = ' ';	// reset
				}
				//logsimple("after +-: current=%d", current);
				// calculate previous
				if (op == '+')
					sum += current;
			   	else if (op == '-')
					sum -= current;
				op = c;	// for the next or last
				current = sym - '0';	// reset
			break;
			case '*':
			case '/':
				if (opmul == '*' || opmul == '/'){
					if (opmul == '*')
						summul *= current;
					else  if (opmul == '/'){
								if (current == 0 || summul % current > 0)	// not dididev
									return INT_MAX;
								summul /= current;	// dev by zero could be
					}
				} else 
					summul = current;
				//logsimple("after */: summul=%d, curr=%d", summul, current);
				opmul = c;	// save mul/div operation
				current = sym - '0';	// reset
			break;
			default:
				current = current * 10 + (sym - '0'); 
				logsimple("no op: curr=%d", current);
			break;
		}
	}
	// last
	//logsimple("after : opmul=%c, summul=%d", opmul, summul);
	if (opmul == '*' || opmul == '/'){
		if (opmul == '*')
			summul *= current;
		else if (opmul == '/'){
			if (current == 0 || summul % current > 0)	// not dididev
				return INT_MAX;
			summul /= current;
		}
		current = summul;
	}
	//logsimple("after all: op=%c, sum=%d", op, sum);
	if (op == '+')
		sum += current;
	else if (op == '-')
		sum -= current;

	return logret(sum, "%d", sum);
}

int				op_sym_flog(FILE *f, const struct op_sym* s){
	int cnt = 0;
	for (int i = 0; i < OP_SYM_SZ && i < s->sz; i++){
		if (s->op[i] != ' ')
			fputc(s->op[i], f);
		fputc(s->sym[i], f);
		cnt += 2;
	}
	fputc('\n', f);
	return ++cnt;
}

int				op_sym_strinit(struct op_sym *s, const char *str, int oper_total){
	int i = 0, pos = 0;
	char op = ' ';
	while (pos < OP_SYM_SZ && str[i] != '\0'){
		char c = str[i];
		if (c == '+' || c == '-' || c == '*' || c == '/'){
			logsimple("in+-: pos=%d i=%d c=%c", pos, i, c); 
			op = c;
			i++;
		} else {
			s->op[pos] = op;
			op = ' ';
			s->sym[pos] = c;
			i++;
			pos++;
		}
	}
	logsimple("pos=%d i=%d", pos, i);
	s->sym[pos] = s->op[pos] = '\0';
	logsimple("s [%s], oper [%s]", s->sym, s->op);
	s->pos = 0;	// for gen_strings
	s->oper_total = oper_total;
	return s->sz = pos;
}


int				gen_strings(struct op_sym *buf, int cnt_oper, int value, char sym){
	logenter("cnt_oper %d, value %d, pos=%d sz=%d operations [%c]", cnt_oper, value, buf->pos, buf->sz, sym);
	int res = 0;

	// check current
	if (calc_string(buf) == value){
		//printf("FOUND: with cnt=%d pos=%d\n", cnt_oper, buf->pos);
		printf("FOUND(%d): ", buf->oper_total - cnt_oper);
		op_sym_flog(stdout, buf);
		res++;
	}
	if (cnt_oper > 0 && buf->pos < buf->sz){	// if we have free operation and free symbols are remains
		// check no op
		struct op_sym tmp = *buf;
		tmp.op[tmp.pos] = '+';
		tmp.pos++;

		if (buf->pos > 0)	// skip first +
			if (sym == ' ' || sym == '+')
				res += gen_strings(&tmp, cnt_oper - 1, value, sym);
		
		// check no op
		tmp.op[tmp.pos - 1] = ' ';
		res += gen_strings(&tmp, cnt_oper, value, sym);
		
		// check -
		if (sym == ' ' || sym == '-'){
			tmp.op[tmp.pos - 1] = '-';	
			res += gen_strings(&tmp, cnt_oper - 1, value, sym);
		}

		if (buf->pos > 0){
			// check *
			if (sym == ' ' || sym == '*'){
				tmp.op[tmp.pos - 1] = '*';	
				res += gen_strings(&tmp, cnt_oper - 1, value, sym);
			}
			// check /
			if (sym == ' ' || sym == '/'){
				tmp.op[tmp.pos - 1] = '/';	
				res += gen_strings(&tmp, cnt_oper - 1, value, sym);
			}
		}
	} 

	return logret(res, "res=%d", res);
}
