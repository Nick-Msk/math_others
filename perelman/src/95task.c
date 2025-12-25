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

static int				total_cnt_checked = 0;

// util
static int				op_sym_flog(FILE *f, const struct op_sym *s);
static int				op_sym_strinit(struct op_sym *s, const char *str, int oper_total);

static int				gen_sequnces(int value, const char *str);
static int				calc_string(const struct op_sym *s);

int				main(int argc, const char *argv[]){
	loginit("log/95task.log", false, 0, "Start");

	int value;	
	int found_cnt;
	const char *pattern = "123456789";

	if (!check_arg(2, "Usage: %s value (int)\n", *argv))
        return 1;

	value = atoi(argv[1]);

	if (argc > 2)
		pattern = argv[2];

	printf("Check value = %d with 1 op '/' for patter [%s]\n'", value, pattern);

	if ((found_cnt = gen_sequnces(value, pattern)) > 0)
		printf("%d was found\n", found_cnt);
	else
		printf("Not found...\n");

	printf("Total %d was checked\n", total_cnt_checked);
	
	logclose("...");
	return 0;
}

// note - INT_MAX is marker for undefined value
static int				calc_string(const struct op_sym *s){
//	logenter("Elem %d", s->sz);
	//op_sym_flog(logfile, s);
	
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
									return INT_MAX;	//logret(INT_MAX, "%d", INT_MAX);
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
				return INT_MAX;	//logret(INT_MAX, "%d", INT_MAX);
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

	return sum; // logret(sum, "%d", sum);
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
	op_sym_flog(logfile, buf);
	total_cnt_checked++;
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

// find from 0 to pos 
static bool		op_sym_not_in(const struct op_sym *buf, char c){
	for (int i = 0; i < buf->pos; i++)
		if (buf->sym[i] == c)
			return false;
	return true;
}		

static int		generate(struct op_sym *buf, int value){
	//logenter("pos =%d sz=%d", buf->pos, buf->sz);
	//op_sym_flog(logfile, buf);
	int res = 0;

	if (buf->pos < buf->sz){
		for (int j = 0; j < buf->sz; j++){
			char c = j + 1 + '0';
			if (op_sym_not_in(buf, c)){
				buf->sym[buf->pos] = c;
			//	logmsg("check %c in pos %d", c, buf->pos);
				buf->pos++;
				res += generate(buf, value);
				buf->pos--;
			}
		}
	} else 
		res += check_div(buf, value);

	return res;	// logret(res, "%d", res);
}

// ONLY FOR 1 OPERATION Div (/), value is 2..9 as per task
// wrapper for generate()
static int				gen_sequnces(int value, const char *str){
	logenter("Value %d", value);
	
	struct op_sym buf;
	op_sym_strinit(&buf, str, 1);
	
	int res = generate(&buf, value);

	return logret(res, "res=%d", res);
}

