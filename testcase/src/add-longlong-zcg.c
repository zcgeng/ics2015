#include "trap.h"

long long add(long long a, long long b) {
	long long c = a + b;
	return c;
}
int main() {
	long long a = add(0x7fffffffffffffffLL, 0);
	nemu_assert(a == 0x7fffffffffffffffLL);

	HIT_GOOD_TRAP;

	return 0;
}
