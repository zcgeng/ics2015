/*************************************************************************
	> File Name: cache.c
	> Author: zcgeng
	> Mail: zcg1996@qq.com
	> Created Time: 2016年04月05日 星期二 14时42分24秒
 ************************************************************************/

#include "common.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#define ADDRESS_BITS 32 

#define BLOCK_OFFSET_BITS 6
#define CACHE_SIZE (64 << 10) 
#define WAY_NUM 8
#define CACHE_INDEX_BITS 7 //log(2, GROUP_NUM)


#define BLOCK_SIZE (1 << BLOCK_OFFSET_BITS)
#define BLOCK_NUM (CACHE_SIZE / BLOCK_SIZE) 
#define GROUP_NUM (BLOCK_NUM / WAY_NUM)
#define CACHE_TAG_BITS (ADDRESS_BITS - BLOCK_OFFSET_BITS - CACHE_INDEX_BITS)
int32_t cache2_read(hwaddr_t, size_t);
void cache2_write(hwaddr_t, size_t, uint32_t);

typedef union{
    struct{
        uint32_t block_offset : BLOCK_OFFSET_BITS;
        uint32_t index : CACHE_INDEX_BITS;
        uint32_t tag : CACHE_TAG_BITS;
    };
    uint32_t addr;
}cache_addr;

typedef struct{
    struct{
        uint32_t valid : 1;
        uint32_t tag : CACHE_TAG_BITS;
    };
    uint8_t block[BLOCK_SIZE];
}cache_block;

cache_block cache[GROUP_NUM][WAY_NUM];

void init_cache(){
    Assert((1 << CACHE_INDEX_BITS) == GROUP_NUM, "wrong cache_addr structure! CACHE_INDEX_BITS should be %d\n", (int)log2(GROUP_NUM));
    int i, j;
    for(i = 0; i < GROUP_NUM; ++i)
        for(j = 0; j < WAY_NUM; ++j)
            cache[i][j].valid = 0;
}

uint32_t cache_read(hwaddr_t addr, size_t len){
    int i;
    cache_addr caddr;
    caddr.addr = addr;
    for(i = 0; i < WAY_NUM; ++i){
        if(cache[caddr.index][i].valid == 1 && cache[caddr.index][i].tag == caddr.tag){
            // hit: read the cache
            if(len + caddr.block_offset <= BLOCK_SIZE){
                uint32_t tmp;
                memcpy(&tmp, &cache[caddr.index][i].block[caddr.block_offset], len);
                return tmp;
            }
            else{
		//Log("I am unaligned\n");
                uint32_t high, low;
                uint32_t len2 = len + caddr.block_offset - BLOCK_SIZE;
                uint32_t len1 = len - len2;
                memcpy(&low, &cache[caddr.index][i].block[caddr.block_offset], 4);
                high = cache_read(addr + len1, 4) << (len1*8);
		//Log("high = %x, low = %x", high, low);
                switch(len1){
                    case 1:
                        unalign_rw(&high, 1) = low;
                    break;
                    case 2:
                        unalign_rw(&high, 2) = low;
                    break;
                    case 3:
                        unalign_rw(&high, 3) = low;
                    break;
                }
                return high;
            }
        }
    }
    // miss: find the block to replace
    //Log("miss\n");
    for(i = 0; i < WAY_NUM; ++i)
        if(cache[caddr.index][i].valid == 0) break;
    if( i == WAY_NUM ){
        srand(time(0));
        i = rand() % WAY_NUM;
    }
    // replace
    cache[caddr.index][i].tag = caddr.tag;
    cache[caddr.index][i].valid = 1;
    cache_addr tmpaddr = caddr;
    tmpaddr.block_offset = 0;
    int j = 0;
    for(j = 0; j < BLOCK_SIZE; ++j){
        cache[caddr.index][i].block[j] = cache2_read(tmpaddr.addr + j, 1); 
    }
    return cache_read(addr, len);
}

void cache_write(hwaddr_t addr, size_t len, uint32_t data){
    cache_addr caddr;
    caddr.addr = addr;
    int i;
    for(i = 0; i < WAY_NUM; ++i){
        if(cache[caddr.index][i].valid == 1 && cache[caddr.index][i].tag == caddr.tag){
            if(caddr.block_offset + len <= BLOCK_SIZE)
                memcpy(&cache[caddr.index][i].block[caddr.block_offset], &data, len);
            else{
		//Log("I am unaligned!(cache write)\n");
                uint32_t len2 = caddr.block_offset + len - BLOCK_SIZE;
                uint32_t len1 = len - len2;
		int j = 0;
                for(j = 0; j < len1; ++j){
		    uint32_t tmp = data << (j*8);
		    cache[caddr.index][i].block[caddr.block_offset+j] = unalign_rw(&tmp, 1);
		}
		cache_write(addr + len1, len2, data>>(len1*8));
            }
	    break;
        }
    }
    cache2_write(addr, len, data);
}

void cache_debug(hwaddr_t addr){
    int i;
    cache_addr caddr;
    caddr.addr = addr;
    printf("addr = 0x%08x, tag = 0x%x, index = 0x%x, offset = 0x%x\n", addr, caddr.tag, caddr.index, caddr.block_offset);
    for(i = 0; i < WAY_NUM; ++i){
        if(cache[caddr.index][i].valid == 1 && cache[caddr.index][i].tag == caddr.tag){
            printf("hit at cache[index=0x%x][i=0x%x], block tag:%x\n", caddr.index, i, cache[caddr.index][i].tag);
            // hit: read the cache
            uint8_t tmp;
            memcpy(&tmp, &cache[caddr.index][i].block[caddr.block_offset], 1);
            printf("value at address 0x%08x is 0x%02x\n", addr, tmp);
            cache_addr tmpaddr = caddr;
            tmpaddr.block_offset = 0;
            printf("cache block: begin at address %08x\n", tmpaddr.addr);
            int j = 0;
            for(j = 0; j < BLOCK_SIZE; ++j){
                printf("%02x ", cache[caddr.index][i].block[j]);
                if(j % 16 == 15) printf("\n");
            }
            printf("\n");
            return;
        }
    }
    printf("missed! the No.0x%x group of cache:(only show valid blocks)\n", caddr.index);
    for(i = 0; i < WAY_NUM; ++i){
        if(cache[caddr.index][i].valid)
            printf("cache[0x%x][0x%x]: valid:%d, tag:0x%x\n", caddr.index, i,cache[caddr.index][i].valid,cache[caddr.index][i].tag);
    }
}
#undef ADDRESS_BITS
#undef BLOCK_OFFSET_BITS
#undef CACHE_SIZE
#undef WAY_NUM
#undef CACHE_INDEX_BITS
#undef BLOCK_SIZE
#undef BLOCK_NUM
#undef GROUP_NU
#undef CACHE_TAG_BITS
