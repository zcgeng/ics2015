#include "cpu/exec/template-start.h"

#define instr movs

static void do_execute() {
	op_src->type = op_dest->type = OP_TYPE_REG;
	op_src->reg = R_ESI; 
	op_dest->reg = R_EDI;
	snprintf(op_src->str, OP_STR_SIZE, "%%%s", REG_NAME(R_ESI));
	snprintf(op_dest->str, 11, "%%es:(%%edi)");
	MEM_W(cpu.edi, MEM_R(cpu.esi, R_DS), R_ES);
	//swaddr_write(cpu.edi, DATA_BYTE, (DATA_TYPE) cpu.eax);
	cpu.edi += eflags.DF == 0? +DATA_BYTE : -DATA_BYTE;
	cpu.esi += eflags.DF == 0? +DATA_BYTE : -DATA_BYTE;
//	print_asm("stos" str(SUFFIX) "%%%s, /%\/%\es:(/%\/%\edi)", REG_NAME(R_EAX));
	print_asm_template2();
}

make_instr_helper(m)

#include "cpu/exec/template-end.h"
