#include "cpu/exec/template-start.h"

#define instr sbb

static void do_execute() {
    DATA_TYPE result = op_dest->val - op_src->val - cpu.CF;
    update_EFLAGS_PZS(result);
    //CF: 借位
    cpu.CF = (op_dest->val > op_src->val + cpu.CF) ? 0 : 1;
    //AF: 低三位是否借位
    cpu.AF = (op_dest->val & 0x7) > (op_src->val & 0x7) + cpu.CF ? 0 : 1;
    //OF: overflow flag, 同号相减不会溢出;异号相减, 结果与被减数反号则溢出.
    if( MSB(op_dest->val) != MSB(op_src->val) && MSB(result) != MSB(op_dest->val)) 
        cpu.OF = 1; 
    else cpu.OF = 0;
    OPERAND_W(op_dest, result); //先判断EFLAGS变化, 最后再改变op_dest的值
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
