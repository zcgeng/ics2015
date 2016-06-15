#include "cpu/exec/template-start.h"

#define instr ja

make_helper(concat(ja_i_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("ja" str(SUFFIX) " %x", eip + imm + 1 + DATA_BYTE); 
	if (eflags.CF == 0 && eflags.ZF ==0) cpu.eip += imm;
	return DATA_BYTE + 1;
}
#undef instr

#define instr jae

make_helper(concat(jae_i_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("jae" str(SUFFIX) " %x", eip + imm + 1 + DATA_BYTE); 
	if (eflags.CF == 0) cpu.eip += imm;
	return DATA_BYTE + 1;
}
#undef instr

#define instr jb

make_helper(concat(jb_i_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("jb" str(SUFFIX) " %x", eip + imm + 1 + DATA_BYTE); 
	if (eflags.CF == 1) cpu.eip += imm;
	return DATA_BYTE + 1;
}
#undef instr

#define instr jbe

make_helper(concat(jbe_i_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("jbe" str(SUFFIX) " %x", eip + imm + 1 + DATA_BYTE); 
	if (eflags.CF == 1 || eflags.ZF == 1) cpu.eip += imm;
	return DATA_BYTE + 1;
}
#undef instr

#define instr je
make_helper(concat(je_i_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("je" str(SUFFIX) " %x", eip + imm + 1 + DATA_BYTE); 
	if (eflags.ZF == 1) cpu.eip += imm;
	return DATA_BYTE + 1;
}
#undef instr

#define instr jg
make_helper(concat(jg_i_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("jg" str(SUFFIX) " %x", eip + imm + 1 + DATA_BYTE); 
	if (eflags.ZF == 0 && eflags.SF == eflags.OF) cpu.eip += imm;
	return DATA_BYTE + 1;
}
#undef instr

#define instr jge
make_helper(concat(jge_i_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("jge" str(SUFFIX) " %x", eip + imm + 1 + DATA_BYTE); 
	if (eflags.SF == eflags.OF) cpu.eip += imm;
	return DATA_BYTE + 1;
}
#undef instr

#define instr jl
make_helper(concat(jl_i_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("jl" str(SUFFIX) " %x", eip + imm + 1 + DATA_BYTE); 
	if (eflags.SF != eflags.OF) cpu.eip += imm;
	return DATA_BYTE + 1;
}
#undef instr

#define instr jle
make_helper(concat(jle_i_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("jle" str(SUFFIX) " %x", eip + imm + 1 + DATA_BYTE); 
	if (eflags.ZF == 1 || eflags.SF != eflags.OF) cpu.eip += imm;
	return DATA_BYTE + 1;
}
#undef instr

#define instr jne
make_helper(concat(jne_i_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("jne" str(SUFFIX) " %x", eip + imm + 1 + DATA_BYTE); 
	if (eflags.ZF == 0) cpu.eip += imm;
	return DATA_BYTE + 1;
}
#undef instr

#define instr jno
make_helper(concat(jno_i_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("jno" str(SUFFIX) " %x", eip + imm + 1 + DATA_BYTE); 
	if (eflags.OF == 0) cpu.eip += imm;
	return DATA_BYTE + 1;
}
#undef instr

#define instr jnp
make_helper(concat(jnp_i_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("jnp" str(SUFFIX) " %x", eip + imm + 1 + DATA_BYTE); 
	if (eflags.PF == 0) cpu.eip += imm;
	return DATA_BYTE + 1;
}
#undef instr

#define instr jns
make_helper(concat(jns_i_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("jns" str(SUFFIX) " %x", eip + imm + 1 + DATA_BYTE); 
	if (eflags.SF == 0) cpu.eip += imm;
	return DATA_BYTE + 1;
}
#undef instr

#define instr jo
make_helper(concat(jo_i_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("jo" str(SUFFIX) " %x", eip + imm + 1 + DATA_BYTE); 
	if (eflags.OF == 1) cpu.eip += imm;
	return DATA_BYTE + 1;
}
#undef instr

#define instr jp
make_helper(concat(jp_i_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("jp" str(SUFFIX) " %x", eip + imm + 1 + DATA_BYTE); 
	if (eflags.PF == 1) cpu.eip += imm;
	return DATA_BYTE + 1;
}
#undef instr

#define instr s
make_helper(concat(js_i_, SUFFIX)) {
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	print_asm("js" str(SUFFIX) " %x", eip + imm + 1 + DATA_BYTE); 
	if (eflags.SF == 1) cpu.eip += imm;
	return DATA_BYTE + 1;
}
#undef instr

#include "cpu/exec/template-end.h"

/*#define DATA_BYTE 1
#include "jcc-template-a.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-template-a.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-template-a.h"
#undef DATA_BYTE

make_helper_v(concat(j, CC));

#undef COND
#undef CC
*/
