#ifndef __ADD_H__
#define __ADD_H__

make_helper(add_i2a_b);		//0x2C
make_helper(add_i2rm_b);	//0x80 /5
make_helper(add_r2rm_b);	//0x28
make_helper(add_rm2r_b);	//0x2A

make_helper(add_i2a_v);		//0x2D
make_helper(add_i2rm_v);	//0x81 /5
make_helper(add_si2rm_v);	//0x83 /5
make_helper(add_r2rm_v);	//0x29
make_helper(add_rm2r_v);	//0x2B

#endif
