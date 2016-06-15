#include "cpu/exec/template-start.h"

/*
#define instr CC

static void do_execute(){
	if (COND) cpu.eip += op_src->val;
	print_asm_template1();
}
*/
make_helper(concat(concat3(j, CC, _),SUFFIX)) {
	DATA_TYPE imm = op_src->val;//instr_fetch(eip+1, DATA_BYTE);
	if (COND) cpu.eip += imm;
	print_asm("j"str(CC)"\t %x", cpu.eip+imm+1+DATA_BYTE);
	return 1+DATA_BYTE;
}

//make_instr_helper(i);

#include "cpu/exec/template-end.h"
