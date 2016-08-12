#pragma once

#include "io.h"

#define DEFINE_LIST(T) \
\
typedef struct list_##T list_##T; \
struct list_##T { \
	list_##T * nxt; \
	bool deleted; \
	T element; \
\
}; \
\
int length_list_##T(list_##T * v) { \
	int res = 0; \
	while (v != NULL) { \
		if (!v->deleted) ++res; \
		v = v->nxt; \
	} \
	return res; \
} \
void print_list_##T(list_##T * v, void (*print)(FILE *, T), FILE * f) { \
	fprintf(f, "%d\n", length_list_##T(v)); \
	while (v != NULL) { \
		if (!v->deleted) print(f, v->element); \
		v = v->nxt; \
	} \
} \
bool read_list_##T(list_##T ** to, bool (*read)(FILE *, T *), FILE * f) { \
	int n; \
	if (!readln_int_range(f, 0, INT_MAX, &n)) return false; \
	list_##T * res = NULL; \
	while (n--) { \
		list_##T * cur = (list_##T *)memory_manager_alloc(sizeof(list_##T)); \
		if (!read(f, &cur->element)) return false; \
		cur->nxt = res; \
		cur->deleted = false; \
		res = cur; \
	} \
	*to = res; \
	return true; \
} \
void add_list_##T(list_##T ** v, T x) { \
	list_##T * nw = (list_##T *)memory_manager_alloc(sizeof(list_##T)); \
	nw->element = x; \
	nw->deleted = false; \
	nw->nxt = *v; \
	*v = nw; \
} \
bool any_list_##T(list_##T * v, bool (*pred)(const T *)) { \
	while (v != NULL) { \
		if (!v->deleted && pred(&v->element)) return true; \
		v = v->nxt; \
	} \
	return false; \
} \
bool delete_any_list_##T(list_##T * v, bool (*pred)(const T *)) { \
	bool res = false; \
	while (v != NULL) { \
		if (!v->deleted && pred(&v->element)) { \
			v->deleted = true; \
			res = true; \
		} \
		v = v->nxt; \
	} \
	return res; \
} \
int count_list_##T(list_##T * v, bool (*pred)(const T *)) { \
	int res = 0; \
	while (v != NULL) { \
		if (!v->deleted && pred(&v->element)) { \
			res++; \
		} \
		v = v->nxt; \
	} \
	return res; \
}

#define list(T) list_##T

#define read_list(T) read_list_##T

#define print_list(T) print_list_##T

#define add_list(T) add_list_##T

#define length_list(T) length_list_##T

#define any_list(T) any_list_##T

#define delete_any_list(T) delete_any_list_##T

#define count_list(T) count_list_##T
