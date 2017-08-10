#include "cpu/exec/template-start.h"

#if DATA_BYTE == 1
#define CODE_LEN 2
#endif
#if DATA_BYTE == 2
#define CODE_LEN 4
#endif
#if DATA_BYTE == 4
#define CODE_LEN 6
#endif

#define get_new_eip()\
	int32_t val = op_src->val;\
	val = val << (32 - DATA_BYTE * 8);\
	val = val >> (32 - DATA_BYTE * 8);\
	uint32_t new_eip = cpu.eip + val;\
	if(DATA_BYTE == 2) new_eip &= 0xffff;\
	print_asm(str(instr) " $0x%x", new_eip + CODE_LEN);

/*---------------------------------------------*/
#define instr jo

static void do_execute() {
	get_new_eip();
	if(cpu.OF == 1) cpu.eip = new_eip;
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jno

static void do_execute() {
	get_new_eip();
	if(cpu.OF == 0) cpu.eip = new_eip;
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jb

static void do_execute() {
	get_new_eip();
	if(cpu.CF == 1) cpu.eip = new_eip;
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jae

static void do_execute() {
	get_new_eip();
	if(cpu.CF == 0) cpu.eip = new_eip;
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr je

static void do_execute() {
	get_new_eip();
	if(cpu.ZF == 1) cpu.eip = new_eip;
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jne

static void do_execute() {
	get_new_eip();
	if(cpu.ZF == 0) cpu.eip = new_eip;
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jbe

static void do_execute() {
	get_new_eip();
	if(cpu.CF == 1 || cpu.ZF == 1) cpu.eip = new_eip;
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr ja

static void do_execute() {
	get_new_eip();
	if(cpu.CF == 0 && cpu.ZF == 0) cpu.eip = new_eip;
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr js

static void do_execute() {
	get_new_eip();
	if(cpu.SF == 1) cpu.eip = new_eip;
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jns

static void do_execute() {
	get_new_eip();
	if(cpu.SF == 0) cpu.eip = new_eip;
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jp

static void do_execute() {
	get_new_eip();
	if(cpu.PF == 1) cpu.eip = new_eip;
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jnp

static void do_execute() {
	get_new_eip();
	if(cpu.PF == 0) cpu.eip = new_eip;
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jl

static void do_execute() {
	get_new_eip();
	if(cpu.SF != cpu.OF) cpu.eip = new_eip;
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jge

static void do_execute() {
	get_new_eip();
	if(cpu.SF == cpu.OF) cpu.eip = new_eip;
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jle

static void do_execute() {
	get_new_eip();
	if(cpu.ZF == 1 || cpu.SF != cpu.OF) cpu.eip = new_eip;
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jg

static void do_execute() {
	get_new_eip();
	if(cpu.ZF == 0 && cpu.SF == cpu.OF) cpu.eip = new_eip;
}

make_instr_helper(i)

#undef instr
/*---------------------------------------------*/
#define instr jcxz

static void do_execute() {
	get_new_eip();
	if(cpu.ecx == 0) cpu.eip = new_eip;
}

make_instr_helper(i)
#undef instr
/*---------------------------------------------*/

#undef CODE_LEN
#include "cpu/exec/template-end.h"
