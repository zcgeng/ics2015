#include "cpu/exec/template-start.h"

#define instr call

static void do_execute() {
	cpu.esp -= 4;
	if(op_src->type == OP_TYPE_IMM){
		swaddr_write(cpu.esp, 4, cpu.eip + DATA_BYTE + 1, R_SS);// PUSH 返回地址
		cpu.eip += op_src->val;
		if(DATA_BYTE == 2) cpu.eip &= 0x0000ffff;
		op_src->val += DATA_BYTE + 1;
   		print_asm_template1();
	}
	else{
		//Log("1now eip = 0x%x, esp = 0x%x\n", cpu.eip, cpu.esp);
		swaddr_write(cpu.esp, 4, cpu.eip + 2, R_SS);
		cpu.eip = (op_src->val) - 2;
		//Log("2now eip = 0x%x\n", cpu.eip);
		print_asm_template1();
	}	

	//print_asm_template1();
}


make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
