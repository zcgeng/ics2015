#include "cpu/exec/template-start.h"

#define instr cmova
static void do_execute() {
	if (eflags.CF == 0 && eflags.ZF==0) {
		OPERAND_W(op_dest, op_src->val);
	}
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#define instr cmove
static void do_execute() {
	if (eflags.ZF == 1) {
		OPERAND_W(op_dest, op_src->val);
	}
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#define instr cmovge
static void do_execute() {
	if (eflags.SF == eflags.OF) {
		OPERAND_W(op_dest, op_src->val);
	}
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#define instr cmovne
static void do_execute() {
	if (eflags.ZF == 0) {
		OPERAND_W(op_dest, op_src->val);
	}
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#define instr cmovs
static void do_execute() {
	if (eflags.SF == 1) {
		OPERAND_W(op_dest, op_src->val);
	}
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#define instr cmovns
static void do_execute() {
	if (eflags.SF == 0) {
		OPERAND_W(op_dest, op_src->val);
	}
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#define instr cmovo
static void do_execute() {
	if (eflags.OF == 1) {
		OPERAND_W(op_dest, op_src->val);
	}
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#define instr cmovno
static void do_execute() {
	if (eflags.OF == 0) {
		OPERAND_W(op_dest, op_src->val);
	}
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#define instr cmovb
static void do_execute() {
	if (eflags.CF == 1) {
		OPERAND_W(op_dest, op_src->val);
	}
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#define instr cmovae
static void do_execute() {
	if (eflags.CF == 0) {
		OPERAND_W(op_dest, op_src->val);
	}
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#define instr cmovp
static void do_execute() {
	if (eflags.PF == 1) {
		OPERAND_W(op_dest, op_src->val);
	}
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#define instr cmovnp
static void do_execute() {
	if (eflags.PF == 0) {
		OPERAND_W(op_dest, op_src->val);
	}
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#define instr cmovl
static void do_execute() {
	if (eflags.SF != eflags.OF) {
		OPERAND_W(op_dest, op_src->val);
	}
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#define instr cmovle
static void do_execute() {
	if (eflags.ZF == 1 || eflags.SF != eflags.OF) {
		OPERAND_W(op_dest, op_src->val);
	}
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#define instr cmovg
static void do_execute() {
	if (eflags.ZF == 0 && eflags.SF == eflags.OF) {
		OPERAND_W(op_dest, op_src->val);
	}
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#define instr cmovbe
static void do_execute() {
	if (eflags.CF == 1 || eflags.ZF == 1) {
		OPERAND_W(op_dest, op_src->val);
	}
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr
#include "cpu/exec/template-end.h"
