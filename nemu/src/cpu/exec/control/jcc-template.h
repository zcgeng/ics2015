#include "cpu/exec/template-start.h"

#define update_eip()\
	int32_t val = op_src->val;\
	val = val << (32 - DATA_BYTE * 8);\
	val = val >> (32 - DATA_BYTE * 8);\
	cpu.eip += val;\
	if(DATA_BYTE == 2) cpu.eip &= 0xffff;

#if DATA_BYTE == 1
#define CODE_LEN 2
#endif
#if DATA_BYTE == 2
#define CODE_LEN 4
#endif
#if DATA_BYTE == 4
#define CODE_LEN 6
#endif


/*---------------------------------------------*/
#define instr jo

static void do_execute() {
	if(cpu.OF == 1) {update_eip();}
	print_asm("jo $0x%x", cpu.eip + CODE_LEN);
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jno

static void do_execute() {
	if(cpu.OF == 0) {update_eip();}
	print_asm("jno $0x%x", cpu.eip + CODE_LEN);
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jb

static void do_execute() {
	if(cpu.CF == 1) {update_eip();}
	print_asm("jb $0x%x", cpu.eip + CODE_LEN);
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jae

static void do_execute() {
	if(cpu.OF != 0) {update_eip();}
	print_asm("jae $0x%x", cpu.eip + CODE_LEN);
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr je

static void do_execute() {
	if(cpu.ZF == 1) {update_eip();}
	print_asm("je $0x%x", cpu.eip + CODE_LEN);
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jne

static void do_execute() {
	if(cpu.ZF == 0) {update_eip();}
	print_asm("jne $0x%x", cpu.eip + CODE_LEN);
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jbe

static void do_execute() {
	if(cpu.CF == 1 || cpu.ZF == 1) {update_eip();}
	print_asm("jbe $0x%x", cpu.eip + CODE_LEN);
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr ja

static void do_execute() {
	if(cpu.CF == 0 && cpu.ZF == 0) {update_eip();}
	print_asm("ja $0x%x", cpu.eip + CODE_LEN);
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr js

static void do_execute() {
	if(cpu.SF == 1) {update_eip();}
	print_asm("js $0x%x", cpu.eip + CODE_LEN);
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jns

static void do_execute() {
	if(cpu.SF == 0) {update_eip();}
	print_asm("jns $0x%x", cpu.eip + CODE_LEN);
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jp

static void do_execute() {
	if(cpu.PF == 1) {update_eip();}
	print_asm("jp $0x%x", cpu.eip + CODE_LEN);
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jnp

static void do_execute() {
	if(cpu.PF == 0) {update_eip();}
	print_asm("jnp $0x%x", cpu.eip + CODE_LEN);
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jl

static void do_execute() {
	if(cpu.SF != cpu.OF) {update_eip();}
	print_asm("jl $0x%x", cpu.eip + CODE_LEN);
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jge

static void do_execute() {
	if(cpu.SF == cpu.OF) {update_eip();}
	print_asm("jge $0x%x", cpu.eip + CODE_LEN);
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jle

static void do_execute() {
	if(cpu.ZF == 1 || cpu.SF != cpu.OF) {update_eip();}
	print_asm("jle $0x%x", cpu.eip + CODE_LEN);
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jg

static void do_execute() {
	if(cpu.ZF == 0 && cpu.SF == cpu.OF) {update_eip();}
	print_asm("jg $0x%x", cpu.eip + CODE_LEN);
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jcxz

static void do_execute() {
	if(cpu.ecx == 0) {update_eip();}
	print_asm("jcxz $0x%x", cpu.eip + CODE_LEN);
}

make_instr_helper(i)
#undef instr
/*---------------------------------------------*/

#undef CODE_LEN
#include "cpu/exec/template-end.h"
