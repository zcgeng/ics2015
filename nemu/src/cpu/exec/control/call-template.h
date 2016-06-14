#include "cpu/exec/template-start.h"

#define instr call

static void do_execute() {
	cpu.esp -= 4;
	int len = get_len();
		swaddr_write(cpu.esp, 4, cpu.eip + len, R_SS);// PUSH 返回地址
	if(op_src->type == OP_TYPE_IMM){
		cpu.eip += op_src->val;
		op_src->val += len;
		snprintf(op_src->str, OP_STR_SIZE, "$0x%x", cpu.eip + len + 1);
	}
	else{
		cpu.eip = (op_src->val) - len - 1;

		print_asm_template1();
	}	

}


make_instr_helper(si)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
