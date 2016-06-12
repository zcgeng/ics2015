#include "cpu/exec/helper.h"

void load_sreg(uint32_t);

make_helper(iret) {
	cpu.eip = swaddr_read(cpu.esp, 4, R_SS);
	cpu.esp += 4;

	cpu.cs = swaddr_read(cpu.esp, 4, R_SS);
	cpu.esp += 4;
	load_sreg(R_CS);

	cpu.eflags = swaddr_read(cpu.esp, 4, R_SS);
	cpu.esp += 4;

	print_asm("iret");
	return 1;
}
