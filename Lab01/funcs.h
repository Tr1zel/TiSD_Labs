#ifndef FUNCS_H
#define FUNCS_H
#include "IO.h"
int multiply_long_numbers(const long_number_t *a, const long_number_t *b, long_number_t *result);
void normalize_mantissa(char *mantissa, int *exponent, char *sign_e);

#endif
