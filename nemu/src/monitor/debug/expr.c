#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NUM
    
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
	{"\\+", '+'},					// plus
	{"==", EQ},			    		// equal
    {"-", '-'},                     // sub
    {"\\*", '*'},                   // mul
    {"/", '/'},                     // dev
    {"\\(", '('},                   // left bracket
    {"\\)", ')'},                   // right bracket
    {"[0-9]", NUM}                       // numbers
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
			Assert(ret != 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
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
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */

                tokens[nr_token].type = rules[i].token_type;
				switch(rules[i].token_type) {
                case '+':
                case '-':
                case '*':
                case '/':
                case '(':
                case ')':
                    break;
                case NUM:
                    if(substr_len > 32) Assert(0,"input too long number!\n");
                    strncpy(tokens[nr_token].str, substr_start, substr_len);
                    tokens[nr_token].str[31] = '\0';
                    break;
                case NOTYPE:
                    nr_token--;
                    break;
                default: panic("please implement me");
				}

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
bool check_parentheses(int p, int q){
    // 检验括号是否合法
    int count = 0;
    bool flag = true;
    for(int i = p; i <= q; ++i){
        char tmp = tokens[i].str[0];
        if( tmp == '(' )
            count++;
        else if( tmp == ')' )
            count--;
        if (count < 0) 
            flag = false;
    }
    if(count) flag = false;
    if(!flag){
        printf("表达式括号不匹配!\n");
        return 0;
    }
    // 检验是否被一对括号包围
    if(tokens[p].str[0] != '(' || tokens[q].str[0] != ')')
        return false;
    for(count = 0, int i = p+1; i <= q-1; ++i){
        char tmp = tokens[i].str[0];
        if( tmp == '(' )
            count++;
        else if( tmp == ')' )
            count--;
        if (count < 0) 
            flag = false;
    }
    if(flag)
        return true;
    else
        return false;
}
eval(p,	q){
    if(p>q){
        Assert(0,"Bad expression\n");
        /*Bad expression*/
    }
    else if(p == q){	
        /*Single token.
         * For now this token should be a number.	
         * Return the value of the number.
         */	
        if(tokens[p].type == NUM){
            int number = 0;
            char *tmp = tokens[p].str;
            while(*tmp){
                number = num * 10 + *tmp - '0';
                tmp++;
            }
            return number;
        }
    }
    else if(check_parentheses(p, q) == true) {
        /* The expression is surrounded by a matched pair of parentheses.	
         * If that is the case, just throw away the parentheses.
         */
        return eval(p + 1, q - 1);	
    }
    else{
        int op;
        int count = 0;
        for(int i = p; i <= q; ++i){
            if(tokens[i].str[0] == '(') count++;
            if(tokens[i].str[0] == ')') count--;

        }
        int val1 = eval(p, op- 1);
        int val2 = eval(op + 1, q);	

        switch(op_type) {
            case '+': return val1 + val2;
            case '-': return val1 - val2;
            case '*': return val1 * val2;
            case '/': return val1 / val2;
            default: assert(0);
        }
    }
}
uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
    return eval(0,nr_token);
	panic("please implement me");
	return 0;
}

