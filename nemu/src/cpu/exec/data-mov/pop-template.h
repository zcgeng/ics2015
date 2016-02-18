#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
	DATA_TYPE result = swaddr_read(cpu.esp, DATA_BYTE);
	cpu.esp += DATA_BYTE;
	OPERAND_W(op_src, result);
	print_asm_template1();
}

make_instr_helper(r)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"