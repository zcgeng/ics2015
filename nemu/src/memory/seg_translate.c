#include "common.h"
#include "cpu/reg.h"
#include "../../lib-common/x86-inc/mmu.h"
#include <stdio.h>

uint32_t lnaddr_read(lnaddr_t addr, size_t len);
//uint8_t sreg_cache;

void erode_sreg(uint8_t sreg) {
	uint8_t tmp[8]; 
	int i;
	for(i = 0; i < 8; ++ i) 
		tmp[i] = lnaddr_read(cpu.GDTR.base + cpu.SR[sreg].INDEX * 8 + i, 1);
	SegDesc *segdesc = (SegDesc*)tmp;
	uint32_t base;
	base=(segdesc->base_31_24 << 24) + (segdesc->base_23_16 << 16) + segdesc->base_15_0;
	cpu.SR_cache[sreg].base=base;
	cpu.SR_cache[sreg].vaild=true;
}

lnaddr_t seg_translate(swaddr_t addr, uint8_t sreg) {
	uint32_t base;
	if (cpu.cr0.protect_enable == 0) return addr;
	if (cpu.SR_cache[sreg].vaild) {
		base=cpu.SR_cache[sreg].base;
	}
	else {
		erode_sreg(sreg);
		base=cpu.SR_cache[sreg].base;
	}
	return base + addr;
	//lnaddr_t segbase=lnaddr_read(cpu.GDTR.base+cpu.SR[sreg].INDEX*8, 4);
//	printf("%x\n", cpu.GDTR.base);
//	printf("%x\n", segbase);
//	return segbase+addr;
}
