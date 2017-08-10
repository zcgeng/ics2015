#include "common.h"
#include "burst.h"
#include "misc.h"

/* Simulate the (main) behavor of DRAM.
 * Although this will lower the performace of NEMU, it makes
 * you clear about how DRAM perform read/write operations.
 * Note that cross addressing is not simulated.
 */

#define SET_WIDTH 7
#define ROW_WIDTH 3
#define COL_WIDTH 6
#define FLAG_WIDTH (27 - COL_WIDTH - SET_WIDTH)

typedef union {
	struct {
		uint32_t col	: COL_WIDTH;
		uint32_t set	: SET_WIDTH;
		uint32_t flag	: FLAG_WIDTH;
	};
	uint32_t addr;
} cache_addr;


#define NR_COL (1 << COL_WIDTH)
#define NR_ROW (1 << ROW_WIDTH)
#define NR_SET (1 << SET_WIDTH)
#define NR_FLAG (1 << FLAG_WIDTH)

#define HW_MEM_SIZE (1 << 27)

typedef struct {
	uint8_t data[NR_ROW][NR_COL];
	uint32_t flag[NR_ROW];
	bool valid[NR_ROW];
} CACHE;

CACHE cache[NR_SET];
uint64_t cpu_time;

uint32_t dram_read(hwaddr_t, size_t);
uint32_t L2_cache_read(hwaddr_t, size_t);
//void dram_write(hwaddr_t, size_t, uint32_t);

void init_cache() {
	memset(cache, 0, sizeof(cache));
	cpu_time=0;
}

static uint32_t rand(uint32_t addr) {
	return (addr%1007);
}

static void cache_in_read(hwaddr_t addr, void *data) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	cache_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t col = temp.col;
	uint32_t set = temp.set;
	uint32_t flag = temp.flag;
	bool full=true;
	bool find=false;
	uint32_t row, k;
	for (row=0; row<NR_ROW; row++) {
		if (cache[set].valid[row]) {
			if (cache[set].flag[row]==flag) {
				memcpy(data, cache[set].data[row]+col, BURST_LEN);
				find=true;
				break;
			}
			else {
				full=false;
				k=row;
			}
		}
	}
	if (find==false) {
		cpu_time+=200;
		if (full) k=rand(addr) & (NR_ROW - 1);
		cache[set].valid[k]=true;
		cache[set].flag[k]=flag;
		uint32_t i;
		for (i=0; i<NR_COL; i++) {
		//	cache[set].data[k][i]=dram_read((addr & ~(NR_COL-1))+i, 1);
			cache[set].data[k][i]=L2_cache_read((addr & ~(NR_COL-1))+i, 1);
		}
		memcpy(data, cache[set].data[k]+col, BURST_LEN);
	}
	else cpu_time+=2;
}
static void cache_in_write(hwaddr_t addr, void *data, uint8_t *mask) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);
	cache_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t col = temp.col;
	uint32_t set = temp.set;
	uint32_t flag = temp.flag;
	uint32_t row;
	bool find=false;
	for (row=0; row<NR_ROW; row++) {
		if(cache[set].valid[row] && cache[set].flag[row]==flag) {
			memcpy_with_mask(cache[set].data[row] + col, data, BURST_LEN, mask);
			find=true;
			break;
		}
	}
	if (find) cpu_time+=2;
	else cpu_time+=200;
}

uint32_t cache_read(hwaddr_t addr, size_t len) {
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	
	cache_in_read(addr, temp);
	
	if(offset + len > BURST_LEN) {
		/* data cross the burst boundary */	
		cache_in_read(addr + BURST_LEN, temp + BURST_LEN);
	}
								
	return unalign_rw(temp + offset, 4);
}

void cache_write(hwaddr_t addr, size_t len, uint32_t data) {
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	uint8_t mask[2 * BURST_LEN];
	memset(mask, 0, 2 * BURST_LEN);

	*(uint32_t *)(temp + offset) = data;
	memset(mask + offset, 1, len);
	
	cache_in_write(addr, temp, mask);

	if(offset + len > BURST_LEN) {
		/* data cross the brust boundary */
		cache_in_write(addr + BURST_LEN, temp + BURST_LEN, mask + BURST_LEN);
	}
}
