#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ , NUM, LPER, RPER, DEREF, NUM16, REG, NEG, VAR
	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"[a-zA-Z_]+[0-9a-zA-Z_]+", VAR},		// var
	{"\\+", '+'},					// plus 43
	{"\\-", '-'}, 					// 45
	{"\\*", '*'},					//42
	{"\\/", '/'},					//47
	{"\\(", LPER},
	{"\\)", RPER},
	{"0x[0-9|a-f]{1,32}", NUM16},
	{"\\$[a-z]+", REG},
	{"[0-9]{1,32}", NUM},
	{"==", EQ}					// equal
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i, j;
	regmatch_t pmatch;
	nr_token = 0;
//	printf("%d\n", NR_REGEX);
	while(e[position] != '\0') {
//		printf("%d %c\n", position, e[position]);  
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;
//				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;
//				printf("%d\n", rules[i].token_type);
				switch(rules[i].token_type) {
					case 43:
					case 45:
					case 42:
					case 47:
					case LPER:
					case RPER:
					case NUM: 
					case REG:
					case EQ:
					case NUM16: 
					case VAR:
					tokens[nr_token].type=rules[i].token_type;
					for (j=0; j<substr_len; j++) tokens[nr_token].str[j]=substr_start[j];
					nr_token++;
				//	default: panic("please implement me");
				}
				//nr_token++;
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

bool check_parentheses(int p, int q) {
	int i;
	if (tokens[p].type==259 && tokens[q].type==260) {
		int npar=1;
		for (i=p+1; i<q; i++) {
		if (npar==0) return false;
			if (tokens[i].type==259) npar++;
			if (tokens[i].type==260) npar--;
		}
	}
	else return false;
	return true;
}

uint32_t domope(int p, int q) {
	int i, pos=p, nper=0;

	for (i=p; i<=q; i++) {
		if (tokens[i].type==LPER) nper++;
		if (tokens[i].type==RPER) nper--;
		if ((tokens[i].type==43 || tokens[i].type==45) && (i>pos) && (nper==0)) pos=i;
	}
	if (pos==p) for (i=p; i<=q; i++) {
		if (tokens[i].type==LPER) nper++;
		if (tokens[i].type==RPER) nper--;
		if ((tokens[i].type==42 || tokens[i].type==47) && (i>pos)&& (nper==0)) pos=i;
	}
	if (pos==p) for (i=p; i<=q; i++) {
		if (tokens[i].type==LPER) nper++;
		if (tokens[i].type==RPER) nper--;
		if ((tokens[i].type==EQ) && (i>pos) && (nper==0)) pos=i;
	}
	return pos;
}

extern int find_var(char *str);

uint32_t eval(int p, int q) {
//	printf("	%d %d\n", p, q);
	if(p > q) panic(" Bad expression ");
	else if(p == q) {
//		printf("type %d str %s\n", tokens[p].type, tokens[p].str);
		if (tokens[p].type==NUM) {
			int val=0, i;
			for (i=0; i<strlen(tokens[p].str); i++) val=val*10+tokens[p].str[i]-'0';
//			printf("NUM=%d\n", val);
			return val;
		}	
		else if (tokens[p].type==NUM16) {
			int val=0, i;
			for (i=2; i<strlen(tokens[p].str); i++) 
			if (tokens[p].str[i]>='a' && tokens[p].str[i]<='z') val=val*16+tokens[p].str[i]-'a'+10;
			else val=val*16+tokens[p].str[i]-'0';
//			printf("16NUM=%d\n",val);
			return val;
		}
		else if (tokens[p].type==REG) {
//			printf("%s\n", tokens[p].str);
			if (!strcmp(tokens[p].str,"$eax")) return cpu.eax;
			else if (!strcmp(tokens[p].str,"$ecx")) return cpu.ecx;
			else if (!strcmp(tokens[p].str,"$edx")) return cpu.edx;
			else if (!strcmp(tokens[p].str,"$ebx")) return cpu.ebx;
			else if (!strcmp(tokens[p].str,"$esp")) return cpu.esp;
			else if (!strcmp(tokens[p].str,"$ebp")) return cpu.ebp;
			else if (!strcmp(tokens[p].str,"$esi")) return cpu.esi; 
			else if (!strcmp(tokens[p].str,"$edi")) return cpu.edi;
			else if (!strcmp(tokens[p].str,"$eip")) return cpu.eip;
			else panic(" Bad expression ");
		}
		else if (tokens[p].type==VAR) {
			return find_var(tokens[p].str);
		}
		else panic(" Bad expression ");
	}
	else if (check_parentheses(p, q) == true) return eval(p + 1, q - 1);
	else if (tokens[p].type==NEG) return 0-eval(p+1, q);
	else if (tokens[p].type==DEREF) {
		return swaddr_read(eval(p+1, q), 1, R_DS);
	}
	else {
		int op = domope(p, q);
	//	printf("%d	%c\n", op, tokens[op].type);
		int op_type=tokens[op].type, val1 = eval(p, op - 1), val2 = eval(op + 1, q);
		switch(op_type) {
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;
			case EQ: {
				if (val1 == val2) return 1;
				else return 0;
				}
			default: assert(1);
		}
	}
	printf("NOooooooooooooo");
	return 0;
}

uint32_t expr(char *e, bool *success) {
	int i;
	for (i=0; i<nr_token; i++)
		memset(tokens[i].str, 0 , sizeof(tokens[i].str));
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
//	printf("nr_token=%d\n", nr_token);
	for(i = 0; i < nr_token; i ++) {
		if(tokens[i].type=='*' && (i==0 || tokens[i-1].type=='+' || tokens[i-1].type == '-' || tokens[i-1].type=='*' || tokens[i-1].type=='/')) tokens[i].type=DEREF;
		if(tokens[i].type=='-' && (i==0 || tokens[i-1].type=='+' || tokens[i-1].type ==NEG  || tokens[i-1].type=='*' || tokens[i-1].type=='/')) tokens[i].type=NEG;
	}
//	printf("%s = %d\n", e, eval(0, nr_token-1));
//	int i;
//	for (i=0; i<nr_token; i++)
//		printf("%d	%s\n", tokens[i].type, tokens[i].str);	
	/* TODO: Insert codes to evaluate the expression. */
	return eval(0, nr_token-1);
//	return 0;
}

