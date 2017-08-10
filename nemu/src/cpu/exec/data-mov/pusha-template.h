#include "cpu/exec/template-start.h"

#define instr pusha

static void do_execute() {
	int i;
	uint32_t temp=cpu.esp;
	for (i = 0; i < 8; i++) {
		cpu.esp -= DATA_BYTE;
		if (i == 4) swaddr_write(cpu.esp, DATA_BYTE, temp, R_SS);
		else swaddr_write(cpu.esp, DATA_BYTE, cpu.gpr[i]._32, R_SS);
//		printf("%d %x\n", i, cpu.gpr[i]._32);
	}
	print_asm_template1();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
