#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute() {
	cpu.eip += swaddr_read(cpu.esp, 4);
	cpu.esp += 4;
	cpu.eip -= (1 + DATA_BYTE * 8);
	print_asm("ret $0x%x", cpu.eip + DATA_BYTE + 1);
}


make_instr_helper(i)

#include "cpu/exec/template-end.h"
