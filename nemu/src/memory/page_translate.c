#include "common.h"
#include "cpu/reg.h"
#include "../../lib-common/x86-inc/mmu.h"
#include <stdio.h>

uint32_t hwaddr_read(lnaddr_t addr, size_t len);
typedef union {
	struct {
		uint32_t offset: 12;
		uint32_t page  : 10;
		uint32_t dir   : 10;
	};
	uint32_t val;
} lnaddrdevide;
typedef struct {
	uint32_t data;
	uint32_t flag;
	bool vaild;
} TLB;
TLB tlb[64];

void init_tlb() {
	memset(tlb, 0, sizeof(tlb));
}

hwaddr_t page_translate(lnaddr_t addr) {
	if (!(cpu.cr0.protect_enable == 1 && cpu.cr0.paging == 1)) return addr;
	lnaddrdevide lnaddr;
	lnaddr.val=addr;
	int set;
	for (set=0; set<64; set++) 
		if (tlb[set].vaild && tlb[set].flag==((lnaddr.dir<<10)+lnaddr.page)) {
			return (tlb[set].data<<12)+lnaddr.offset;
		}
	
//	printf("%x\n", cpu.cr3.page_directory_base);
	PDE dir;
	dir.val=hwaddr_read((cpu.cr3.page_directory_base<<12)+lnaddr.dir*4, 4);
//	if (dir.val!=1208327) 
//	printf("%d\n", dir.val);
	if (dir.present==0) panic("the page is empty addr=%x\n", addr);
	PTE page;
	page.val=hwaddr_read((dir.page_frame<<12)+lnaddr.page*4, 4);
	assert(page.present==1);
	bool find=false;
	for (set=0; set<64; set++) 
		if (tlb[set].vaild==false) {
			tlb[set].vaild=true;
			tlb[set].data=page.page_frame;
			tlb[set].flag=(lnaddr.dir<<10)+lnaddr.page;
			find=true;
		}
	if (find==false) {
		set=addr%0x3f;
		tlb[set].vaild=true;
		tlb[set].flag=(lnaddr.dir<<10)+lnaddr.page;
		tlb[set].data=page.page_frame;
	}
	return (page.page_frame<<12)+lnaddr.offset;
}
