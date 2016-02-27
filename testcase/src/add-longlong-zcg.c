#include "trap.h"

int main() {
	long long a = 0x7fffffffffffffffLL;
	long long b = 0;
	long long c = a + b;
	nemu_assert(c == 0x7fffffffffffffffLL);

	HIT_GOOD_TRAP;

	return 0;
}
