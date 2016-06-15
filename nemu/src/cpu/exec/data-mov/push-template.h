#include "cpu/exec/template-start.h"

#define instr push

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
		cpu.esp -= 2;
		MEM_W(cpu.esp, op_src->val, R_SS);
	}
	else {
		cpu.esp -= 4;
		swaddr_write(cpu.esp, 4, op_src->val, R_SS);	
//		MEM_W(cpu.esp, op_src->val);
	}
	print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(r)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
