/*************************************************************************
	> File Name: test.c
	> Author: zcgeng
	> Mail: zcg1996@qq.com
	> Created Time: 2016年03月09日 星期三 17时34分50秒
 ************************************************************************/

#include<stdio.h>
typedef int FLOAT;
FLOAT f2F(float a) {
        unsigned int aa = *(int*)&a;
        int Sign = (aa >> 31) & 1;
        int Exp = (aa >> 23) & 0xff;
        int Fraction = aa & 0x7fffff;
        int shr = (-16 - Exp + 127 + 23);
        int ans = (Fraction | 0x800000);
        if(shr > 0)
            ans >>= shr;
        else
            ans << (-shr);
        if(Sign == 1)
            ans = -ans;
        return ans;
    
}
int main(){
    printf("%08x\n", f2F(5.6));
    return 0;
}
//int main(){
//    FLOAT a=f2F(1.2);
//    FLOAT b=int2F(10);
//    int c=0;
//    if(b>f2F(7.9)){
//         c=F2int(F_div_F(F_mul_F((a + int2F(1)),b),f2F(2.3)));
//    }
//    printf("%d\n", c);
//}
