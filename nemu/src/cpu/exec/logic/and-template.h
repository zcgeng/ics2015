#include "cpu/exec/template-start.h"

#define instr and

static void do_execute () {
	DATA_TYPE ans = op_dest->val & op_src->val;
	OPERAND_W(op_dest, ans);
	/* TODO: Update EFLAGS. */
	DATA_TYPE pf=(ans & 255);
	pf=(pf>>4)&pf;
	pf=(pf>>2)&pf;
	pf=(pf>>1)&pf;
	eflags.PF=(pf & 1);
	eflags.ZF=(ans == 0);
	eflags.SF=MSB(ans);

	eflags.CF=eflags.OF=0;

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
