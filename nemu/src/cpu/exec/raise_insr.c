#include <setjmp.h>
#include "cpu/exec/helper.h"
#include "../../lib-common/x86-inc/mmu.h"

extern jmp_buf jbuf;

uint32_t lnaddr_read(lnaddr_t , size_t);
//uint8_t sreg_cache(uint8_t, uint32_t);
void erode_sreg(uint8_t);

void raise_intr(uint8_t NO, uint32_t len) {
	/* TODO: Trigger an interrupt/exception with ``NO''.
	 * That is, use ``NO'' to index the IDT.
 	 */
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, eflags.val, R_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.CS, R_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip + len, R_SS);
//	printf("eflags=%d cs=%d eip=%x\n", eflags.val, cpu.CS, cpu.eip+len);
//	printf("esp=%x\n", cpu.esp);	
	uint8_t idt[8];
	int i;
	for (i = 0; i < 8; i++)
		idt[i]=lnaddr_read(cpu.IDTR.base + 8 * NO + i, 1);
 	GateDesc *gde = (GateDesc*) idt;
	cpu.SS = gde->segment;
	erode_sreg(R_CS);
	cpu.eip = (gde->offset_31_16 << 16) + gde->offset_15_0;
	/* Jump back to cpu_exec() */
	longjmp(jbuf, 1);
}
