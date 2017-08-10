#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_, SUFFIX)){
	MEM_W(cpu.edi, MEM_R(cpu.esi, R_DS), R_ES);
	if(cpu.DF == 0){
		cpu.esi += DATA_BYTE;
		cpu.edi += DATA_BYTE;
	}
	else{
		cpu.esi -= DATA_BYTE;
		cpu.edi -= DATA_BYTE;
	}
	print_asm("movs" str(SUFFIX));
	return 1;
}



#include "cpu/exec/template-end.h"
