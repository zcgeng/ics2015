#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {
    uint32_t srcval = op_src->val;
    srcval <<= 32 - DATA_BYTE * 8;
    srcval >>= 32 - DATA_BYTE * 8;
    DATA_TYPE result = op_dest->val - srcval;
    update_EFLAGS_PZS(result);
    //CF: 借位
    cpu.CF = (op_dest->val > srcval) ? 0 : 1;
    //AF: 低三位是否借位
    cpu.AF = (op_dest->val & 0x8) > (srcval & 0x8) ? 0 : 1;
    //OF: overflow flag, 同号相减不会溢出;异号相减, 结果与被减数反号则溢出.
    if( MSB(op_dest->val) != MSB(srcval) && MSB(result) != MSB(op_dest->val)) 
        cpu.OF = 1; 
    else cpu.OF = 0;
	print_asm_template2();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(i2a)
make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)



#include "cpu/exec/template-end.h"
