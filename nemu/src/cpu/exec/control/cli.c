#include "cpu/exec/helper.h"
/*
#define DATA_BYTE 1
#include "hlt-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "hlt-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "hlt-template.h"
#undef DATA_BYTE
*/
#define DATA_BYTE 4
#include "cpu/exec/template-start.h"

make_helper(cli) {
	eflags.IF=0;
	print_asm("sti");
	return 1;
}
#include "cpu/exec/template-end.h"
#undef DATA_BYTE
/* for instruction encoding overloading */
