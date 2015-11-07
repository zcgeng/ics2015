#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_list[NR_WP];
static WP *head, *free_;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_list;
}

/* TODO: Implement the functionality of watchpoint */


void new_wp(char* args){
    if( free == NULL ) assert(0);
    WP* tmp = free_;
    tmp -> s_expr = args;
    bool success;
    tmp -> last_value = expr(args, &success);
    if( succsee == false ){
        printf("Failed to create a new watchpoint(bad expression)\n");
        return;
    }
    free_ = free_ -> next;
    tmp -> next = head;
    head = tmp;
    return tmp;
}

void free_wp(int number){
    WP* tmp = head;
    WP* last = head;
    while(tmp){
        if( tmp -> NO == number)
            break;
        tmp = tmp -> next;
        last = tmp;
    }
    if( tmp == NULL ){
        printf("didn't find watchpoint number : %d\n", number);
        return;
    }
    last -> next = tmp -> next;
    tmp -> next = free_;
    free_ = tmp;
    return;
}

