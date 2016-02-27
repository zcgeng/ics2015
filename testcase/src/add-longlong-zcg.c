#include "trap.h"

int main() {
	long long a = 1LL;
	long long b = 0x7fffffffffffffffLL;
	long long c = a + b;
	nemu_assert(c == 0x8000000000000000LL);

	HIT_GOOD_TRAP;

	return 0;
}
