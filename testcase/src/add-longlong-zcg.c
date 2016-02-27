#include "trap.h"

int main() {
	long long a = 0x7fffffffffffffffLL + 0;
	nemu_assert(a == 0x7fffffffffffffffLL);

	HIT_GOOD_TRAP;

	return 0;
}
