#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-template.h"
#undef DATA_BYTE

make_helper_v(je_i);
make_helper_v(jbe_i);
make_helper_v(jne_i);
make_helper_v(js_i);
make_helper_v(jns_i);
make_helper_v(jg_i);
make_helper_v(jge_i);
make_helper_v(jl_i);
make_helper_v(jle_i);
make_helper_v(ja_i);
make_helper_v(jae_i);
make_helper_v(jb_i);
make_helper_v(jno_i);
make_helper_v(jo_i);
make_helper_v(jp_i);
make_helper_v(jnp_i);
/*
extern char addr_asm[];
void print_addr(swaddr_t);
extern char suffix;

#define CC e
#define COND (eflags.ZF)
#include "jcc-template.h"

#define CC ne
#define COND (!eflags.ZF)
#include "jcc-template.h"

#define CC s
#define COND (eflags.SF)
#include "jcc-template.h"

#define CC ns
#define COND (!eflags.SF)
#include "jcc-template.h"

#define CC g
#define COND (!(eflags.SF^eflags.OF)&&!eflags.ZF)
#include "jcc-template.h"

#define CC ge
#define COND (!(eflags.SF^eflags.OF))
#include "jcc-template.h"

#define CC l
#define COND (eflags.SF^eflags.OF)
#include "jcc-template.h"

#define CC le
#define COND ((eflags.SF^eflags.OF)||eflags.ZF)
#include "jcc-template.h"

#define CC a
#define COND (!eflags.CF&&!eflags.ZF)
#include "jcc-template.h"

#define CC ae
#define COND (!eflags.CF)
#include "jcc-template.h"

#define CC b
#define COND (eflags.CF)
#include "jcc-template.h"

#define CC be
#define COND (eflags.CF||eflags.ZF)
#include "jcc-template.h"

#define CC no
#define COND (!eflags.OF)
#include "jcc-template.h"

#define CC o
#define COND (eflags.OF)
#include "jcc-template.h"

#define CC p
#define COND (eflags.PF)
#include "jcc-template.h"

#define CC po
#define COND (!eflags.PF)
#include "jcc-template.h"

make_helper_v(concat(j, CC, _i));


#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-template.h"
#undef DATA_BYTE

*/

/* for instruction encoding overloading */


