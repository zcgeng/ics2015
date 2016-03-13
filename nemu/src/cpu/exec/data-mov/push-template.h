#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	cpu.esp -= 4;
	if(DATA_BYTE == 1){
		op_src->val &= 0x000000ff;
		op_src->val <<= 24;
		op_src->val >>= 24;
	}
	else if(DATA_BYTE == 2){
		op_src->val &= 0x0000ffff;
		op_src->val <<= 16;
		op_src->val >>= 16;
	}
	swaddr_write(cpu.esp, 4, op_src->val);
	print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(r)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
