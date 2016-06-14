#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);
WP* new_wp();
void free_wp(char*);
uint32_t expr(char*, bool*);
void print_w();

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
extern int cpu_time;
static int cmd_q(char *args) {
	printf(" Total cpu time = %d\n", cpu_time);
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args){
	int i;
	if (args==NULL) i=1;
	else sscanf(args, "%d", &i);
	cpu_exec(i);
//	printf("%d", i);
//	cpu_exec(arg);
	return 0;
}

static int cmd_info(char *args){
//	int i;
	char ch;
	sscanf(args, "%c", &ch);
	if (ch=='r') {
	//	for (i=0; i<8; i++)
		//	printf("%08x %08x %08x %08x\n", cpu.gpr[i]._32, cpu.gpr[i]._16, cpu.gpr[i]._8[0], cpu.gpr[i]._8[1]); 
		printf("eax:%08x ecx:%08x edx:%08x ebx:%08x\n", cpu.eax, cpu.ecx, cpu.edx, cpu.ebx);
		printf("esp:%08x ebp:%08x esi:%08x edi:%08x\n", cpu.esp, cpu.ebp, cpu.esi, cpu.edi);
		printf("eip:%08x\n", cpu.eip);
		printf("CF:%d PF:%d AF:%d DF:%d\n", cpu.CF, cpu.PF, cpu.AF, cpu.DF);
		printf("ZF:%d SF:%d OF:%d\n", cpu.ZF, cpu.SF, cpu.OF);
		printf("cpu.CR0:%08x cpu.PE:%d cpu.PG:%d\n", cpu.cr0.val, cpu.cr0.protect_enable, cpu.cr0.paging);
		printf("cpu.CR3:%08x\n", cpu.cr3.val);
		printf("cpu.GDTR.base:%08x cpu.GDTR.limit:%d\n", cpu.GDTR.base, cpu.GDTR.limit);
	}
	else if (ch=='w') print_w();
	return 0;
}

static int cmd_p(char *args){
	bool success=true;
//	uint32_t v=expr(args, &success);
	printf("%d\n", expr(args, &success));
	return 0;
}

static int cmd_x(char *args) {
	int i, j, ad, expr=0;
	sscanf(args, "%d %X", &ad, &expr);
	int a;
	for (i=0; i<ad; i++){
		
		for (j=0; j<4; j++) {
			a=swaddr_read(expr+i*4+j, 1, R_DS);
			printf("%02x ", a);
		}
		printf("\n");
	}
	return 0;
}

static int cmd_w(char *args) {
//	printf("%s\n", args);
	WP *new=new_wp();
//	printf("%d\n", (*new).NO);
	int i;
	for (i=0; i<strlen(args); i++)
		(*new).expr[i]=args[i];
	bool success=true;
	(*new).value=expr(args, &success);
	printf("%d\n", (*new).value);
	return 0;
}

static int cmd_d(char *args) {
//	printf("d %s\n", args);
	free_wp(args);
	return 0;
}

extern bool find_stack(int addr, char *str);

static int cmd_bt(char *args) {
	int ebp = cpu.ebp;
	int eip = cpu.eip;
	int cnt = 0;
	char str[32];
	if(!find_stack(eip, str)) {
		printf("No stack.\n");
		return 0;
	}
	while(find_stack(eip, str)) {
		printf("#%d eip=0x%x in %s(%d, %d, %d, %d)\n", ++ cnt, eip, str, 
		swaddr_read(ebp + 0x8, 4, R_SS), swaddr_read(ebp + 0xc, 4, R_SS), 
		swaddr_read(ebp + 0x10, 4, R_SS), swaddr_read(ebp + 0x14, 4, R_SS));
		eip = swaddr_read(ebp + 4, 4, R_SS);
		ebp = swaddr_read(ebp, 4, R_SS);
	}
	return 0;
}

static int cmd_cache(char *args) {
	return 0;
}

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help        ", "Display informations about all supported commands", cmd_help },
	{ "c           ", "Continue the execution of the program", cmd_c},
	{ "q           ", "Exit NEMU", cmd_q },
	{ "si [N]      ", "Run one instruction exactly", cmd_si },
	{ "info SUBCMD ", "List of intger registers and their contents", cmd_info },
	{ "p EXPR      ", "Print value of expression EXP", cmd_p },
	{ "x n EXPR    ", "Starting display address", cmd_x},
	{ "w EXPR      ", "Set a watchpoint whenever the value of EXPR changes", cmd_w},
	{ "d N         ", "Delete the Nth watchpoint", cmd_d},
	{ "bt          ", "Print the stack", cmd_bt},
	{ "cache ADDR  ", "Using the ADDR to find cache", cmd_cache},
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
			if(strstr(cmd_table[i].name, arg) == cmd_table[i].name) {
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
			if(strstr(cmd_table[i].name, cmd) == cmd_table[i].name) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
