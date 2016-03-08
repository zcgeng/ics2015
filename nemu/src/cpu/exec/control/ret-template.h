#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute() {
	cpu.eip = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;
	cpu.eip -= (1 + DATA_BYTE);
	cpu.esp += op_src->val;
	print_asm("ret $0x%x", op_src->val);
}


make_instr_helper(i)

#include "cpu/exec/template-end.h"
