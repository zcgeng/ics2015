#include "cpu/exec/template-start.h"

/*---------------------------------------------*/
#define instr seto

static void do_execute() {
	if(eflags.OF == 1) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)

#undef instr
/*---------------------------------------------*/
#define instr setno

static void do_execute() {
	if(eflags.OF == 0) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)

#undef instr
/*---------------------------------------------*/
#define instr setb

static void do_execute() {
	if(eflags.CF == 1) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)

#undef instr
/*---------------------------------------------*/
#define instr setae

static void do_execute() {
	if(eflags.CF == 0) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)

#undef instr
/*---------------------------------------------*/
#define instr sete

static void do_execute() {
	if(eflags.ZF == 1) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)

#undef instr
/*---------------------------------------------*/
#define instr setne

static void do_execute() {
	if(eflags.ZF == 0) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)

#undef instr
/*---------------------------------------------*/
#define instr setbe

static void do_execute() {
	if(eflags.CF == 1 || eflags.ZF == 1) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)

#undef instr
/*---------------------------------------------*/
#define instr seta

static void do_execute() {
	if(eflags.CF == 0 && eflags.ZF == 0) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)

#undef instr
/*---------------------------------------------*/
#define instr sets

static void do_execute() {
	if(eflags.SF == 1) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)

#undef instr
/*---------------------------------------------*/
#define instr setns

static void do_execute() {
	if(eflags.SF == 0) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)

#undef instr
/*---------------------------------------------*/
#define instr setp

static void do_execute() {
	if(eflags.PF == 1) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)

#undef instr
/*---------------------------------------------*/
#define instr setnp

static void do_execute() {
	if(eflags.PF == 0) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)

#undef instr
/*---------------------------------------------*/
#define instr setl

static void do_execute() {
	if(eflags.SF != eflags.OF) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)

#undef instr
/*---------------------------------------------*/
#define instr setge

static void do_execute() {
	if(eflags.SF == eflags.OF) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)

#undef instr
/*---------------------------------------------*/
#define instr setle

static void do_execute() {
	if(eflags.ZF == 1 || eflags.SF != eflags.OF) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)

#undef instr
/*---------------------------------------------*/
#define instr setg

static void do_execute() {
	if(eflags.ZF == 0 && eflags.SF == eflags.OF) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)

#undef instr
/*---------------------------------------------*/
#define instr setcxz

static void do_execute() {
	if(cpu.ecx == 0) OPERAND_W(op_src, 1);
	else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)
#undef instr
/*---------------------------------------------*/

#include "cpu/exec/template-end.h"
