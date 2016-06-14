#include "cpu/exec/template-start.h"

#define instr cbw_cwde

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat3(instr, _, SUFFIX))
{
#if DATA_BYTE == 2
    signed char AL = reg_b(R_AL);
    reg_w(R_AX) = (short) AL;
    print_asm("cbw");
#elif DATA_BYTE == 4
	short AX = reg_w(R_AX);
    cpu.eax = (int) AX;
    print_asm("cwde");
#else
#error unknown DATA_BYTE
#endif
    return 1;
}

#endif


#include "cpu/exec/template-end.h"
