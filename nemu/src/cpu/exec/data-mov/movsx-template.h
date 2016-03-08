#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {
	DATA_TYPE_S result = op_src->val;
	//Log("0x%x %d %d %d\n",result, op_src->val, (int)op_dest->size, (int)op_src->size);
	result <<= (op_src->size - op_dest->size) * 8;
	//Log("0x%x\n", result);
	result >>= (op_src->size - op_dest->size) * 8;
	//Log("0x%x\n", result);
	OPERAND_W(op_dest, result);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
