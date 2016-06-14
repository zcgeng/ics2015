#include "cpu/exec/template-start.h"

#define instr stos

#if DATA_BYTE == 1
#define MASK ((uint32_t) 0xff)
#elif DATA_BYTE == 2
#define MASK ((uint32_t) 0xffff)
#elif DATA_BYTE == 4
#define MASK ((uint32_t) 0xffffffff)
#else
#error unknown DATA_BYTE
#endif

make_helper(concat(stos_, SUFFIX)){
	MEM_W(cpu.edi, REG(R_EAX) & MASK, R_ES);
	if(cpu.DF == 0) cpu.edi += DATA_BYTE;
	else cpu.edi -= DATA_BYTE;
	print_asm("stos" str(SUFFIX));
	return 1;
}

#undef MASK


#include "cpu/exec/template-end.h"
