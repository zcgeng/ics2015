#include "common.h"

uint32_t dram_read(hwaddr_t, size_t);
uint32_t cache_read(hwaddr_t, size_t);
uint32_t L2_cache_read(hwaddr_t, size_t);
uint32_t mmio_read(hwaddr_t, size_t, int);

void dram_write(hwaddr_t, size_t, uint32_t);
void cache_write(hwaddr_t, size_t, uint32_t);
void L2_cache_write(hwaddr_t, size_t, uint32_t);
void mmio_write(hwaddr_t, size_t, uint32_t, int);

lnaddr_t seg_translate(swaddr_t, uint8_t);
hwaddr_t page_translate(lnaddr_t);
uint32_t is_mmio(hwaddr_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
//	if (addr>8388608) printf("%x\n", addr);
	int num;
	num = is_mmio(addr);
	if (num==-1) return cache_read(addr, len) & (~0u >> ((4 - len) << 3));
	else return mmio_read(addr, len, num) & (~0u >> ((4 - len) << 3));
//	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	int num;
	num = is_mmio(addr);
	if (num==-1) {
		cache_write(addr, len, data);
		L2_cache_write(addr, len, data);
	}
	else mmio_write(addr, len, data, num);
//	dram_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
//	if ((addr&0xfff)+len-1>0x1000) {
	/* this is a special case, you can handle it later. */
//		panic("Page cross the page boundary !");//ssert(0);
//	}
//	else {
		hwaddr_t hwaddr = page_translate(addr);
		return hwaddr_read(hwaddr, len);
//	}
	//return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
//	if ((addr&0xfff)+len-1>0x1000) {
	/* this is a special case, you can handle it later. */
//		panic("Page cross the page boundary !");//ssert(0);
//	}
//	else {
		hwaddr_t hwaddr = page_translate(addr);
		hwaddr_write(hwaddr, len, data);
//	}
	//hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr, sreg);
	//printf("%x	%x\n", lnaddr, addr);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr, sreg);
	//printf("%x	%x\n", lnaddr, addr);
	lnaddr_write(lnaddr, len, data);
}

