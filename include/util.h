#ifndef _UTIL_H
#define _UTIL_H

#include "../include/includes.h"

int get_random_int(int);
void shuffle(char **, int);
void clean_input(char *);
void str_toupper(char *);
bool str_contains_char(char *, char);
void flush_stdin();
void abort_on_null_ptr(void *);

#endif