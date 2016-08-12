#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "memory_management.h"
#include "io.h"
#include "list.h"

const int MIN_HEIGHT = 80;
const int MAX_HEIGHT = 300;
const int MIN_WEIGHT = 30;
const int MAX_WEIGHT = 1000;

void reading_database_error() {
	puts("Reading database error");
	finish_program();
}

#define FILE_DATABASE "database.txt"

typedef char * char_ptr;

typedef struct crime crime;
typedef struct bandit bandit;
typedef struct pair_crime_bandit pair_crime_bandit;
typedef struct pair_crime_comment pair_crime_comment;

DEFINE_LIST(char_ptr)

struct crime {
	int crime_id;
	char * crime_scene;
	char * effects;
};

DEFINE_LIST(crime)

struct bandit {
	int bandit_id;
	char * full_name;
	char * nick_name;
	int height;
	int weight;
	char * hair_color;
	char * special_signs;
	char * citizenship;
	list(char_ptr) *  languages;
	char * terroristic_organization;
};

DEFINE_LIST(bandit)

struct pair_crime_bandit {
	int crime_id;
	int bandit_id;
};

DEFINE_LIST(pair_crime_bandit)

struct pair_crime_comment {
	int crime_id;
	char * comment;
};

DEFINE_LIST(pair_crime_comment)

list(crime) * crime_list = NULL;
list(bandit) * bandit_list = NULL;
list(pair_crime_bandit) * pair_crime_bandit_list = NULL;
list(pair_crime_comment) * pair_crime_comment_list = NULL;


int global_ID;
bool check_crime_ID(const crime * x) {
	if (x->crime_id == global_ID) return true;
	return false;
}
bool check_bandit_ID(const bandit * x) {
	if (x->bandit_id == global_ID) return true;
	return false;
}

//crime = (crime_id, crime_scene, effects)
bool read_crime(FILE * f, crime * res) {
	if (f == NULL) {
		return false;
	}
	if (!readln_int_range(f, 0, INT_MAX, &res->crime_id)) return false;
	global_ID = res->crime_id;
	if (any_list(crime)(crime_list, check_crime_ID)) return false;
	if (!readln_non_null(f, &res->crime_scene)) return false;
	if (!readln_non_null(f, &res->effects)) return false;
	return true;
}

//bandit = (bandit_id, full_name, nick_name, height, weight, hair_color, special_signs, citizenship, list of languages, terroristic_organization)
bool read_bandit(FILE * f, bandit * res) {
	if (f == NULL) {
		return false;
	}
	if (!readln_int_range(f, 0, INT_MAX, &res->bandit_id)) return false;
	global_ID = res->bandit_id;
	if (any_list(bandit)(bandit_list, check_bandit_ID)) return false;
	if (!readln_non_null(f, &res->full_name)) return false;
	if (!readln_non_null(f, &res->nick_name)) return false;
	if (!readln_int_range(f, MIN_HEIGHT, MAX_HEIGHT, &res->height)) return false;
	if (!readln_int_range(f, MIN_WEIGHT, MAX_WEIGHT, &res->weight)) return false;
	if (!readln_non_null(f, &res->hair_color)) return false;
	if (!readln_non_null(f, &res->special_signs)) return false;
	if (!readln_non_null(f, &res->citizenship)) return false;
	if (!read_list(char_ptr)(&res->languages, readln_non_null, f)) return false;
	if (!readln_non_null(f, &res->terroristic_organization)) return false;
	return true;
}

//pair_crime_bandit = (crime_id, bandit_id)
bool read_pair_crime_bandit(FILE * f, pair_crime_bandit * res) {
	if (!readln_int(f, &res->crime_id)) return false;
	global_ID = res->crime_id;
	if (!any_list(crime)(crime_list, check_crime_ID)) return false;
	if (!readln_int(f, &res->bandit_id)) return false;
	global_ID = res->bandit_id;
	if (!any_list(bandit)(bandit_list, check_bandit_ID)) return false;
	return true;
}

//pair_crime_comment = (crime_id, comment)
bool read_pair_crime_comment(FILE * f, pair_crime_comment * res) {
	if (!readln_int(f, &res->crime_id)) return false;
	global_ID = res->crime_id;
	if (!any_list(crime)(crime_list, check_crime_ID)) return false;
	if (!readln_non_null(f, &res->comment)) return false;
	return true;
}

