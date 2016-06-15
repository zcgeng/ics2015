#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute() {
	DATA_TYPE ans = op_src->val;
	if (ans >> (8 * op_src->size - 1) == 1) ans |= ((1 << 8 * (DATA_BYTE - op_src->size))-1) << 8 * op_src->size;
	else ans &= ((1 << 8 * op_src->size)-1);
	OPERAND_W(op_dest, ans);
	print_asm_template2();
}

make_instr_helper(rm_b2r)
make_instr_helper(rm_w2r)

#include "cpu/exec/template-end.h"
