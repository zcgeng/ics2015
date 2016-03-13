#include "FLOAT.h"
#include <stdio.h>
#include <stdlib.h>

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	long long A = a;
	long long B = b;
	long long ans = A * B;
	return ans>>16;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	unsigned long long A = Fabs(a), B = Fabs(b);
    unsigned long long l = 0, r = 0xffffffffffffffffll, middle;
    A = A << 16;
    while(l < r){
        middle = (l + r) >> 1;
        //printf("middle = 0x%llx\n", middle);
        unsigned long long tmp = middle * B;
        if(tmp < A) l = middle + 1;
        else if(tmp > A) r = middle - 1;
        else break;
    }
    long long ans = middle;
    if((a < 0 && b > 0) || (a > 0 && b < 0)) ans = -ans;
	return ans;
}

FLOAT f2F(float a) {
	int i, uf, m, e, s, ans;
	uf = *(int*)&a;
	m = uf & ((1 << 23) - 1);
	e = ((uf >> 23) & ((1 << 8) - 1)) - 127;
	s = uf >> 31;
	ans = 1;
	for(i = 1; i <= e + 16; ++ i) {
		ans = (ans << 1) + ((m & (1 << 22)) >> 22);
		if (ans < 0) return 0x80000000u;
		m = m << 1;
	}
	if (s != 0) ans = (~ans) + 1;
	return (FLOAT)(ans);
}

FLOAT Fabs(FLOAT a) {
	FLOAT result = a<0?-a:a;
	return result;
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
