#include "funcs.h"
#include "IO.h"

#define BASE 10000 // Основание системы счисления (4 цифры)
#define MAX_BLOCKS 10

// нормализация мантиссы к формату 0.xxxxx
void normalize_mantissa(char *mantissa, int *exponent, char *sign_e)
{
    int start = 0;
    while (mantissa[start] == '0' && mantissa[start + 1] != '\0')
        start++;
    if (start > 0)
    {
        memmove(mantissa, mantissa + start, strlen(mantissa) - start + 1);
        if (*sign_e == '+')
            *exponent -= start;
        else
            *exponent += start;
    }
}

void string_to_blocks(const char *str, int blocks[], int *num_blocks)
{
    int len = strlen(str);
    *num_blocks = 0;

    for (int i = len - 1; i >= 0; i -= 4)
    {
        int block = 0;
        int power = 1;

        for (int j = 0; j < 4; j++)
        {
            int pos = i - j;
            if (pos >= 0 && isdigit(str[pos]))
            {
                block += (str[pos] - '0') * power;
                power *= 10;
            }
        }

        blocks[(*num_blocks)++] = block;
    }
}

void multiply_blocks(const int a[], int a_len, const int b[], int b_len, int result[], int *result_len)
{
    *result_len = a_len + b_len;
    for (int i = 0; i < *result_len; i++)
        result[i] = 0;
    for (int i = 0; i < a_len; i++)
    {
        int carry = 0;
        for (int j = 0; j < b_len; j++)
        {
            int temp = a[i] * b[j] + result[i + j] + carry;
            result[i + j] = temp % BASE;
            carry = temp / BASE;
        }
        result[i + b_len] = carry;
    }

    while (*result_len > 1 && result[*result_len - 1] == 0)
        (*result_len)--;
}

void blocks_to_string(const int blocks[], int num_blocks, char *result)
{
    int pos = 0;
    pos += sprintf(result + pos, "%d", blocks[num_blocks - 1]);
    for (int i = num_blocks - 2; i >= 0; i--)
        pos += sprintf(result + pos, "%04d", blocks[i]); // 04d гарантирует 4 цифры
    result[pos] = '\0';
}

int multiply_long_numbers(const long_number_t *a, const long_number_t *b, long_number_t *result)
{
    result->sign_m = (a->sign_m == b->sign_m) ? '+' : '-';

    if (strcmp(a->mantissa, "0") == 0 || strcmp(b->mantissa, "0") == 0)
    {
        strcpy(result->mantissa, "0");
        result->exponent = 0;
        result->sign_e = '+';
        return 0;
    }
    long_number_t a_norm = *a;
    long_number_t b_norm = *b;

    normalize_mantissa(a_norm.mantissa, &a_norm.exponent, &a_norm.sign_e);
    normalize_mantissa(b_norm.mantissa, &b_norm.exponent, &b_norm.sign_e);

    int a_blocks[MAX_BLOCKS], b_blocks[MAX_BLOCKS], res_blocks[MAX_BLOCKS * 2];
    int a_len, b_len, res_len;

    string_to_blocks(a_norm.mantissa, a_blocks, &a_len);
    string_to_blocks(b_norm.mantissa, b_blocks, &b_len);

    multiply_blocks(a_blocks, a_len, b_blocks, b_len, res_blocks, &res_len);

    char temp_result[80] = {0};
    blocks_to_string(res_blocks, res_len, temp_result);

    int exp_a = (a_norm.sign_e == '+') ? a_norm.exponent : -a_norm.exponent;
    int exp_b = (b_norm.sign_e == '+') ? b_norm.exponent : -b_norm.exponent;

    int len_a = strlen(a_norm.mantissa);
    int len_b = strlen(b_norm.mantissa);
    int len_res = strlen(temp_result);

    int total_exp = exp_a + exp_b + (len_res - len_a - len_b);

    if (total_exp >= 0)
    {
        result->sign_e = '+';
        result->exponent = total_exp;
    }
    else
    {
        result->sign_e = '-';
        result->exponent = -total_exp;
    }

    if (len_res > 40)
    {
        strncpy(result->mantissa, temp_result, 40);
        result->mantissa[40] = '\0';
        len_res = 40;
    }
    else
        strcpy(result->mantissa, temp_result);

    normalize_mantissa(result->mantissa, &result->exponent, &result->sign_e);
    if (result->exponent > 99999 || result->exponent < -99999)
    {
        printf("Ошибка большой порядок в ответе!\n");
        return ERR_EXP;
    }

    return 0;
}
