#include "cpu/exec/template-start.h"

#define instr cmps

static void do_execute() {
	op_src->type = op_dest->type = OP_TYPE_REG;
	op_src->reg = R_ESI; 
	op_dest->reg = R_EDI;
	snprintf(op_src->str, 11, "%%es:(%%edi)");
	snprintf(op_dest->str, 11, "%%ds:(%%esi)");
	OPERAND_W(op_src, MEM_R(cpu.edi, R_ES));
	OPERAND_W(op_dest, MEM_R(cpu.esi, R_DS));
	DATA_TYPE ans = op_dest->val - op_src->val;
	DATA_TYPE pf=(ans & 255);
	pf=(pf>>4)&pf;
	pf=(pf>>2)&pf;
	pf=(pf>>1)&pf;
	eflags.PF=(pf & 1);
	eflags.ZF=(ans == 0);
	eflags.SF=MSB(ans);
	eflags.CF=op_dest->val < op_src->val;
	if ((MSB(op_dest->val)!=MSB(op_src->val))&&(MSB(op_dest->val)==MSB(ans)))
	eflags.OF = 1; else eflags.OF = 0;
	cpu.edi += eflags.DF == 0? +DATA_BYTE : -DATA_BYTE;
	cpu.esi += eflags.DF == 0? +DATA_BYTE : -DATA_BYTE;
	print_asm_template2();
}

make_instr_helper(m)

#include "cpu/exec/template-end.h"
