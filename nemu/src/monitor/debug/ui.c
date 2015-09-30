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
    if (args != NULL) sscanf(args,"%d",&n);
    cpu_exec(n);
    return 0;
}

static int cmd_info(char *args) {
    char command;
    sscanf(args,"%c",&command);
    if(command == 'r'){
        //print the rigisters
        printf("eax\t0x%0x\t%d\n",cpu.eax,cpu.eax);
        printf("ecx\t0x%0x\t%d\n",cpu.eax,cpu.ecx);
        printf("edx\t0x%0x\t%d\n",cpu.eax,cpu.edx);
        printf("ebx\t0x%0x\t%d\n",cpu.eax,cpu.ebx);
        printf("esp\t0x%0x\t%d\n",cpu.eax,cpu.esp);
        printf("ebp\t0x%0x\t%d\n",cpu.eax,cpu.ebp);
        printf("esi\t0x%0x\t%d\n",cpu.eax,cpu.esi);
        printf("edi\t0x%0x\t%d\n",cpu.eax,cpu.edi);

        printf("ax\t0x%0x\t%d\n",reg_w(0),reg_w(0));
        printf("cx\t0x%0x\t%d\n",reg_w(1),reg_w(1));
        printf("dx\t0x%0x\t%d\n",reg_w(2),reg_w(2));
        printf("bx\t0x%0x\t%d\n",reg_w(3),reg_w(3));
        printf("sp\t0x%0x\t%d\n",reg_w(4),reg_w(4));
        printf("bp\t0x%0x\t%d\n",reg_w(5),reg_w(5));
        printf("si\t0x%0x\t%d\n",reg_w(6),reg_w(6));
        printf("di\t0x%0x\t%d\n",reg_w(7),reg_w(7));

        printf("ah\t0x%0x\t%d\n",reg_b(0),reg_b(0));
        printf("ch\t0x%0x\t%d\n",reg_b(1),reg_b(1));
        printf("dh\t0x%0x\t%d\n",reg_b(2),reg_b(2));
        printf("bh\t0x%0x\t%d\n",reg_b(3),reg_b(3));
        printf("sh\t0x%0x\t%d\n",reg_b(4),reg_b(4));
        printf("bh\t0x%0x\t%d\n",reg_b(5),reg_b(5));
        printf("sh\t0x%0x\t%d\n",reg_b(6),reg_b(6));
        printf("dh\t0x%0x\t%d\n",reg_b(7),reg_b(7));

        printf("eip\t0x%0x\t%d\n",cpu.eip,cpu.eip);
    }else if(command == 'w'){
        //TODO:打印监视点信息
    }else{
        printf("Unknown command: info %c\n",command);
    }
    return 0;
}

static int cmd_x(char* args){
    //扫描内存 unfinished!!
    int n,address;
    sscanf(args,"%d %x",&n,&address);
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
