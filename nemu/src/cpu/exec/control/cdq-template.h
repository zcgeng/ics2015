#include "cpu/exec/template-start.h"

make_helper(concat(cdq_, SUFFIX)) {
	if((cpu.eax >> (DATA_BYTE * 8 - 1)) != 0) 
		cpu.edx = (1ll << (DATA_BYTE * 8)) - 1; 
	else cpu.edx = 0x0;
	print_asm(DATA_BYTE == 4 ? "cltd" : "cwtl");
	return 1;
}

#include "cpu/exec/template-end.h"