void print_char_ptr(FILE * f, char_ptr x) {
	fprintf(f, "%s\n", x);
}

//crime = (crime_id, crime_scene, effects)
void print_crime(FILE * f, crime x) {
	fprintf(f, "%d\n", x.crime_id);
	fprintf(f, "%s\n", x.crime_scene);
	fprintf(f, "%s\n", x.effects);
}

//bandit = (bandit_id, full_name, nick_name, height, weight, hair_color, special_signs, citizenship, list of languages, terroristic_organization)
void print_bandit(FILE * f, bandit x) {
	fprintf(f, "%d\n", x.bandit_id);
	fprintf(f, "%s\n", x.full_name);
	fprintf(f, "%s\n", x.nick_name);
	fprintf(f, "%d\n", x.height);
	fprintf(f, "%d\n", x.weight);
	fprintf(f, "%s\n", x.hair_color);
	fprintf(f, "%s\n", x.special_signs);
	fprintf(f, "%s\n", x.citizenship);
	print_list(char_ptr)(x.languages, print_char_ptr, f);
	fprintf(f, "%s\n", x.terroristic_organization);
}

//pair_crime_bandit = (crime_id, bandit_id)
void print_pair_crime_bandit(FILE * f, pair_crime_bandit x) {
	fprintf(f, "%d\n", x.crime_id);
	fprintf(f, "%d\n", x.bandit_id);
}

//pair_crime_comment = (crime_id, comment)
void print_pair_crime_comment(FILE * f, pair_crime_comment x) {
	fprintf(f, "%d\n", x.crime_id);
	fprintf(f, "%s\n", x.comment);
}

void create_empty_database() {
	FILE * f = fopen(FILE_DATABASE, "w");
	fprintf(f, "0\n0\n0\n0\n");
	fclose(f);
}

void load_database() {
	FILE * f = fopen(FILE_DATABASE, "r");
	if (f == NULL) {
		create_empty_database();	
		f = fopen(FILE_DATABASE, "r");
	}
	if (!read_list(crime)(&crime_list, read_crime, f)) {
		fclose(f);
		reading_database_error();
	}
	if (!read_list(bandit)(&bandit_list, read_bandit, f)) {
		fclose(f);
		reading_database_error();
	}
	if (!read_list(pair_crime_bandit)(&pair_crime_bandit_list, read_pair_crime_bandit, f)) {
		fclose(f);
		reading_database_error();
	}
	if (!read_list(pair_crime_comment)(&pair_crime_comment_list, read_pair_crime_comment, f)) {
		fclose(f);
		reading_database_error();
	}
	fclose(f);
}

void save_database() {
	FILE * f = fopen(FILE_DATABASE, "w");
	print_list(crime)(crime_list, print_crime, f);
	print_list(bandit)(bandit_list, print_bandit, f);
	print_list(pair_crime_bandit)(pair_crime_bandit_list, print_pair_crime_bandit, f);
	print_list(pair_crime_comment)(pair_crime_comment_list, print_pair_crime_comment, f);
	fclose(f);
}

//crime = (crime_id, crime_scene, effects)
crime read_crime_from_console() {
	crime res;
	res.crime_id = (crime_list == NULL ? 0 : crime_list->element.crime_id + 1);
	res.crime_scene = readln_non_null_from_console("crime scene");
	res.effects = readln_non_null_from_console("effects from crime");
	return res;
}

list(char_ptr) * read_list_of_languages_from_console() {
	int n = readln_int_range_from_console("number of language(s) of bandit", 1, 20);
	list(char_ptr) * res = NULL;
	for (int i = 1; i <= n; i++) {
		char buff[100];
		sprintf(buff, "enter language number %d", i);
		add_list(char_ptr)(&res, readln_non_null_from_console(buff));
	}
	return res;
}

