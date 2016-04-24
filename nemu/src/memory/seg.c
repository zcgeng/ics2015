#include "nemu.h"

void load_sreg(uint32_t sreg) {
	//load into cache
	uint8_t tmp[8]; 
	int i;
	uint32_t base, limit;
	//printf("%d, 0x%x\n", sreg, cpu.SR[sreg].index);
	for(i = 0; i < 8; ++ i){
		tmp[i] = lnaddr_read(cpu.gdtr.base + cpu.SR[sreg].index * 8 + i, 1);
		//printf("%d ", tmp[i]);
	}
	//printf("\n");
	SegDesc *segdesc = (SegDesc*)tmp;
	limit = (segdesc->limit_19_16 << 16) + segdesc->limit_15_0;
	base = (segdesc->base_31_24 << 24) + (segdesc->base_23_16 << 16) + segdesc->base_15_0;
	Assert(segdesc->present == 1, "Segdesc is not valid! 0x%x", cpu.gdtr.base + cpu.SR[sreg].index * 8);
	Assert(cpu.SR[sreg].index * 8 < limit, "Segment overflow!");
	cpu.SR_cache[sreg].valid = true;
	cpu.SR_cache[sreg].limit = limit;
	cpu.SR_cache[sreg].base = base;
	cpu.SR_cache[sreg].dpl = segdesc->privilege_level;
}

lnaddr_t seg_translate(swaddr_t addr, uint8_t sreg) {
	if (cpu.cr0.protect_enable == 0) return addr;
	uint32_t base;
	if(cpu.SR_cache[sreg].valid == 0) load_sreg(sreg);

	base = cpu.SR_cache[sreg].base;
	return base + addr;
}
