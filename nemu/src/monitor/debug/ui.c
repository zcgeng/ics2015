#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_si(char *args) {
    int n = 1;
    if (args != NULL){
        if(sscanf(args,"%d",&n) != 1){
            printf("wrong argument\n");
            return 0;
        }
    } 
    cpu_exec(n);
    return 0;
}

static int cmd_info(char *args) {
    char command;
    if(args == NULL){
        printf("please input arguments\n");
        return 0;
    }
    if(sscanf(args,"%c",&command) != 1){
        printf("wrong argument\n");
        return 0;
    }
    if(command == 'r'){
        //print the rigisters
        int i = 0;
        for(i = 0;i < 8; ++i){
            printf("%s\t0x%x\t\t%u\n",regsl[i],reg_l(i),reg_l(i));
        }
        for(i = 0;i < 8; ++i){
            printf("%s\t0x%x\t\t%u\n",regsw[i],reg_w(i),reg_w(i));
        }
        for(i = 0;i < 8; ++i){
            printf("%s\t0x%x\t\t%u\n",regsb[i],reg_b(i),reg_b(i));
        }

        printf("eip\t0x%x\t%d\n",cpu.eip,cpu.eip);
        printf("eflags\t0x%x\t%d\n",cpu.eflags, cpu.eflags);
        printf("CF\t%x\n",cpu.CF);
        printf("PF\t%x\n",cpu.PF);
        printf("ZF\t%x\n",cpu.ZF);
        printf("SF\t%x\n",cpu.SF);
        printf("IF\t%x\n",cpu.IF);
        printf("DF\t%x\n",cpu.DF);
        printf("OF\t%x\n",cpu.OF);
    }else if(command == 'w'){
        //打印监视点信息
        print_wp();
    }else{
        printf("Unknown command: info %c\n",command);
    }
    return 0;
}

static int cmd_x(char* args){
    //扫描内存 
    int n,address;
    if(args == NULL){
        printf("please input arguments\ntype 'help' to get more informations\n");
        return 0;
    }
    if(sscanf(args,"%d %x",&n,&address) != 2){
        printf("wrong arguments\n");
        return 0;
    }
    
    int i;
    for(i = 0;i < n; ++i){
        printf("%x:\t",address);
        int j;
        for(j = 0;j < 4; ++j){
            uint8_t memory = hwaddr_read(address, 8);
            printf("%02x ",memory);
            address += 1;
        }
        printf("\n");
    }
    return 0;
}
static int cmd_p(char *args){
    if(args == NULL){
        printf("Please input an expression!\n");
        return 0;
    }
    bool success;
    int ans = expr(args, &success);
    if(!success)
        printf("There is an error occured in function expr()\n");
    else 
        printf(" : %d\t(0x%x)\n",ans,ans);
    return 0;
}

static int cmd_w(char *args){
    if(args == NULL){
        printf("Please input an expression!\n");
        return 0;
    }
    new_wp(args);
    return 0;
}

static int cmd_d(char *args){
    if(args == NULL){
        printf("Please input a number!\n");
        return 0;
    }
    int num;
    if( sscanf(args, "%d", &num) == 0 ){
        printf("Not a number!\n");
        return 0;
    }
    if( num < 0 || num >= 32 ){
        printf("n must be smaller than 32 and no smaller than 0\n");
        return 0;
    }
    free_wp(num);
    return 0;
}

static int cmd_help(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
    { "si", "si [N] top the program after N instructions.(N : default 1)" , cmd_si },
    { "info", "info r : print out the register",cmd_info },
    { "x", "x N EXPR : Calculate the EXPR and print the RAM of the next N DWORDs from the result of EXPR",cmd_x },
    { "p", "p EXPR : Calculate the EXPR and print the result out.",cmd_p },
    { "w", "w EXPR : create a watchpoint of an expression, when the value changed, the program will stop.", cmd_w },
    { "d", "d NO : delete the watchpoint which has number NO.", cmd_d }
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
