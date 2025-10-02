#ifndef TABLE_FUNCS_H
#define TABLE_FUNCS_H
#include "struct.h"
#include <stdio.h>

void print_countries_table(country *countries, int count);
void table_free(country *countries, int count);
int parse_countries_file(FILE **f, country **countries, int *count);
int add_line_end_in_table(country **countries, int *count);
int delete_by_field(country **countries, int *count);
int create_keys_table(country *counries, key **keys, int count);
void print_keys_table(key *keys, int count);
void sort_table_keys(key **keys, int count);
void sort_table_countries(country **countries, int count);

#endif
