#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "mov-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(mov_i2r)
make_helper_v(mov_i2rm)
make_helper_v(mov_r2rm)
make_helper_v(mov_rm2r)
make_helper_v(mov_a2moffs)
make_helper_v(mov_moffs2a)

make_helper(mov_cr2r){
	decode_r_l_intrenal(eip + 1, op_dest);
	// uint8_t opcode = instr_fetch(cpu.eip + 1, 1);
	OPREND_W(op_dest, cpu.cr0.val);
	print_asm("mov %%cr0, %s", op_dest->str);
	return 2;
}

make_helper(mov_r2cr){
	decode_r_l_intrenal(eip + 1, op_src);
	// uint8_t opcode = instr_fetch(cpu.eip + 1, 1);
	cpu.cr0.val = op_src->val;
	print_asm("mov %s, %%cr0", op_src->str);
	return 2;
}
