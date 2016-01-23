#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {
	DATA_TYPE result = op_src->val;
	if(result >> (8 * op_src->size - 1) == 1) //如果是负数
		result |= ((1 << 8 * (DATA_BYTE - op_src->size))-1) << 8 * op_src->size;
	else //正数
		result &= ((1 << 8 * op_src->size)-1);
	OPERAND_W(op_dest, result);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
