#include "cpu/exec/template-start.h"

#define instr call

static void do_execute() {
	int len = get_len(); 
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip + len, R_SS);
	if (op_src->type == OP_TYPE_IMM) {
//		MEM_W(cpu.esp, cpu.eip + DATA_BYTE);	
		cpu.eip += op_src->val;
		snprintf(op_src->str, OP_STR_SIZE, "$0x%x", cpu.eip + len + 1);
	}
	else {
//		MEM_W(cpu.esp, cpu.eip + 1); 
//		printf("%d\n", op_src->val);
		cpu.eip = op_src->val - len - 1;
	}
//	printf("%x %x\n", cpu.eip, cpu.esp);
//	printf("%d\n", cpu.eip);
	print_asm_template1();
/*	cpu.esp -= DATA_BYTE;
	MEM_W(cpu.esp, cpu.eip + op_src->val + 1);
	print_asm("call" str(SUFFIX) " %x", cpu.eip + op_src->val + DATA_BYTE + 1);
	cpu.eip += op_src->val;*/
}

make_instr_helper(si)
make_instr_helper(rm)
/*
make_helper(concat(call_rel_, SUFFIX)) {
	DATA_TYPE rel = instr_fetch(eip + 1, DATA_BYTE);
	reg_l(R_ESP) -= 4;
	MEM_W(reg_l(R_ESP),eip + 1 + DATA_BYTE);
	print_asm("call" str(SUFFIX) " %x",eip + rel + DATA_BYTE + 1);
	cpu.eip += rel;
	return 1 + DATA_BYTE;
}
*/
#include "cpu/exec/template-end.h"
