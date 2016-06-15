#include "cpu/exec/template-start.h"

#define instr mov

static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX), R_DS);

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr, R_DS);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

make_helper(concat(mov_rm2s_, SUFFIX)) {
	uint32_t rm = instr_fetch(cpu.eip+1, 1);
//	printf("%x\n", rm);

	uint8_t reg = rm & 7;
	uint8_t sreg = (rm >> 3) & 7;
	cpu.SR[sreg]._16 = REG(reg);
	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(reg), sreg);
	return 2;
}

#include "cpu/exec/template-end.h"
