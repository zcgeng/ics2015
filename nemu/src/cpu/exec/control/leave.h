/*************************************************************************
	> File Name: leave.h
	> Author: zcgeng
	> Mail: zcg1996@qq.com
	> Created Time: 2015年11月20日 星期五 21时56分42秒
 ************************************************************************/

#ifndef _LEAVE_H
#define _LEAVE_H
#include "cpu/exec/helper.h"

make_helper(leave){
    cpu.esp = cpu.ebp;
    cpu.ebp = swaddr_read(cpu.esp, 4, R_SS);
    cpu.esp += 4;
    print_asm("leave");
    return 1;
}
#endif
