#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "sub-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "sub-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "sub-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(sub_i2a)		//0x2D
make_helper_v(sub_i2rm)		//0x81 /5
make_helper_v(sub_si2rm)	//0x83 /5
make_helper_v(sub_r2rm)		//0x29
make_helper_v(sub_rm2r)		//0x2B

