#include "IO.h"
#include "funcs.h"
int count_letter(char *str, char ch);
void add_null_start_float(char *str, size_t *len_str);
int input_float(long_number_t *num, char num_str[41], size_t *len_num);
int input_int(long_number_t *num, char num_str[41]);
void find_mantissa_float(char *str, char mantissa[40]);
int find_exp(char *str, char *sing_e, int *exponent);
int only_zeros_before_dot(const char *str);
void add_sign_if_missing(char *str, size_t *len_str);
int validate_input_string(const char *str);

int parse_input(long_number_t *first_num, long_number_t *second_num, size_t *len_first_num, size_t *len_second_num)
{
    char first_num_str[42];
    char second_num_str[42];
    char entry;

    printf("Введите первое целое число для умножения в формате +-123\n");
    printf("+---------1---------2---------3---------4\n");

    if (scanf("%40s%c", first_num_str, &entry) != 2 || entry != '\n')
    {
        printf("fail scanf first num\n");
        return 1;
    }

    if (validate_input_string(first_num_str) != 0)
    {
        printf("Недопустимые символы в числе!\n");
        return 1;
    }

    *len_first_num = strlen(first_num_str);
    add_sign_if_missing(first_num_str, len_first_num);

    if (strchr(first_num_str, '.') == NULL && strchr(first_num_str, 'e') == NULL && strchr(first_num_str, 'E') == NULL)
    {
        if (input_int(first_num, first_num_str) != 0)
            return ERR_INT;
    }
    else
    {
        printf("Первое число должно быть целым!\n");
        return ERR_INPUT;
    }

    printf("Введите второе действительное число для умножения в формате +-123.123\n");
    printf("+---------1---------2---------3---------4\n");

    if (scanf("%40s%c", second_num_str, &entry) != 2 || entry != '\n')
    {
        printf("fail scanf second num\n");
        return 1;
    }

    if (validate_input_string(second_num_str) != 0)
    {
        printf("Недопустимые символы в числе!\n");
        return 1;
    }

    *len_second_num = strlen(second_num_str);
    add_sign_if_missing(second_num_str, len_second_num);

    if (strchr(second_num_str, '.') != NULL || strchr(second_num_str, 'e') != NULL || strchr(second_num_str, 'E') != NULL)
    {
        if (input_float(second_num, second_num_str, len_second_num) != 0)
            return ERR_FLOAT;
    }
    else
    {
        printf("Второе число должно быть вещественным!\n");
        return ERR_INPUT;
    }

    return 0;
}

int validate_input_string(const char *str)
{
    if (str == NULL)
        return -1;

    int i = 0;
    if (str[0] == '+' || str[0] == '-')
        i = 1;

    int dot_count = 0;
    int e_count = 0;

    for (; str[i] != '\0'; i++)
    {
        if (isdigit(str[i]))
            continue;
        else if (str[i] == '.')
        {
            dot_count++;
            if (dot_count > 1)
                return -1;
        }
        else if (str[i] == 'e' || str[i] == 'E')
        {
            e_count++;
            if (e_count > 1)
                return -1;

            if (str[i + 1] == '+' || str[i + 1] == '-')
            {
                i++;
                if (str[i + 1] == '\0')
                    return -1;
            }
        }
        else
            return -1;
    }

    return 0;
}

int input_int(long_number_t *num, char num_str[41])
{
    num->sign_m = num_str[0];

    size_t mantissa_len = strlen(num_str + 1);
    if (mantissa_len > 39)
    {
        printf("Ошибка, целое число должно быть до 40 цифр\n");
        return ERR_INT;
    }
    else
    {
        strncpy(num->mantissa, num_str + 1, 39);
        num->mantissa[39] = '\0';
    }
    if (strchr(num_str, '.') == NULL && strchr(num_str, 'e') == NULL && strchr(num_str, 'E') == NULL)
    {
        num->sign_e = '+';
        num->exponent = strlen(num->mantissa);
    }

    return 0;
}

