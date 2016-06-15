#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "ret-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "ret-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "ret-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

/* for instruction encoding overloading */
/*
#define DATA_BYTE 4
#include "cpu/exec/template-start.h"
make_helper(ret) {
	cpu.eip = MEM_R(cpu.esp);
	cpu.esp +=DATA_BYTE; 
	if (ops_decoded.is_data_size_16) cpu.eip &= 0x0000ffff;
	print_asm("ret\n");
	return 0;
}

#include "cpu/exec/template-end.h"
#undef DATA_BYTE*/
