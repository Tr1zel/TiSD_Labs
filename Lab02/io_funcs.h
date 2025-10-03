#ifndef IO_FUNCS_H
#define IO_FUNCS_H
#include "struct.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_args(int *choice);
void print_table_for_keys(country *countries, key *keys, int count);
int print_findest_struct_for_task(country *countries, int count);
void print_countries_table(country *countries, int count);
void print_keys_table(key *keys, int count);

#endif
