#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "cmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmp-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(cmp_i2a)		//0x3D
make_helper_v(cmp_i2rm)		//0x81 /7
make_helper_v(cmp_si2rm)	//0x83 /7
make_helper_v(cmp_r2rm)		//0x39
make_helper_v(cmp_rm2r)		//0x3B

