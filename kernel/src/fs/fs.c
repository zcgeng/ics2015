#include "common.h"

typedef struct {
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;
typedef struct {
	bool opened;
	uint32_t offset;
} Fstate;

enum {SEEK_SET, SEEK_CUR, SEEK_END};

/* This is the information about all files in disk. */
static const file_info file_table[] = {
	{"1.rpg", 188864, 1048576}, {"2.rpg", 188864, 1237440},
	{"3.rpg", 188864, 1426304}, {"4.rpg", 188864, 1615168},
	{"5.rpg", 188864, 1804032}, {"abc.mkf", 1022564, 1992896},
	{"ball.mkf", 134704, 3015460}, {"data.mkf", 66418, 3150164},
	{"desc.dat", 16027, 3216582}, {"fbp.mkf", 1128064, 3232609},
	{"fire.mkf", 834728, 4360673}, {"f.mkf", 186966, 5195401},
	{"gop.mkf", 11530322, 5382367}, {"map.mkf", 1496578, 16912689},
	{"mgo.mkf", 1577442, 18409267}, {"m.msg", 188232, 19986709},
	{"mus.mkf", 331284, 20174941}, {"pat.mkf", 8488, 20506225},
	{"rgm.mkf", 453202, 20514713}, {"rng.mkf", 4546074, 20967915},
	{"sss.mkf", 557004, 25513989}, {"voc.mkf", 1997044, 26070993},
	{"wor16.asc", 5374, 28068037}, {"wor16.fon", 82306, 28073411},
	{"word.dat", 5650, 28155717},
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

static Fstate fPool[NR_FILES + 3];

void ide_read(uint8_t *, uint32_t, uint32_t);
void ide_write(uint8_t *, uint32_t, uint32_t);

/* TODO: implement a simplified file system here. */
bool equal(const char * a , char *b){
	int i = 0;
	while(a[i]){
		if(a[i] != *b)
		  return false;
		i++;
		b++;
	}
	if(*b)return false;
	return true;
}
int fs_open(const char *pathname  , int flags) {
	int i ; 
	int fd;
	for(i = 0 ; i < NR_FILES ; i++) {
		if(equal(pathname , file_table[i].name)){
			fd = i + 3;
			fPool[fd].opened = true;
			fPool[fd].offset = 0;
			return fd;
		}
	}
	Log("%s",pathname);
	assert(0);
	return -1; 
}

int fs_read(int fd , void *buf , int len) {
	int file_poi = fd - 3;
	if(fPool[fd].offset + len > file_table[file_poi].size){
		len = file_table[file_poi].size - fPool[fd].offset;
	}
	if(!fPool[fd].opened)return 0;
	ide_read(buf,file_table[file_poi].disk_offset + fPool[fd].offset , len);
	fPool[fd].offset += len ;
	return len;
}

int fs_write(int fd , void *buf , int len) {
	int fPoi = fd - 3 ;
	if(!fPool[fd].opened)return 0;
	if(fPool[fd].offset + len > file_table[fPoi].size)
		len = file_table[fPoi].size - fPool[fd].offset;
	ide_write(buf , file_table[fPoi].disk_offset + fPool[fd].offset , len);
	fPool[fd].offset += len ;
	return len ;
}

int fs_lseek(int fd , int offset , int whence) {
	if(!fPool[fd].opened)return -1; 
	switch(whence) {
		case SEEK_SET: fPool[fd].offset = offset;break;
		case SEEK_CUR: fPool[fd].offset += offset;break;
		case SEEK_END: fPool[fd].offset = file_table[fd - 3].size + offset;break;
	}
	if(fPool[fd].offset > file_table[fd-3].size){
		fPool[fd].offset = file_table[fd - 3].size;
	}
	return fPool[fd].offset;
}

int fs_close(int fd){
	fPool[fd].opened = false;
	return 0;
}

