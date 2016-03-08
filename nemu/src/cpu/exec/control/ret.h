#ifndef __RET_H__
#define __RET_H__

make_helper(ret_i_w);
make_helper(ret){
    cpu.eip = swaddr_read(cpu.esp, 4);
    cpu.esp += 4;
    cpu.eip -= 1;
    print_asm("ret");
    return 1;
}

#endif
