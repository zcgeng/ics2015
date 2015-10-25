#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NEQ, SHL, SHR, LE, GE, NUM, MINUS, DEREF
    
	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
    int precedence; // 优先级,无意义时值为99
    bool associate; // 结合性:从左向右结合为1,从右向左为0
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE, 99, 0},				// spaces
	{"\\+", '+', 12, 1},					// plus
	{"==", EQ, 10, 1},			    		// equal
    {"!=", NEQ, 10, 1},
    {"-", '-', 12, 1},                     // sub
    {"\\*", '*', 13, 1},                   // mul
    {"/", '/', 13, 1},                     // dev
    {"\\(", '(', 15, 1},                   // left bracket
    {"\\)", ')', 15, 1},                   // right bracket
    {"%", '%', 13, 1},                      // mod
    {"<<", SHL, 11, 1},                     // shift left
    {">>", SHR, 11, 1},                     // shift right
    {">=", GE, 10, 1},
    {"<=", LE, 10, 1},
    {">", '>', 10, 1},
    {"<", '<', 10, 1},
    {"[0-9]+", NUM, 99, 0},                       // numbers
    {"-", MINUS, 14, 0},                    // 取负
    {"*", DEREF, 14, 0}                     // 取值
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
    int precedence;
    bool associate;
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

				//Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */

                tokens[nr_token].type = rules[i].token_type;
                tokens[nr_token].associate = rules[i].associate;
                tokens[nr_token].precedence = rules[i].precedence;
				switch(rules[i].token_type) {
                case '+':
                case '-':
                case '*':
                case '/':
                case '(':
                case ')':
                case '%':
                case '>':
                case '<':
                case EQ:
                case NEQ:
                case SHL:
                case SHR:
                case GE:
                case LE:
                    break;
                case NUM:
                    if(substr_len > 31) Assert(0,"input too long number!\n");
                    strncpy(tokens[nr_token].str, substr_start, substr_len);
                    tokens[nr_token].str[substr_len] = '\0';
                    break;
                case NOTYPE:
                    nr_token--;
                    break;
                default: printf("can not recognize the token type : %d\n",rules[i].token_type);
                         return false;
				}
                nr_token++;

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
    // 只检验是否被一对括号包围
    int count = 0;
    bool flag = true;
    if(tokens[p].type != '(' || tokens[q].type != ')')
        return false;
    int i;
    for(count = 0, i = p+1; i <= q-1; ++i){
        int tmp = tokens[i].type;
        if( tmp == '(' )
            count++;
        else if( tmp == ')' )
            count--;
        if (count < 0) 
            flag = false;
    }
    if(count != 0) flag = false;
    return flag;
}
int eval(int p, int q, bool *success){
    if(p>q){
        printf("Bad expression : p > q\n");
        *success = false;
        return 0;
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
                number = number * 10 + *tmp - '0';
                tmp++;
            }
            return number;
        }
    }
    else if(check_parentheses(p, q) == true) {
        /* The expression is surrounded by a matched pair of parentheses.	
         * If that is the case, just throw away the parentheses.
         */
        return eval(p + 1, q - 1,success);	
    }
    else{
        bool par_err = false; 
        int op = p;
        int count = 0;
        int i = 0;
        for(i = p; i <= q; ++i){
            if(tokens[i].type == '(') count++;
            else if(tokens[i].type == ')') count--;
            else if(count == 0){
                if(tokens[i].precedence < tokens[op].precedence)
                    op = i; // 找到最低优先级
                if(tokens[i].precedence == tokens[op].precedence && tokens[i].associate == 1)
                    op = i; // 找到最后结合的 
            }
            if(count < 0) par_err = true;
        }
        if(count != 0) par_err = true;
        if(par_err){
            printf("parentheses error!\n");
            *success = false;
            return 1;
        }
        
        int val1 = eval(p, op- 1,success);
        int val2 = eval(op + 1, q,success);	
        int op_type = tokens[op].type;
        switch(op_type) {
            case '+': return val1 + val2;
            case '-': return val1 - val2;
            case MINUS: return -val2;
            case DEREF: return 0; // TODO
            case '*': return val1 * val2;
            case '/': 
                if(val2==0){
                    printf("Error! divided by 0\n");
                    *success = false;
                    return 1;
                }
                else return val1/val2;
            case '%':
                if(val2==0){
                    printf("Error! mod by 0\n");
                    *success = false;
                    return 1;
                }
                else return val1 % val2;
            case EQ : return val1 == val2;
            case NEQ : return val1 != val2;
            case '>' : return val1 > val2;
            case '<' : return val1 < val2;
            case SHL : return val1 << val2;
            case SHR : return val1 >> val2;
            case GE : return val1 >= val2;
            case LE : return val1 <= val2;
            default: printf("op_type : didn't find %d\n",op_type);
                    *success = false;
                    return 1;
        }
    }
    *success = false;
    return 1;
}
uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
    int i = 0;
    for(i = 0; i < nr_token; i++){
        if(tokens[i].type == '-' && (i == 0 || tokens[i-1].type != NUM))
            tokens[i].type = MINUS;// 只有'-'前面是数字时,它才是减号
        if(tokens[i].type == '*' && (i == 0 || tokens[i-1].type != NUM))
            tokens[i].type = DEREF;
    }
    *success = true;
    return eval(0,nr_token-1,success);
	//panic("please implement me");
	return 0;
}