int input_float(long_number_t *num, char num_str[41], size_t *len_num)
{
    num->sign_m = num_str[0];
    if (count_letter(num_str, '.') > 1 || count_letter(num_str, 'e') > 1 || count_letter(num_str, 'E') > 1)
    {
        printf("Ошибка в вводе действительного числа!\n");
        return ERR_FLOAT;
    }
    if ((strchr(num_str, '.') - num_str) == 1) // если нет нуля (+.0123)
    {
        add_null_start_float(num_str, len_num);
        find_mantissa_float(num_str, num->mantissa);
        if (find_exp(num_str, &num->sign_e, &num->exponent) != 0)
        {
            printf("Проверьте порядок числа!\n");
            return ERR_EXP;
        }
    }
    else
    {
        find_mantissa_float(num_str, num->mantissa);
        if (find_exp(num_str, &num->sign_e, &num->exponent) != 0)
        {
            printf("Проверьте порядок числа!\n");
            return ERR_EXP;
        }
    }
    return 0;
}

int count_letter(char *str, char ch)
{
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] == ch)
            count++;
    return count;
}

void add_null_start_float(char *str, size_t *len_str)
{
    (*len_str) += 2;
    char new_str[43];
    new_str[0] = str[0];
    new_str[1] = '0';
    strcat(new_str, (str) + 1);
    strcpy(str, new_str);
}

void find_mantissa_float(char *str, char mantissa[40])
{
    int j = 0;
    int found_non_zero = 0;

    int end_pos = strlen(str);
    char *e_pos = strchr(str, 'E');
    if (e_pos == NULL)
        e_pos = strchr(str, 'e');
    if (e_pos != NULL)
        end_pos = e_pos - str;

    for (int i = 0; i < end_pos && j < 39; i++)
        if (isdigit(str[i]) && j < 39)
            if (found_non_zero || str[i] != '0')
            {
                mantissa[j] = str[i];
                j++;
                found_non_zero = 1;
            }

    if (j == 0)
    {
        mantissa[0] = '0';
        j = 1;
    }

    if (j < 40)
        mantissa[j] = '\0';
    else
        mantissa[39] = '\0';
}

int find_exp(char *str, char *sign_e, int *exponent)
{
    char *e_pos = strchr(str, 'E');
    if (!e_pos)
        e_pos = strchr(str, 'e');

    int explicit_exp = 0;
    if (e_pos)
        explicit_exp = atoi(e_pos + 1);

    char *dot_pos = strchr(str, '.');

    int idx_first_digit = -1;
    for (int i = 0; str[i] != '\0' && str + i != e_pos; i++)
    {
        if (isdigit(str[i]) && str[i] != '0')
        {
            idx_first_digit = i;
            break;
        }
    }

    if (idx_first_digit == -1)
    {
        *sign_e = '+';
        *exponent = 0;
        return 0;
    }

    int exp_val = explicit_exp;

    if (exp_val > 99999 || exp_val < -99999)
        return ERR_EXP;

    if (dot_pos)
    {
        int dot_index = dot_pos - str;
        if (idx_first_digit < dot_index)
            exp_val += dot_index - idx_first_digit;
        else
            exp_val -= (idx_first_digit - dot_index - 1);
    }
    else
    {
        exp_val += (strlen(str) - idx_first_digit);
    }

    if (exp_val >= 0)
    {
        *sign_e = '+';
        *exponent = exp_val;
    }
    else
    {
        *sign_e = '-';
        *exponent = -exp_val;
    }

    return 0;
}

int only_zeros_before_dot(const char *str)
{
    char *dot_pos = strchr(str, '.');
    if (dot_pos == NULL)
        return 0;

    for (const char *p = str; p < dot_pos; p++)
        if (*p != '0' && *p != '+' && *p != '-')
            return 0;
    return 1;
}

void add_sign_if_missing(char *str, size_t *len_str)
{
    if (str[0] != '+' && str[0] != '-')
    {
        for (int i = *len_str; i >= 0; i--)
            str[i + 1] = str[i];
        str[0] = '+';
        (*len_str)++;
    }
}

void remove_trailing_zeros(char *mantissa)
{
    int len = strlen(mantissa);
    while (len > 1 && mantissa[len - 1] == '0')
    {
        mantissa[len - 1] = '\0';
        len--;
    }
}

void output_num(long_number_t result_num)
{
    remove_trailing_zeros(result_num.mantissa);
    printf("%c0.%s E %c%d\n", result_num.sign_m, result_num.mantissa, result_num.sign_e, result_num.exponent);
}
