#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movsv_, SUFFIX)){
	MEM_W(cpu.edi, MEM_R(cpu.esi));
	if(cpu.DF == 0){
		cpu.esi += DATA_BYTE;
		cpu.edi += DATA_BYTE;
	}
	else{
		cpu.esi -= DATA_BYTE;
		cpu.edi -= DATA_BYTE;
	}
	print_asm_template2();
}



#include "cpu/exec/template-end.h"
