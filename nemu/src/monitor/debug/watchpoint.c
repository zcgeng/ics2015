#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_list[NR_WP];
static WP *head, *free_;

WP* new_wp() {
	if (free_==NULL) assert(0);
	WP *new=free_;
	free_=(*free_).next;
	(*new).next=head;
	head=new;
	printf("New watchpoint %d = ", (*new).NO);
	return new;
}

void free_wp(char *expr) {
	WP *list1=head;
	int num;
	sscanf(expr, "%d", &num);
	printf("%d\n", num);
	if ((*list1).NO==num) {
		printf("Delete watchpoint %d\n", (*list1).NO);
		head=(*list1).next;
		(*list1).next=free_;
		free_=list1;
	}
	else {
		WP *list2=(*list1).next;
		while (list2!=NULL) {
			if ((*list2).NO==num) {
				printf("Delete watchpoint %d\n", (*list2).NO);
				(*list1).next=(*list2).next;
				(*list2).next=free_;
				free_=list2;
			}
			list1=list2;
			list2=(*list2).next;
		}
	}
}

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i+1;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;
	
	head = NULL;
	free_ = wp_list;
}

bool check() {
	WP *list=head;
	bool flag=false, success=true;
	while (list!=NULL) {
		int v=expr((*list).expr, &success);
		if (v!=(*list).value) {
			printf("Stop at watchpoint %d:\n", (*list).NO);
			printf("Old value = %d\n", (*list).value);
			printf("New value = %d\n", v);
			(*list).value=v;
			flag=true;
		}
		list=(*list).next;
	}
	return flag;
}

void print_w() {
	WP *list=head;
	while (list!=NULL) {
		printf("Num    Type    Disp Enb Address\n");
		printf("%-3d watchpoint keep  n  %-7s\n", (*list).NO, (*list).expr);
		list=(*list).next;
	}
}
/* TODO: Implement the functionality of watchpoint */


