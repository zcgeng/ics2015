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

make_helper(hlt) {
	while (!cpu.INTR);
	print_asm("hlt");
	return 1;
}
#include "cpu/exec/template-end.h"
#undef DATA_BYTE
/* for instruction encoding overloading */