//bandit = (bandit_id, full_name, nick_name, height, weight, hair_color, special_signs, citizenship, list of languages, terroristic_organization)
bandit read_bandit_from_console() {
	bandit res;
	res.bandit_id = (bandit_list == NULL ? 0 : bandit_list->element.bandit_id + 1);
	res.full_name = readln_non_null_from_console("full name of bandit");
	res.nick_name = readln_non_null_from_console("nickname of bandit");
	res.height = readln_int_range_from_console("height of bandit", MIN_HEIGHT, MAX_HEIGHT);
	res.weight = readln_int_range_from_console("weight of bandit", MIN_WEIGHT, MAX_WEIGHT);
	res.hair_color = readln_non_null_from_console("hair color of bandit");
	res.special_signs = readln_non_null_from_console("special signs of bandit");
	res.citizenship = readln_non_null_from_console("citizenship of bandit");
	res.languages = read_list_of_languages_from_console();
	res.terroristic_organization = readln_non_null_from_console("terroristic organization of bandit (none if none)");
	return res;
}

void print_bandit_info_short(FILE * f, bandit x) {
	fprintf(f, "Bandit unique ID: %d, full name: %s, nickname: %s\n", 
			x.bandit_id, x.full_name, x.nick_name);
}

void print_crime_info_short(FILE * f, crime x) {
	fprintf(f, "Crime unique ID: %d, crime scene: %s, effects: %s\n", 
			x.crime_id, x.crime_scene, x.effects);
}

int readln_int_from_crime_IDs_from_console() {
	int x;
	while (true) {
		printf("enter crime ID from the following:\n");
		printf("number of crimes ");
		print_list(crime)(crime_list, print_crime_info_short, stdout);
		fflush(stdout);
		x = readln_int_from_console("enter single integer");
		global_ID = x;
		if (any_list(crime)(crime_list, check_crime_ID)) {
			break;
		}
	}
	return x;
}

int readln_int_from_bandit_IDs_from_console() {
	int x;
	while (true) {
		printf("enter bandit ID from the following:\n");
		printf("number of bandits ");
		print_list(bandit)(bandit_list, print_bandit_info_short, stdout);
		fflush(stdout);
		x = readln_int_from_console("enter single integer");
		global_ID = x;
		if (any_list(bandit)(bandit_list, check_bandit_ID)) {
			break;
		}
	}
	return x;
}

//pair_crime_bandit = (crime_id, bandit_id)
pair_crime_bandit read_pair_crime_bandit_from_console() {
	pair_crime_bandit res;

	res.crime_id = readln_int_from_crime_IDs_from_console();
	res.bandit_id = readln_int_from_bandit_IDs_from_console();

	return res;
}

void do_you_want_to_save() {
	puts("Do you want to save changes? y or n");
	fflush(stdout);
	char c = read_char_from_set_from_console("yn");
	if (c == 'y') {
		save_database();
		puts("database saved!");
		fflush(stdout);
	}
}
//bandit = (bandit_id, full_name, nick_name, height, weight, hair_color, special_signs, citizenship, list of languages, terroristic_organization)
void print_bandit_info(FILE * f, bandit x) {
	puts("------------------------------------------------------------------------------");
	fprintf(f, "Bandit unique ID: %d\n", x.bandit_id);
	fprintf(f, "Full Name: %s\n", x.full_name);
	fprintf(f, "Nickname: %s\n", x.nick_name);
	fprintf(f, "Height: %d sm\n", x.height);
	fprintf(f, "Weight: %d kg\n", x.weight);
	fprintf(f, "Hair color: %s\n", x.hair_color);
	fprintf(f, "Special signs: %s\n", x.special_signs);
	fprintf(f, "Citizenship: %s\n", x.citizenship);
	fprintf(f, "Number of languages is ");
	print_list(char_ptr)(x.languages, print_char_ptr, f);
	fprintf(f, "Terroristic organization: %s\n", x.terroristic_organization);
}

//crime = (crime_id, crime_scene, effects)
void print_crime_info(FILE * f, crime x) {
	puts("------------------------------------------------------------------------------");
	fprintf(f, "Crime unique ID: %d\n", x.crime_id);
	fprintf(f, "Crime scene: %s\n", x.crime_scene);
	fprintf(f, "Effects: %s\n", x.effects);
}

void print_info_about_bandits() {
	printf("number of bandits in database is ");
	print_list(bandit)(bandit_list, print_bandit_info, stdout);
	fflush(stdout);
}

