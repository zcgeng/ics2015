#include "cpu/exec/template-start.h"

#define instr cmps

make_helper(concat3(instr, _, SUFFIX)) {
	DATA_TYPE dest, src;
	dest = swaddr_read(cpu.esi, DATA_BYTE, R_SS);
	src = swaddr_read(cpu.edi,  DATA_BYTE, R_ES);
	DATA_TYPE result = dest - src;
    update_EFLAGS_PZS(result);
    //CF: 借位
    cpu.CF = (((long long)dest - (long long)src) >> (8 * DATA_BYTE)) & 1;
    //AF: 低三位是否借位
    cpu.AF = (dest & 0x8) > (src & 0x8) ? 0 : 1;
    //OF: overflow flag, 同号相减不会溢出;异号相减, 结果与被减数反号则溢出.
    if( MSB(dest) != MSB(src) && MSB(result) != MSB(dest)) 
        cpu.OF = 1; 
    else cpu.OF = 0;

	cpu.edi += (cpu.DF == 0 ? DATA_BYTE : -DATA_BYTE);
	cpu.esi += (cpu.DF == 0 ? DATA_BYTE : -DATA_BYTE);
	print_asm("cmps" str(SUFFIX));
	return 1;
}



#include "cpu/exec/template-end.h"
