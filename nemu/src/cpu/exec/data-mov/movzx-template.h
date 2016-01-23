#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute() {
	DATA_TYPE result = op_src->val & ((1ll << (op_src->size * 8)) - 1);
	OPERAND_W(op_dest, result);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
