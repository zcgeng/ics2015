/*************************************************************************
	> File Name: cache.c
	> Author: zcgeng
	> Mail: zcg1996@qq.com
	> Created Time: 2016年04月05日 星期二 14时42分24秒
 ************************************************************************/

#include "common.h"
#include "stdlib.h"
#include "time.h"
#define CACHE_SIZE (64 << 10) 
#define BLOCK_SIZE 64
#define WAY_NUM 8
#define BLOCK_NUM (CACHE_SIZE / BLOCK_SIZE) 
#define LINES_PER_GROUP (BLOCK_NUM / WAY_NUM)
#define ADDRESS_BITS 32 
#define BLOCK_OFFSET_BITS 6
#define CACHE_INDEX_BITS 7
#define CACHE_TAG_BITS (ADDRESS_BITS - BLOCK_OFFSET_BITS - CACHE_INDEX_BITS)
int32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

typedef union{
    struct{
        uint32_t tag : CACHE_TAG_BITS;
        uint32_t index : CACHE_INDEX_BITS;
        uint32_t block_offset : BLOCK_OFFSET_BITS;
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

cache_block cache[WAY_NUM][LINES_PER_GROUP];

void init_cache(){
    int i, j;
    for(i = 0; i < WAY_NUM; ++i)
        for(j = 0; j < LINES_PER_GROUP; ++j)
            cache[i][j].valid = 0;
}

uint32_t cache_read(hwaddr_t addr, size_t len){
    int i;
    cache_addr caddr;
    caddr.addr = addr;
    for(i = 0; i < LINES_PER_GROUP; ++i){
        if(cache[caddr.index][i].valid == 1 && cache[caddr.index][i].tag == caddr.tag){
            // hit: read the cache
            if(len + caddr.block_offset <= BLOCK_SIZE){
                uint32_t tmp;
                memcpy(&tmp, &cache[caddr.index][i].block[caddr.block_offset], len);
                return tmp;
            }
            else{
                uint32_t tmp;
                uint32_t len2 = len + caddr.block_offset - BLOCK_SIZE;
                uint32_t len1 = len - len2;
                switch(len1){
                    case 1:
                        memcpy(&tmp, &cache[caddr.index][i].block[caddr.block_offset], 1);
                        unalign_rw(&tmp + 1, 3) = cache_read(addr + 1, 3); //the second time to read the cache
                    break;
                    case 2:
                        memcpy(&tmp, &cache[caddr.index][i].block[caddr.block_offset], 2);
                        unalign_rw(&tmp + 2, 2) = cache_read(addr + 2, 2); 
                    break;
                    case 3:
                        memcpy(&tmp, &cache[caddr.index][i].block[caddr.block_offset], 3);
                        unalign_rw(&tmp + 3, 1) = cache_read(addr + 3, 1); 
                    break;
                }
                return tmp;
            }
        }
    }
    // miss: find the block to replace
    Log("miss\n");
    for(i = 0; i < LINES_PER_GROUP; ++i)
        if(cache[caddr.index][i].valid == 0) break;
    if( i == LINES_PER_GROUP ){
        srand(time(0));
        i = rand() % LINES_PER_GROUP;
    }
    // replace
    cache[caddr.index][i].tag = caddr.tag;
    cache[caddr.index][i].valid = 1;
    cache_addr tmpaddr = caddr;
    tmpaddr.block_offset = 0;
    int j = 0;
    for(j = 0; j < BLOCK_SIZE; ++j){
        cache[caddr.index][i].block[j] = dram_read(tmpaddr.addr + j, 1); 
    }
    return cache_read(addr, len);
}

void cache_write(hwaddr_t addr, size_t len, uint32_t data){
    cache_addr caddr;
    caddr.addr = addr;
    int i;
    for(i = 0; i < LINES_PER_GROUP; ++i){
        if(cache[caddr.index][i].valid == 1 && cache[caddr.index][i].tag == caddr.tag){
            if(caddr.block_offset + len <= BLOCK_SIZE)
                memcpy(&cache[caddr.index][i].block[caddr.block_offset], &data, len);
            else{
                uint32_t len2 = caddr.block_offset + len - BLOCK_SIZE;
                uint32_t len1 = len - len2;
                switch(len1){
                    case 1:
                        memcpy(&cache[caddr.index][i].block[caddr.block_offset], &unalign_rw(&data, 1), 1);
                        cache_write(addr + 1, len2, unalign_rw(&data + 1, 3));
                    break;
                    case 2:
                        memcpy(&cache[caddr.index][i].block[caddr.block_offset], &unalign_rw(&data, 2), 2);
                        cache_write(addr + 2, len2, unalign_rw(&data + 2, 2));
                    break;
                    case 3:
                        memcpy(&cache[caddr.index][i].block[caddr.block_offset], &unalign_rw(&data, 1), 1);
                        cache_write(addr + 3, len2, unalign_rw(&data + 3, 1));
                    break;
                }
            }
        }
    }
    dram_write(addr, len, data);
}

void cache_debug(hwaddr_t addr){
#define CACHE_SIZE (64 << 10) 
#define BLOCK_SIZE 64
#define WAY_NUM 8
#define BLOCK_NUM (CACHE_SIZE / BLOCK_SIZE) 
#define LINES_PER_GROUP (BLOCK_NUM / WAY_NUM)
#define ADDRESS_BITS 32 
#define BLOCK_OFFSET_BITS 6
#define CACHE_INDEX_BITS 7
#define CACHE_TAG_BITS (ADDRESS_BITS - BLOCK_OFFSET_BITS - CACHE_INDEX_BITS)
    printf("%x %x\n", CACHE_SIZE, CACHE_TAG_BITS);
    int i;
    cache_addr caddr;
    caddr.addr = addr;
    printf("addr = 0x%08x, tag = 0x%x, index = 0x%x, offset = 0x%x\n", addr, caddr.tag, caddr.index, caddr.block_offset);
    for(i = 0; i < LINES_PER_GROUP; ++i){
        if(cache[caddr.index][i].valid == 1 && cache[caddr.index][i].tag == caddr.tag){
            printf("hit at cache[index=0x%x][i=0x%x]\n", caddr.index, i);
            // hit: read the cache
            uint32_t tmp;
            memcpy(&tmp, &cache[caddr.index][i].block[caddr.block_offset], 4);
            printf("value at address 0x%08x is 0x%08x\n", addr, tmp);
            cache_addr tmpaddr = caddr;
            tmpaddr.block_offset = 0;
            printf("cache block: begin at address %08x\n", tmpaddr.addr);
            int j = 0;
            for(j = 0; j < BLOCK_SIZE; ++j){
                printf("%02x ", cache[caddr.index][i].block[j]);
                if(j % 8 == 7) printf("\n");
            }
            printf("\n");
            return;
        }
    }
    printf("missed! the No.0x%x group of cache:(only show valid blocks)\n", caddr.index);
    for(i = 0; i < LINES_PER_GROUP; ++i){
        if(cache[caddr.index][i].valid)
            printf("cache[0x%x][0x%x]: valid:%d, tag:0x%x\n", caddr.index, i,cache[caddr.index][i].valid,cache[caddr.index][i].tag);
    }
}
