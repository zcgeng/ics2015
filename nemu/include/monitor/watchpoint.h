#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"
#include "cpu/reg.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
    char s_expr[128];
    int last_value;
	/* TODO: Add more members if necessary */
} WP;

void new_wp(char*);
void check_wp();
void free_wp(int number);
#endif
