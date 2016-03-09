#include "trap.h"
#include <string.h>

	char h[2] = "h";
	char e[2] = "e";
	char he[4] = "he";
int main() {
	
	nemu_assert(strcmp(he, "he") == 0);

	HIT_GOOD_TRAP;
	return 0;
}
