/*************************************************************************
	> File Name: cache.c
	> Author: zcgeng
	> Mail: zcg1996@qq.com
	> Created Time: 2016年04月05日 星期二 14时42分24秒
 ************************************************************************/

#include "common.h"
#include "stdlib.h"
#define CACHE_SIZE (64 << 10) 
#define BLOCK_SIZE 64
#define WAY_NUM 8
#define BLOCK_NUM (CACHE_SIZE / BLOCK_SIZE) 
#define LINES_PER_GROUP (BLOCK_NUM / WAY_NUM)
#define ADDRESS_BITS 32 
#define BLOCK_OFFSET_BITS 6
#define CACHE_INDEX_BITS 7
#define CACHE_TAG_BITS (ADDRESS_BITS - BLOCK_OFFSET_BITS - CACHE_INDEX_BITS)
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
    cache_addr caddr = addr;
    for(i = 0; i < LINES_PER_GROUP; ++i){
        if(cache[caddr.index][i].valid = 1 && cache[caddr.index][i].tag == caddr.tag){
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
                memcpy(&unalign_rw(&tmp, len1), &cache[caddr.index][i].block[caddr.block_offset], len1);
                unalign_rw(&tmp + len1, len2) = cache_read(addr + len1, len2); //the second time to read the cache
                return tmp;
            }
        }
    }
    // miss: find the block to replace
    for(i = 0; i < LINES_PER_GROUP; ++i)
        if(cache[caddr,index][i].valid == 0) break;
    if( i == LINES_PER_GROUP ){
        srand(time(0));
        i = rand() % LINES_PER_GROUP;
    }
    // replace
    cache[caddr.index][i].tag = caddr.tag;
    cache[caddr.index][i].valid = 1;
    cache_addr tmpaddr = caddr;
    tmpaddr.offset = 0;
    int j = 0;
    for(j = 0; j < BLOCK_SIZE; ++j){
        cache[caddr.index][i].block[j] = dram_read(tmpaddr + j, 1); 
    }
    return cache_read(addr, len);
}

void cache_write(hwaddr_t addr, size_t len, uint32_t data){
    cache_addr caddr = addr;
    int i;
    for(i = 0; i < LINES_PER_GROUP; ++i){
        if(cache[caddr.index][i].valid == 1 && cache[caddr.index][i].tag == caddr.tag){
            if(caddr.block_offset + len <= BLOCK_SIZE)
                memcpy(&cache[caddr.index][i].block[caddr.block_offset], &data, len);
            else{
                uint32_t tmp;
                uint32_t len2 = caddr.block_offset + len - BLOCK_SIZE;
                uint32_t len1 = len - len2;
                memcpy(&cache[caddr.index][i].block[caddr.block_offset], &unalign_rw(&data, len1), len1);
                cache_write(addr + len1, len2, &unalign_rw(&data + len1, len2));
            }
        }
    }
    dram_write(addr, len, data);
}
