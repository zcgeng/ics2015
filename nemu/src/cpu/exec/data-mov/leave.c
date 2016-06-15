#include "cpu/exec/helper.h"
/*
#define DATA_BYTE 1
#include "leave-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "leave-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "leave-template.h"
#undef DATA_BYTE
*/
/* for instruction encoding overloading */

//make_helper_v(call_i)
#define DATA_BYTE 4
#include "cpu/exec/template-start.h"
make_helper(leave) {
	cpu.esp=cpu.ebp;
	cpu.ebp=MEM_R(cpu.esp, R_SS);
	cpu.esp+=4;
//	printf("%x %x\n",cpu.esp, cpu.ebp);
	print_asm("leave");
	return 1;
}

#include "cpu/exec/template-end.h"
#undef DATA_BYTE
