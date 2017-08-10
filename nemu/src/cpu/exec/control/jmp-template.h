#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute() {
/*	printf("str %s\n", op_src->str);
	printf("type %d\n", op_src->type);
	printf("val %d\n", op_src->val);
*/
	int len = get_len();
	if (op_src->type == OP_TYPE_IMM) {
		cpu.eip += op_src->val;
		snprintf(op_src->str, OP_STR_SIZE, "$0x%x", cpu.eip + len + 1);
	}
	else {
		cpu.eip = op_src->val - len - 1;
	}
//	printf("%d\n", cpu.eip);
/*	if (op_src->type == OP_TYPE_IMM) {
//		printf("IMM\n");
		cpu.eip += op_src->val;
//		OPERAND_W(op_src, cpu.eip);
//		printf("%x %x\n", op_src->val, cpu.eip);
		if (DATA_BYTE == 2) {
			cpu.eip &= 0x0000FFFF;
		}
	}
	else if (op_src->type == OP_TYPE_REG || op_src->type == OP_TYPE_MEM){
		cpu.eip = op_src->val - 2;
		printf("%08x\n", cpu.eip);
		printf("RM\n");
		if (DATA_BYTE == 2) {
			cpu.eip &= 0x0000FFFF;
		}
	}*/
//	print_asm("jmp" str(SUFFIX) " %x", cpu.eip-1);
	print_asm_template1();
}

make_instr_helper(si)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