void print_info_about_crimes() {
	printf("number of crimes in database is ");
	print_list(crime)(crime_list, print_crime_info, stdout);
	fflush(stdout);
}

bool check_bandit_ID_pair_crime_bandit(const pair_crime_bandit * x) {
	return x->bandit_id == global_ID;
}

bool check_crime_ID_pair_crime_bandit(const pair_crime_bandit * x) {
	return x->crime_id == global_ID;
}

bool check_crime_ID_pair_crime_comment(const pair_crime_comment * x) {
	return x->crime_id == global_ID;
}

void delete_bandit() {
	if (length_list(bandit)(bandit_list) == 0) {
		puts("There is no bandits in database!");
		fflush(stdout);
		return;
	}
	int x = readln_int_from_bandit_IDs_from_console();
	global_ID = x;
	delete_any_list(bandit)(bandit_list, check_bandit_ID);
	delete_any_list(pair_crime_bandit)(pair_crime_bandit_list, check_bandit_ID_pair_crime_bandit);
	puts("Bandit deleted!");
}

void delete_crime() {
	if (length_list(crime)(crime_list) == 0) {
		puts("There is no crimes in database!");
		fflush(stdout);
		return;
	}
	int x = readln_int_from_crime_IDs_from_console();
	global_ID = x;
	delete_any_list(crime)(crime_list, check_crime_ID);
	delete_any_list(pair_crime_bandit)(pair_crime_bandit_list, check_crime_ID_pair_crime_bandit);
	delete_any_list(pair_crime_comment)(pair_crime_comment_list, check_crime_ID_pair_crime_comment);
	puts("Crime deleted!");
}

void add_comment_to_crime() {
	if (length_list(crime)(crime_list) == 0) {
		puts("There is no crimes in database!");
		fflush(stdout);
		return;
	}
	pair_crime_comment res;
	res.crime_id = readln_int_from_crime_IDs_from_console();
	res.comment = readln_non_null_from_console("comment to crime");
	add_list(pair_crime_comment)(&pair_crime_comment_list, res); 
	puts("Comment added!");
	fflush(stdout);
}

void add_bandit() {
	bandit x = read_bandit_from_console();
	add_list(bandit)(&bandit_list, x);
	puts("Bandit added!");
}

void add_crime() {
	crime x = read_crime_from_console();
	add_list(crime)(&crime_list, x);
	puts("Crime added!");
}

int global_ID_2;

bool check_pair_crime_bandit_for_exist(const pair_crime_bandit * x) {
	return x->crime_id == global_ID && x->bandit_id == global_ID_2;
}

void connect_crime_and_bandit() {
	if (length_list(crime)(crime_list) == 0) {
		puts("There is no crimes in database!");
		fflush(stdout);
		return;
	}
	if (length_list(bandit)(bandit_list) == 0) {
		puts("There is no bandits in database!");
		fflush(stdout);
		return;
	}
	pair_crime_bandit res;
	res.crime_id = readln_int_from_crime_IDs_from_console();
	res.bandit_id = readln_int_from_bandit_IDs_from_console();
	global_ID = res.crime_id;
	global_ID_2 = res.bandit_id;
	if (any_list(pair_crime_bandit)(pair_crime_bandit_list, check_pair_crime_bandit_for_exist)) {
		puts("there is already exist such connection");
		fflush(stdout);
		return;
	}
	add_list(pair_crime_bandit)(&pair_crime_bandit_list, res);
	puts("Connection between crime and bandit added!");
	fflush(stdout);
}

bool check_crime_ID_2(const crime * x) {
	if (x->crime_id == global_ID_2) {
		print_crime_info(stdout, *x);
		return true;
	}
	return false;
}

bool check_bandit_ID_2(const bandit * x) {
	if (x->bandit_id == global_ID_2) {
		print_bandit_info(stdout, *x);
		return true;
	}
	return false;
}

bool check_bandit_ID_pair_crime_bandit_2(const pair_crime_bandit * x) {
	if (x->bandit_id == global_ID) {
		global_ID_2 = x->crime_id;
		count_list(crime)(crime_list, check_crime_ID_2);
		return true;
	}
	return false;
}

