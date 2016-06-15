#include "cpu/exec/template-start.h"

#define instr int
void raise_intr(uint8_t, uint32_t);

static void do_execute() {
//	printf("size=%d len=%d\n", op_src->size, len + 1);
//	printf("val=%d\n", op_src->val);
	int len;
	len = get_len();
	print_asm_template1();
	raise_intr(op_src->val, len);
}

make_instr_helper(i);

#include "cpu/exec/template-end.h"
