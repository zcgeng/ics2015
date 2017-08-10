#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);

	/* DONE: Update EFLAGS. */
	//panic("please implement me");
	update_EFLAGS_PZS(result);
	//OF: overflow flag
	if( result == ~(-1 << (8 * DATA_BYTE - 1)) ) 
		cpu.OF = 1; 
	else cpu.OF = 0;
	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
