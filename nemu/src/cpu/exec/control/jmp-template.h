#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute() {
	if(op_src->type == OP_TYPE_IMM)
		cpu.eip += op_src->val;
	else
		cpu.eip = (op_src->val) - (DATA_BYTE + 1);
	Log("eip = 0x%x src = 0x%x\n", cpu.eip, op_src->val);
	if(DATA_BYTE == 2)
		cpu.eip &= 0x0000ffff;
	print_asm("jmp $0x%x", cpu.eip + DATA_BYTE + 1);
}

make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
