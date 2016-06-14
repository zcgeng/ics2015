#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next, *previous;
	int value;
	char expr[51];
	/* TODO: Add more members if necessary */


} WP;

#endif
