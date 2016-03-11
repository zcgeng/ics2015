#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
    long long A = a;
    long long B = b;
    FLOAT C = (A * B) >> 16; 
    return C;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	unsigned long long A = Fabs(a);
    A <<= 16;
	unsigned long long B = Fabs(b);
	unsigned long long left = 0;
	unsigned long long right = 0xffffffffffffffff;
	unsigned long long middle;
	while(left < right){
		middle = (right + left) >> 1;
		middle &= 0x7fffffffffffffff;
		if(middle * B > A) right = middle - 1;
		else if(middle * B < A) left = middle + 1;
		else break;
	}
    if( (a > 0 && b < 0) || (a < 0 && b > 0) ) middle = -middle;
	return middle;
}

FLOAT f2F(float a) {
        unsigned int aa = *(int*)&a;
        int Sign = (aa >> 31) & 1;
        int Exp = (aa >> 23) & 0xff;
        int Fraction = aa & 0x7fffff;
        int shr = (-16 - Exp + 127 + 23);
        int ans = (Fraction | 0x800000);
        if(shr > 0)
            ans = ans >> shr;
        else
            ans = ans << (-shr);
        if(Sign == 1)
            ans = -ans;
        return ans;
    
}

FLOAT Fabs(FLOAT a) {
	return a < 0 ? -a : a;
}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

