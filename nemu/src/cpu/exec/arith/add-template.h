#include "cpu/exec/template-start.h"

#define instr add

static void do_execute() {
	//OF, SF, ZF, AF, CF, PF 
	DATA_TYPE ans=op_dest->val + op_src->val;
	DATA_TYPE pf=(ans & 255);
	pf=(pf>>4)&pf;
	pf=(pf>>2)&pf;
	pf=(pf>>1)&pf;
	eflags.PF=(pf & 1);
	eflags.ZF=(ans == 0);
	eflags.SF=MSB(ans);

	eflags.CF=ans < op_src->val;
	if ((MSB(op_dest->val)==MSB(op_src->val))&&(MSB(op_src->val)!=MSB(ans)))  eflags.OF=1; else eflags.OF=0;
	OPERAND_W(op_dest, ans);
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
