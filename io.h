#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>

#include "memory_management.h"

int getline(char **, int *, FILE *);

bool readln_int(FILE * f, int * res) {
	char * buf = NULL;
	int len = 0;
	int read = getline(&buf, &len, f);
	if (read == -1 || read == 1) {
		if (buf != NULL) free(buf);
		return false;
	}
	char * end_ptr;
	long int z = strtol(buf, &end_ptr, 10);
	if (end_ptr != buf + read - 1) {
		if (buf != NULL) free(buf);
		return false;
	}
	if (errno == ERANGE) {
		if (buf != NULL) free(buf);
		return false;
	}
	if (z < INT_MIN || z > INT_MAX) {
		if (buf != NULL) free(buf);
		return false;
	}
	free(buf);
	*res = (int)z;
	return true;
}

bool readln_int_range(FILE * f, int l, int r, int * res) {
	int x;
	if (!readln_int(f, &x)) return false;
	if (x < l || x > r) {
		return false;
	}
	*res = x;
	return true;
}

bool readln_non_null(FILE * f, char ** res) {
	char * buf = NULL;
	int len = 0;
	int read = getline(&buf, &len, f);
	if (read == -1) {
		if (buf != NULL) free(buf);
		return false;
	}
	char * str = NEWARR(char, read);
	memcpy(str, buf, read);
	str[read - 1] = '\0';
	free(buf);
	*res = str;
	return true;
}

char * readln_non_null_from_console(const char * variable_name) {
	printf("enter %s:\n", variable_name);
	fflush(stdout);
	char * res = NULL;
	while (true) {
		if (readln_non_null(stdin, &res)) break;
		puts("enter non-empty line!");
		fflush(stdout);
	}
	return res;
}

int readln_int_from_console(const char * variable_name) {
	printf("enter %s:\n", variable_name);
	fflush(stdout);
	int x;
	while (true) {
		if (readln_int(stdin, &x)) break;
		printf("enter integer in range [%d, %d] !\n", INT_MIN, INT_MAX);
		fflush(stdout);
	}
	return x;
}

int readln_int_range_from_console(const char * variable_name, int l, int r) {
	printf("enter %s in range [%d, %d]:\n", variable_name, l, r);
	fflush(stdout);
	int x;
	while (true) {
		if (readln_int_range(stdin, l, r, &x)) break;
		printf("enter integer in range [%d, %d] !\n", l, r);
		fflush(stdout);
	}
	return x;
}

void print_char_set(const char * set) {
	bool was = false;
	putchar('{');
	for (int i = 0; set[i] != '\0'; ++i) {
		if (was) {
			printf(", ");
		}
		was = true;
		putchar(set[i]);
	}
	putchar('}');
}

bool contains(const char * set, char x) {
	for (int i = 0; set[i] != '\0'; i++) {
		if (set[i] == x) return true;
	}
	return false;
}

char read_char_from_set_from_console(const char * set) {
	char * s = NULL;
	while (true) {
		if (readln_non_null(stdin, &s) && strlen(s) == 1 && contains(set, s[0])) break;
		printf("enter symbol from the set ");
		print_char_set(set);
		puts("");
		fflush(stdout);
		s = NULL;
	}
	return s[0];
}
