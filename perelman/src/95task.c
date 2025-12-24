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
static int				op_sym_flog(FILE *f, const struct op_sym *s);
static int				op_sym_strinit(struct op_sym *s, const char *str, int oper_total);

static int				gen_sequnces(int value);
static int				calc_string(const struct op_sym *s);

int				main(int argc, const char *argv[]){
	loginit("log/95task.log", false, 0, "Start");

	int value;	
	int found_cnt;

	if (!check_arg(2, "Usage: %s value (int)\n", *argv))
        return 1;

	value = atoi(argv[1]);

	printf("Check value = %d with 1 op '/'\n'", value);

	if ((found_cnt = gen_sequnces(value)) > 0)
		printf("%d was found\n", found_cnt);
	else
		printf("Not found...\n");
	
	logclose("...");
	return 0;
}

// note - INT_MAX is marker for undefined value
static int				calc_string(const struct op_sym *s){
	logenter("Elem %d", s->sz);
	op_sym_flog(logfile, s);
	
	int sum = 0, summul = 1, current = 0;
	char op = '+'; // for + -
	char opmul = ' ';	// for * /
	
	for (int i = 0; i < s->sz; i++){
		char c = s->op[i];
		char sym = s->sym[i];
	//	logsimple("c=[%c], i=%d op=%c sum=%d curr=%d sym=%c opmul=%c summul=%d", 
	//				c, i, op, sum, current, s->sym[i], opmul, summul);
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
									return logret(INT_MAX, "%d", INT_MAX);
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
	//			logsimple("no op: curr=%d", current);
			break;
		}
	}
	// last
	//logsimple("after : opmul=%c, summul=%d", opmul, summul);
	if (opmul == '*' || opmul == '/'){
		if (opmul == '*')
			summul *= current;
		else if (opmul == '/'){
			if (current == 0 || summul % current > 0){	// not dididev
				return logret(INT_MAX, "%d", INT_MAX);
			}
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

static int				op_sym_flog(FILE *f, const struct op_sym* s){
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

static int				op_sym_strinit(struct op_sym *s, const char *str, int oper_total){
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

static int		check_div(struct op_sym *buf, int value){
	int cnt = 0, i;
	for (i = 1; i < buf->sz; i++){
		buf->op[i] = '/';
		buf->op[i - 1] = ' '; 	// reset prev
		if (calc_string(buf) == value){
			cnt++;
			printf("FOUND: %d=", value);
			op_sym_flog(stdout, buf);
		}
	}
	buf->op[i - 1] = ' '; // reset last
	return cnt;
}

static bool		change_elem(struct op_sym *buf, int elem1, int elem2){
	if (elem1 >= 0 && elem1 < buf->sz && elem2 >= 0 && elem2 < buf->sz){
		char tmp = buf->sym[elem1];
		buf->sym[elem1] = buf->sym[elem2];
		buf->sym[elem2] = tmp;
		return true;
	}
	else {
		fprintf(stderr, "Elements is out of range %d, %d - sz=%d\n", elem1, elem2, buf->sz);
		return false;
	}
}

static int		generate(struct op_sym *buf, int value){
	op_sym_flog(logfile, buf);
	int res = 0;

	// TODO:
	for (int j = 0; j < buf->sz; j++){
		for (int i = 1; i < buf->sz; i++){
			change_elem(buf, i - 1, i);
			op_sym_flog(logfile, buf);
		//	res += check_div(buf, value);
		}
	}
	return res;
}

// ONLY FOR 1 OPERATION Div (/), value is 2..9 as per task
// wrapper for generate()
static int				gen_sequnces(int value){
	logenter("Value %d", value);
	
	struct op_sym buf;
	op_sym_strinit(&buf, "123456789", 1);
	
	int res = generate(&buf, value);

	return logret(res, "res=%d", res);
}

/*
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
				logsimple("HERE pos=%d!!!", buf->pos);	
				op_sym_flog(logfile, &tmp);
				res += gen_strings(&tmp, cnt_oper - 1, value, sym);
			}
		}

		// check the numbers
		for (int i = 1; i <= 9; i++){
			tmp.op[tmp.pos - 1] = i + '0';
			logsimple("in cyrcle! i = %d, pos = %d", i, tmp.pos);
			res += gen_strings(&tmp, cnt_oper, value, sym);
		}
	} 

	return logret(res, "res=%d", res);
} */


