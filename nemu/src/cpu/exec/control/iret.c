#include "cpu/exec/helper.h"

void erode_sreg(uint8_t);

make_helper(iret) {
	cpu.eip = swaddr_read(cpu.esp, 4, R_SS);
	cpu.esp += 4;
	cpu.CS = swaddr_read(cpu.esp, 4, R_SS);
	cpu.esp += 4;
	erode_sreg(R_CS);
	eflags.val = swaddr_read(cpu.esp, 4, R_SS);
	cpu.esp += 4;
//	printf("cpu.eip=%x cpu.CS=%x eflags=%x", cpu.eip, cpu.CS, eflags.val);
	print_asm("iret");
	return 1;
}


