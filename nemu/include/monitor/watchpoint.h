#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
    char s_expr[128];
    int last_value;
	/* TODO: Add more members if necessary */
} WP;

#endif
