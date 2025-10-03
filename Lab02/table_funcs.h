#ifndef TABLE_FUNCS_H
#define TABLE_FUNCS_H
#include "struct.h"
#include <stdio.h>

void table_free(country *countries, int count);
int parse_countries_file(FILE **f, country **countries, int *count);
int add_line_end_in_table(country **countries, int *count);
int delete_by_field(country **countries, int *count);
int create_keys_table(country *counries, key **keys, int count);
void sort_table_keys(key **keys, int count);
void sort_table_countries(country **countries, int count);
int is_alpha_or_space_only(const char *str);
char *my_strdup(const char *str);

#endif
