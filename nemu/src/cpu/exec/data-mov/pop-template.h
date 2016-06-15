#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute() {
/*	int reg_code = instr_fetch(cpu.eip, 1) & 0x7;
	reg_l(R_ESP) -= DATA_BYTE;
	MEM_W(reg_l(R_ESP),REG(reg_code));
	printf("%d\n", reg_code);
	printf("val %x\n", op_src->val);
	printf("type %d\n", op_src->type);
	printf("size %d\n", op_src->size);
	printf("str %s\n", op_src->str);
	printf("opcode %x\n", ops_decoded.opcode);
	printf("is_data_size_16 %d\n", ops_decoded.is_data_size_16);
*/
	if (DATA_BYTE == 2) {
		OPERAND_W(op_src, MEM_R(cpu.esp, R_SS));
		cpu.esp += 2;
	}
	else {
		OPERAND_W(op_src, MEM_R(cpu.esp, R_SS));
		cpu.esp += 4;
	}
	print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(r)

#include "cpu/exec/template-end.h"
