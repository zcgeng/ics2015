#include "cpu/exec/template-start.h"

#define instr ljmp

make_helper(concat(ljmp_, SUFFIX)) {
	uint32_t addr = instr_fetch(cpu.eip+1, 4);
	uint8_t sreg = instr_fetch(cpu.eip+5, 2);
//	printf("%x %x\n", addr, sreg);
	cpu.eip = addr-7;
	cpu.CS = sreg;
	cpu.SR_cache[1].vaild=false;
	print_asm("ljmp" str(SUFFIX) " $0x%x,$0x%x", sreg, addr);
	return 7;
}

#include "cpu/exec/template-end.h"
