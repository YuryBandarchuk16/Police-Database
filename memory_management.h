#pragma once

#include <stdlib.h>

void free(void *);
void * malloc(size_t);

typedef struct memory_manager_tree memory_manager_tree;

struct memory_manager_tree {
	memory_manager_tree * nxt;
	void * ptr;
};

memory_manager_tree * memory_manager_head = NULL;

void finish_program() {
	while (memory_manager_head != NULL) {
		memory_manager_tree * nxt = memory_manager_head->nxt;
		free(memory_manager_head->ptr);
		free(memory_manager_head);
		memory_manager_head = nxt;
	}
	exit(0);
}

void out_of_memory() {
	puts("Out of memory exception");
	finish_program();
	exit(0);
}

void * memory_manager_alloc(size_t sz) {
	memory_manager_tree * nw = (memory_manager_tree *)malloc(sizeof(memory_manager_tree));
	if (nw == NULL) {
		out_of_memory();
	}
	nw->ptr = malloc(sz);
	if (nw->ptr == NULL) {
		free(nw);
		out_of_memory();
	}
	nw->nxt = memory_manager_head;
	memory_manager_head = nw;
	return nw->ptr;
}

#define NEW(TYPE) ((TYPE *)memory_manager_alloc(sizeof(TYPE)))
#define NEWARR(TYPE, CNT) ((TYPE *)memory_manager_alloc(sizeof(TYPE) * CNT))


