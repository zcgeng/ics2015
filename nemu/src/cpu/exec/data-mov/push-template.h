#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	int len;
	len = (DATA_BYTE == 2 ? 2 : 4);
	cpu.esp -= len;
	swaddr_write(cpu.eip, DATA_BYTE, op_src->val);
	print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(r)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
