#include "cpu/exec/template-start.h"

#define instr test

static void do_execute() {
	cpu.OF = 0;
	cpu.CF = 0;
	DATA_TYPE result = op_src->val & op_dest->val;
	update_EFLAGS_PZS(result);
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"
