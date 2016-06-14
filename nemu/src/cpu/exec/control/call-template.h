#include "cpu/exec/template-start.h"

#define instr call

static void do_execute() {
	cpu.esp -= 4;
	int len = get_len() + 1;
	if(op_src->type == OP_TYPE_IMM){
		swaddr_write(cpu.esp, 4, cpu.eip + len, R_SS);// PUSH 返回地址
		cpu.eip += op_src->val;
		op_src->val += len;
	}
	else{
		swaddr_write(cpu.esp, 4, cpu.eip + len, R_SS);
		cpu.eip = (op_src->val) - len;

	}	

	print_asm_template1();
}


make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
