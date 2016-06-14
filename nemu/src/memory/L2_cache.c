#include "common.h"
#include "burst.h"
#include "misc.h"

/* Simulate the (main) behavor of DRAM.
 * Although this will lower the performace of NEMU, it makes
 * you clear about how DRAM perform read/write operations.
 * Note that cross addressing is not simulated.
 */

#define SET_WIDTH 12
#define ROW_WIDTH 4
#define COL_WIDTH 6
#define FLAG_WIDTH (27 - COL_WIDTH - SET_WIDTH)

typedef union {
	struct {
		uint32_t col	: COL_WIDTH;
		uint32_t set	: SET_WIDTH;
		uint32_t flag	: FLAG_WIDTH;
	};
	uint32_t addr;
} L2_cache_addr;


#define NR_COL (1 << COL_WIDTH)
#define NR_ROW (1 << ROW_WIDTH)
#define NR_SET (1 << SET_WIDTH)
#define NR_FLAG (1 << FLAG_WIDTH)

#define HW_MEM_SIZE (1 << 27)

typedef struct {
	uint8_t data[NR_ROW][NR_COL];
	uint32_t flag[NR_ROW];
	bool valid[NR_ROW];
	bool dirty[NR_ROW];
} L2_CACHE;

L2_CACHE L2_cache[NR_SET];

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

void init_L2_cache() {
	memset(L2_cache, 0, sizeof(L2_cache));
}

static uint32_t rand(uint32_t addr) {
	return (addr%1007);
}

static void writeback(uint32_t set, uint32_t row) {
	if (L2_cache[set].dirty[row]) {
		uint32_t i;
		hwaddr_t addrdram=(L2_cache[set].flag[row]<<(COL_WIDTH+SET_WIDTH))+(set<<(COL_WIDTH));
		for (i=0; i<NR_COL; i++) 
			dram_write((addrdram & ~(NR_COL-1))+i, 1, L2_cache[set].data[row][i]);
	}
}

static void write_L2_cache(uint32_t set, uint32_t row, uint32_t flag, uint32_t addr) {
	L2_cache[set].valid[row] = true;
	L2_cache[set].dirty[row] = false;
	L2_cache[set].flag[row] = flag;
	uint32_t i;
	for(i=0; i<NR_COL; i++) 
		L2_cache[set].data[row][i] = dram_read((addr & ~(NR_COL-1))+i, 1);
}

static void L2_cache_in_read(hwaddr_t addr, void *data) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	L2_cache_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t col = temp.col;
	uint32_t set = temp.set;
	uint32_t flag = temp.flag;
	bool full=true;
	bool find=false;
	uint32_t row, k;
	for (row=0; row<NR_ROW; row++) {
		if (L2_cache[set].valid[row]) {
			if (L2_cache[set].flag[row]==flag) {
				memcpy(data, L2_cache[set].data[row]+col, BURST_LEN);
				find=true;
				break;
			}
		}
		else {
			full=false;
			k=row;
		}
	}
	if (find==false) {
		if (full) { 
		/* Write the data back */
			k=rand(addr) & (NR_ROW - 1);
			writeback(set, k);
		}
		write_L2_cache(set, k, flag, addr);
		memcpy(data, L2_cache[set].data[k]+col, BURST_LEN);
	}
}

static void L2_cache_in_write(hwaddr_t addr, void *data, uint8_t *mask) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	L2_cache_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t col = temp.col;
	uint32_t set = temp.set;
	uint32_t flag = temp.flag;
	uint32_t row, k;
	bool find=false, full=true;
	for (row=0; row<NR_ROW; row++) {
		if (L2_cache[set].valid[row]) {
			if (L2_cache[set].flag[row]==flag) {
				memcpy_with_mask(L2_cache[set].data[row] + col, data, BURST_LEN, mask);
				find=true;
				L2_cache[set].dirty[row]=true;
				break;
			}
		}
		else {
			full=false;
			k=row;
		}
	}
	if (find==false) {
		if (full) {
		/* Write the data back */
			k=rand(addr) & (NR_ROW - 1);
			writeback(set, k);
		}
		write_L2_cache(set, k, flag, addr);
		memcpy_with_mask(L2_cache[set].data[k] + col, data, BURST_LEN, mask);
	}
}

uint32_t L2_cache_read(hwaddr_t addr, size_t len) {
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	
	L2_cache_in_read(addr, temp);
	
	if(offset + len > BURST_LEN) {
		/* data cross the burst boundary */	
		L2_cache_in_read(addr + BURST_LEN, temp + BURST_LEN);
	}
								
	return unalign_rw(temp + offset, 4);
}

void L2_cache_write(hwaddr_t addr, size_t len, uint32_t data) {
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	uint8_t mask[2 * BURST_LEN];
	memset(mask, 0, 2 * BURST_LEN);

	*(uint32_t *)(temp + offset) = data;
	memset(mask + offset, 1, len);
	
	L2_cache_in_write(addr, temp, mask);

	if(offset + len > BURST_LEN) {
		/* data cross the brust boundary */
		L2_cache_in_write(addr + BURST_LEN, temp + BURST_LEN, mask + BURST_LEN);
	}
}
