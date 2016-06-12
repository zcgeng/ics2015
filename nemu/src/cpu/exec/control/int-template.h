#include "cpu/exec/template-start.h"

#define instr int
void raise_intr(uint8_t NO);

static void do_execute() {
	cpu.esp -= 4;
	if(op_src->type == OP_TYPE_IMM){
		int len = DATA_BYTE + 1;
		cpu.eip += len;
		print_asm_template1();
		raise_intr(op_src->val);
	}
	else{
		panic("error");
	}	
	
}


make_instr_helper(i)

#include "cpu/exec/template-end.h"
