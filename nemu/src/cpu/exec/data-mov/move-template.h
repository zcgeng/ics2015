#include "cpu/exec/template-start.h"

#define instr move
/*
static void do_execute() {
	printf("%x %x\n", op_dest->val, op_src->val);
//	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2rm)
make_instr_helper(rm2i)
*/
void init_tlb();

make_helper(concat(move_cr2r_, SUFFIX)) { //0f 20
	uint8_t reg=instr_fetch(eip+1, 1) & 0x7;
	uint8_t sreg=(instr_fetch(eip+1, 1)>>3) & 0x7;
//	printf("cr2r REG = %d\n", sreg);
	if (sreg==0) REG(reg)=cpu.cr0.val;
	else if (sreg==3) REG(reg)=cpu.cr3.val;
	print_asm("mov" str(SUFFIX) " %%cr%d, %%%s", sreg, REG_NAME(reg));
	return 2; 
}

make_helper(concat(move_r2cr_, SUFFIX)) { // 0f 22
	uint8_t reg=instr_fetch(eip+1, 1) & 0x7;
	uint8_t sreg=(instr_fetch(eip+1, 1)>>3) & 0x7;
//	printf("r2cr REG = %d %x\n", sreg, REG(reg));
	if (sreg==0) cpu.cr0.val=REG(reg);
	else if (sreg==3) {
		cpu.cr3.val=REG(reg);
		init_tlb();
	}
//	printf("cr3 %x\n", cpu.cr3.val);
	print_asm("mov" str(SUFFIX) " %%%s, %%cr%d", REG_NAME(reg), sreg);
	return 2;
}

#include "cpu/exec/template-end.h"
