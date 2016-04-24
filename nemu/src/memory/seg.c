#include "nemu.h"

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sreg) {
	if (cpu.cr0.protect_enable == 0) return addr;
	uint32_t base;
	uint32_t limit;
	if(cpu.SR_cache[sreg].valid){
		base = cpu.SR_cache[sreg].base;
	}
	else{
		//load into cache
		uint8_t tmp[8]; 
		int i;
		for(i = 0; i < 8; ++ i) 
			tmp[i] = lnaddr_read(cpu.gdtr.base + (cpu.SR[sreg].index << 3) + i, 1);
		SegDesc *segdesc = (SegDesc*)tmp;
		base = (segdesc->base_31_24 << 24) + (segdesc->base_23_16 << 16) + segdesc->base_15_0;
		limit = (segdesc->limit_19_16 << 16) + (segdesc->limit_15_0);
		cpu.SR_cache[sreg].dpl = segdesc->privilege_level;
		cpu.SR_cache[sreg].base = base;
		cpu.SR_cache[sreg].limit = limit;
		cpu.SR_cache[sreg].valid = true;
	}
	return base + addr;
}
