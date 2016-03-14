#include "cpu/exec/template-start.h"

/*---------------------------------------------*/
#define instr cmovo

static void do_execute() {
	if(cpu.OF == 1) {OPERAND_W(op_dest, op_src->val);}
}

make_instr_helper(rm2r)

#undef instr
/*---------------------------------------------*/
#define instr cmovno

static void do_execute() {
	if(cpu.OF == 0) {OPERAND_W(op_dest, op_src->val);}
}

make_instr_helper(rm2r)

#undef instr
/*---------------------------------------------*/
#define instr cmovb

static void do_execute() {
	if(cpu.CF == 1) {OPERAND_W(op_dest, op_src->val);}
}

make_instr_helper(rm2r)

#undef instr
/*---------------------------------------------*/
#define instr cmovae

static void do_execute() {
	if(cpu.CF == 0) {OPERAND_W(op_dest, op_src->val);}
}

make_instr_helper(rm2r)

#undef instr
/*---------------------------------------------*/
#define instr cmove

static void do_execute() {
	if(cpu.ZF == 1) {OPERAND_W(op_dest, op_src->val);}
}

make_instr_helper(rm2r)

#undef instr
/*---------------------------------------------*/
#define instr cmovne

static void do_execute() {
	if(cpu.ZF == 0) {OPERAND_W(op_dest, op_src->val);}
}

make_instr_helper(rm2r)

#undef instr
/*---------------------------------------------*/
#define instr cmovbe

static void do_execute() {
	if(cpu.CF == 1 || cpu.ZF == 1) {OPERAND_W(op_dest, op_src->val);}
}

make_instr_helper(rm2r)

#undef instr
/*---------------------------------------------*/
#define instr cmova

static void do_execute() {
	if(cpu.CF == 0 && cpu.ZF == 0) {OPERAND_W(op_dest, op_src->val);}
}

make_instr_helper(rm2r)

#undef instr
/*---------------------------------------------*/
#define instr cmovs

static void do_execute() {
	if(cpu.SF == 1) {OPERAND_W(op_dest, op_src->val);}
}

make_instr_helper(rm2r)

#undef instr
/*---------------------------------------------*/
#define instr cmovns

static void do_execute() {
	if(cpu.SF == 0) {OPERAND_W(op_dest, op_src->val);}
}

make_instr_helper(rm2r)

#undef instr
/*---------------------------------------------*/
#define instr cmovp

static void do_execute() {
	
	if(cpu.PF == 1) {OPERAND_W(op_dest, op_src->val);}
}

make_instr_helper(rm2r)

#undef instr
/*---------------------------------------------*/
#define instr cmovnp

static void do_execute() {
	
	if(cpu.PF == 0) {OPERAND_W(op_dest, op_src->val);}
}

make_instr_helper(rm2r)

#undef instr
/*---------------------------------------------*/
#define instr cmovl

static void do_execute() {
	
	if(cpu.SF != cpu.OF) {OPERAND_W(op_dest, op_src->val);}
}

make_instr_helper(rm2r)

#undef instr
/*---------------------------------------------*/
#define instr cmovge

static void do_execute() {
	
	if(cpu.SF == cpu.OF) {OPERAND_W(op_dest, op_src->val);}
}

make_instr_helper(rm2r)

#undef instr
/*---------------------------------------------*/
#define instr cmovle

static void do_execute() {
	
	if(cpu.ZF == 1 || cpu.SF != cpu.OF) {OPERAND_W(op_dest, op_src->val);}
}

make_instr_helper(rm2r)

#undef instr
/*---------------------------------------------*/
#define instr cmovg

static void do_execute() {
	
	if(cpu.ZF == 0 && cpu.SF == cpu.OF) {OPERAND_W(op_dest, op_src->val);}
}

make_instr_helper(rm2r)

#undef instr
/*---------------------------------------------*/
#define instr cmovcxz

static void do_execute() {
	
	if(cpu.ecx == 0) {OPERAND_W(op_dest, op_src->val);}
}

make_instr_helper(rm2r)
#undef instr
/*---------------------------------------------*/

#undef CODE_LEN
#include "cpu/exec/template-end.h"
