#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	DATA_TYPE result = op_src->val + 1;
	OPERAND_W(op_src, result);

	DATA_TYPE pf=(result & 255);
	pf=(pf>>4)&pf;
	pf=(pf>>2)&pf;
	pf=(pf>>1)&pf;
	eflags.PF=(pf & 1);
	eflags.ZF=(result == 0);
	eflags.SF=MSB(result);
	
	if (MSB(op_src->val) == 0 && MSB(result) != MSB(op_src->val)) eflags.OF=1; else eflags.OF=0;
	eflags.CF=result < op_src->val;

	/* TODO: Update EFLAGS. */
//	panic("please implement me");

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