bool check_crime_ID_pair_crime_bandit_2(const pair_crime_bandit * x) {
	if (x->crime_id == global_ID) {
		global_ID_2 = x->bandit_id;
		count_list(bandit)(bandit_list, check_bandit_ID_2);
		return true;
	}
	return false;
}

void print_crimes_connected_to_bandit() {
	if (length_list(bandit)(bandit_list) == 0) {
		puts("There is no bandits in database!");
		fflush(stdout);
		return;
	}
	int x = readln_int_from_bandit_IDs_from_console();
	puts("readed!");
	fflush(stdout);
	global_ID = x;
	printf("There is %d crime(s) connected to bandit:\n", count_list(pair_crime_bandit)(pair_crime_bandit_list, check_bandit_ID_pair_crime_bandit));
	fflush(stdout);
	count_list(pair_crime_bandit)(pair_crime_bandit_list, check_bandit_ID_pair_crime_bandit_2);
	fflush(stdout);
}

void print_bandits_connected_to_crime() {
	if (length_list(crime)(crime_list) == 0) {
		puts("There is no crimes in database!");
		fflush(stdout);
		return;
	}
	int x = readln_int_from_crime_IDs_from_console();
	global_ID = x;
	printf("There is %d bandit(s) connected to crime:\n", count_list(pair_crime_bandit)(pair_crime_bandit_list, check_crime_ID_pair_crime_bandit));
	count_list(pair_crime_bandit)(pair_crime_bandit_list, check_crime_ID_pair_crime_bandit_2);
}

bool check_crime_ID_pair_crime_comment_2(const pair_crime_comment * x) {
	if (x->crime_id == global_ID) {
		puts(x->comment);
		return true;
	}
	return false;
}

void print_comments_to_crime() {
	if (length_list(crime)(crime_list) == 0) {
		puts("There is no crimes in database!");
		fflush(stdout);
		return;
	}
	int x = readln_int_from_crime_IDs_from_console();
	global_ID = x;
	printf("There is %d comment(s) connected to crime:\n", count_list(pair_crime_comment)(pair_crime_comment_list, check_crime_ID_pair_crime_comment));
	count_list(pair_crime_comment)(pair_crime_comment_list, check_crime_ID_pair_crime_comment_2);
}

int main()
{
	puts("Welcome to Interpol database!");
	fflush(stdout);
	
	load_database();
	
	bool is_changed = false;

	while (true) {
		puts("Commands:");
		puts("1. Print info about bandits (also bandit IDs here)");
		puts("2. Print info about crimes (also crime IDs here)");
		puts("3. Delete bandit (you must know bandit ID)");
		puts("4. Delete crime (you must know crime ID)");
		puts("5. Add comment to crime (you must know crime ID)");
		puts("6. Add bandit");
		puts("7. Add crime");
		puts("8. Connect crime and bandit (you must know both crime and bandit IDs)");
		puts("9. Save database");
		puts("10. Print crimes connected to bandit");
		puts("11. Print bandits connected to crime");
		puts("12. Print comments to crime");
		puts("0. Exit");
		fflush(stdout);
		int comm = readln_int_range_from_console("command", 0, 12);
		if (comm == 0) {
			if (is_changed) {
				do_you_want_to_save();
			}
			break;
		}
		if (comm == 1) {
			print_info_about_bandits();	
		}
		else if (comm == 2) {
			print_info_about_crimes();
		}
		else if (comm == 3) {
			delete_bandit();				
			is_changed = true;
		}
		else if (comm == 4) {
			delete_crime();
			is_changed = true;
		}
		else if (comm == 5) {
			add_comment_to_crime();
			is_changed = true;
		}
		else if (comm == 6) {
			add_bandit();
			is_changed = true;
		}
		else if (comm == 7) {
			add_crime();
			is_changed = true;
		}
		else if (comm == 8) {
			connect_crime_and_bandit();
			is_changed = true;
		}
		else if (comm == 9) {
			save_database();
			puts("database saved!");
			is_changed = false;
		}
		else if (comm == 10) {
			print_crimes_connected_to_bandit();			
		}
		else if (comm == 11) {
			print_bandits_connected_to_crime();
		}
		else if (comm == 12) {
			print_comments_to_crime();
		}
	}

	finish_program();

	return 0;
}
