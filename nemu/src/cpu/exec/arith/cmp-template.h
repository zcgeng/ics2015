#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute() {
    DATA_TYPE result = op_dest->val - op_src->val;
    eflags.PF = 1; 
    int i; 
    for(i = 0; i < 8; ++i) eflags.PF ^= (result & 0xff) >> i; 
    eflags.ZF = !result; 
    eflags.SF = MSB(result);
    //CF: 借位
    eflags.CF = (((long long)op_dest->val - (long long)op_src->val) >> (8 * DATA_BYTE)) & 1;
    //AF: 低三位是否借位
    eflags.AF = (op_dest->val & 0x8) > (op_src->val & 0x8) ? 0 : 1;
    //OF: overflow flag, 同号相减不会溢出;异号相减, 结果与被减数反号则溢出.
    if( MSB(op_dest->val) != MSB(op_src->val) && MSB(result) != MSB(op_dest->val)) 
        eflags.OF = 1; 
    else eflags.OF = 0;
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
