#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute() {
	cpu.eip = swaddr_read(cpu.esp, 4, R_SS);
	cpu.esp = cpu.esp + 4 + op_src->val;
	if(op_src->val == 0) print_asm("ret"); 
	else {
		cpu.eip -= 2;
		print_asm_template1();
	}
}


make_instr_helper(i)

#include "cpu/exec/template-end.h"
