#include "common.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t cache_read(hwaddr_t, size_t);
uint32_t cache2_read(hwaddr_t, size_t);
void cache_write(hwaddr_t, size_t, uint32_t);

/* Memory accessing interfaces */
void cache2_debug(hwaddr_t addr);
void cache_debug(hwaddr_t addr);
uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	//return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	uint32_t c1 = cache_read(addr, len) & (~0u >> ((4 - len) << 3));
	uint32_t c2 = cache2_read(addr, len) & (~0u >> ((4 - len) << 3));
	if(c1 != c2){
		Log("addr = %x, len = %d, cache1 : %x, cache2 : %x\n", addr, (int)len,c1,c2);
		cache2_debug(addr);	
		cache_debug(addr);		
	}
	return cache_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	//dram_write(addr, len, data);
	cache_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}

