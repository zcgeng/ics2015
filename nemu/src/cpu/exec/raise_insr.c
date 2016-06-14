#include <setjmp.h>
#include "cpu/exec/helper.h"
#include "../../lib-common/x86-inc/mmu.h"

extern jmp_buf jbuf;

uint32_t lnaddr_read(lnaddr_t , size_t);
//uint8_t sreg_cache(uint8_t, uint32_t);
void erode_sreg(uint8_t);

void raise_intr(uint8_t NO) {
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eflags, R_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.CS, R_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip, R_SS);
	
	uint8_t tmp[8];
	int i;
	for(i = 0; i < 8; ++ i) tmp[i] = lnaddr_read(cpu.IDTR.base + NO * 0x8 + i, 1);
	GateDesc *gde = (GateDesc*)tmp;
	cpu.SS = gde->segment;
	erode_sreg(R_CS);
	cpu.eip = (gde->offset_31_16 << 16) + gde->offset_15_0;
	/* Jump back to cpu_exec() */
	longjmp(jbuf, 1);
}
