#include "table_funcs.h"
#include "errors.h"
#include "struct.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

char *my_strdup(const char *str)
{
    size_t len = strlen(str);
    char *copy = malloc(len + 1);
    if (copy)
        strcpy(copy, str);
    return copy;
}

int is_alpha_or_space_only(const char *str)
{
    if (str == NULL || *str == '\0')
        return 0; // Пустая строка — считаем недопустимой

    for (int i = 0; str[i] != '\0'; i++)
        if (!isalpha((unsigned char)str[i]) && !isspace((unsigned char)str[i]))
            return 0; // Найден недопустимый символ
    return 1;         // Строка корректна
}

// Функция парсинга одной строки
int parse_line(const char *line, country *c)
{
    char line_copy[MAX_LINE_LENGTH];
    strncpy(line_copy, line, sizeof(line_copy) - 1);
    line_copy[sizeof(line_copy) - 1] = '\0';

    char *token = strtok(line_copy, "|");
    if (!token)
        return -1;
    c->country_name = my_strdup(token);

    token = strtok(NULL, "|");
    if (!token)
    {
        free(c->country_name);
        return -1;
    }
    c->capital = my_strdup(token);

    token = strtok(NULL, "|");
    if (!token)
    {
        free(c->country_name);
        free(c->capital);
        return -1;
    }
    c->continent = my_strdup(token);

    token = strtok(NULL, "|");
    if (!token)
    {
        free(c->country_name);
        free(c->capital);
        free(c->continent);
        return -1;
    }
    c->visa_avaibility = atoi(token);

    token = strtok(NULL, "|");
    if (!token)
    {
        free(c->country_name);
        free(c->capital);
        free(c->continent);
        return -1;
    }
    c->time_to_flight = atoi(token);

    token = strtok(NULL, "|");
    if (!token)
    {
        free(c->country_name);
        free(c->capital);
        free(c->continent);
        return -1;
    }
    c->min_cost = atoi(token);

    token = strtok(NULL, "|");
    if (!token)
    {
        free(c->country_name);
        free(c->capital);
        free(c->continent);
        return -1;
    }

    char tourism_type[32];
    strcpy(tourism_type, token);

    token = strtok(NULL, "|");
    if (!token)
    {
        free(c->country_name);
        free(c->capital);
        free(c->continent);
        return -1;
    }

    // Парсим поля объединения
    if (strcmp(tourism_type, "excursion") == 0)
    {
        c->type = EXCURSION;
        char *field1 = strtok(token, ",");
        char *field2 = strtok(NULL, ",");
        if (!field1 || !field2)
        {
            free(c->country_name);
            free(c->capital);
            free(c->continent);
            return -1;
        }
        c->kind_tourism.excursion.count_object = atoi(field1);
        c->kind_tourism.excursion.main_object = my_strdup(field2);
    }
    else if (strcmp(tourism_type, "beach") == 0)
    {
        c->type = BEACH;
        char *field1 = strtok(token, ",");
        char *field2 = strtok(NULL, ",");
        if (!field1 || !field2)
        {
            free(c->country_name);
            free(c->capital);
            free(c->continent);
            return -1;
        }
        c->kind_tourism.beach.main_season = my_strdup(field1);
        c->kind_tourism.beach.temperature = atoi(field2);
    }
    else if (strcmp(tourism_type, "sports") == 0)
    {
        c->type = SPORTS;
        char *field1 = strtok(token, ",");
        if (!field1)
        {
            free(c->country_name);
            free(c->capital);
            free(c->continent);
            return -1;
        }
        c->kind_tourism.sports.kind_sport = my_strdup(field1);
    }
    else
    {
        free(c->country_name);
        free(c->capital);
        free(c->continent);
        return -1;
    }

    return 0;
}

void table_free(country *countries, int count)
{
    for (int i = 0; i < count; i++)
    {
        free(countries[i].country_name);
        free(countries[i].capital);
        free(countries[i].continent);
        // Освобождение полей union
        if (countries[i].type == EXCURSION)
        {
            free(countries[i].kind_tourism.excursion.main_object);
        }
        else if (countries[i].type == BEACH)
        {
            free(countries[i].kind_tourism.beach.main_season);
        }
        else if (countries[i].type == SPORTS)
        {
            free(countries[i].kind_tourism.sports.kind_sport);
        }
    }
    free(countries);
}

int parse_countries_file(FILE **f, country **countries, int *count)
{
    *count = 0;
    int capacity = 10; // Начальный размер
    *countries = malloc(capacity * sizeof(country));
    if (!*countries)
    {
        fclose(*f);
        return ERR_MEMORY;
    }

    char line[MAX_LINE_LENGTH];
    int i = 0;
    while (fgets(line, sizeof(line), *f))
    {
        // Убираем символ перевода строки
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }

        if (parse_line(line, &(*countries)[i]) == 0)
        {
            i++;

            // Если достигли лимита, расширяем массив
            if (i >= capacity)
            {
                capacity *= 2;
                country *temp = realloc(*countries, capacity * sizeof(country));
                if (!temp)
                {
                    table_free(*countries, i); // Освобождаем уже прочитанное
                    fclose(*f);
                    return ERR_MEMORY;
                }
                *countries = temp;
            }
        }
        else
        {
            table_free(*countries, i);
            fclose(*f);
            return -3; // ошибка парсинга
        }
    }

    *count = i;
    fclose(*f);
    return 0;
}

int create_keys_table(country *countries, key **keys, int count)
{
    *keys = malloc(count * sizeof(key));
    if (!*keys)
    {
        printf("Ошибка выделения памяти!\n");
        return ERR_MEMORY;
    }

    for (int i = 0; i < count; i++)
    {
        (*keys)[i].index = i;
        (*keys)[i].min_cost = countries[i].min_cost;
    }
    return 0;
}

int add_line_end_in_table(country **countries, int *count)
{
    country temp_country;
    char buf[256];

    while (getchar() != '\n')
        ;

    printf("Введите название страны: ");
    if (scanf("%s", buf) != 1 || is_alpha_or_space_only(buf) == 0)
    {
        printf("Ошибка ввода страны!\n");
        return ERR_INPUT;
    }
    temp_country.country_name = my_strdup(buf);

    printf("Введите столицу страны: ");
    if (scanf("%s", buf) != 1 || is_alpha_or_space_only(buf) == 0)
    {
        printf("Ошибка ввода столицы!\n");
        free(temp_country.country_name);
        return ERR_INPUT;
    }
    temp_country.capital = my_strdup(buf);

    printf("Введите материк страны: ");
    if (scanf("%s", buf) != 1 || is_alpha_or_space_only(buf) == 0)
    {
        printf("Ошибка ввода материка!\n");
        free(temp_country.country_name);
        free(temp_country.capital);
        return ERR_INPUT;
    }
    temp_country.continent = my_strdup(buf);

    printf("Введите необходимость наличие визы (0/1): ");
    if (scanf("%d", &temp_country.visa_avaibility) != 1)
    {
        printf("Ошибка ввода визы!\n");
        free(temp_country.country_name);
        free(temp_country.capital);
        free(temp_country.continent);
        return ERR_INPUT;
    }

    printf("Введите время полета в часах: ");
    if (scanf("%d", &temp_country.time_to_flight) != 1)
    {
        printf("Ошибка ввода времени полета!\n");
        free(temp_country.country_name);
        free(temp_country.capital);
        free(temp_country.continent);
        return ERR_INPUT;
    }

    printf("Введите минимальную стоимость отдыха: ");
    if (scanf("%d", &temp_country.min_cost) != 1)
    {
        printf("Ошибка ввода минимальной стоимости!\n");
        free(temp_country.country_name);
        free(temp_country.capital);
        free(temp_country.continent);
        return ERR_INPUT;
    }

    printf("Введите вид туризма (excursion/beach/sports): ");
    if (scanf("%s", buf) != 1 || is_alpha_or_space_only(buf) == 0)
    {
        printf("Ошибка ввода вида туризма!\n");
        free(temp_country.country_name);
        free(temp_country.capital);
        free(temp_country.continent);
        return ERR_INPUT;
    }

    if (strcmp(buf, "excursion") == 0)
    {
        temp_country.type = EXCURSION;
        printf("Введите количество объектов: ");
        if (scanf("%d", &temp_country.kind_tourism.excursion.count_object) != 1)
        {
            printf("Ошибка ввода!\n");
            free(temp_country.country_name);
            free(temp_country.capital);
            free(temp_country.continent);
            return ERR_INPUT;
        }
        printf("Введите основной объект (history/art/nature/culture): ");
        if (scanf("%s", buf) != 1 || is_alpha_or_space_only(buf) == 0)
        {
            printf("Ошибка ввода!\n");
            free(temp_country.country_name);
            free(temp_country.capital);
            free(temp_country.continent);
            return ERR_INPUT;
        }
        temp_country.kind_tourism.excursion.main_object = my_strdup(buf);
    }
    else if (strcmp(buf, "beach") == 0)
    {
        temp_country.type = BEACH;
        printf("Введите сезон (summer/winter/year-round): ");
        if (scanf("%s", buf) != 1 || is_alpha_or_space_only(buf) == 0)
        {
            printf("Ошибка ввода!\n");
            free(temp_country.country_name);
            free(temp_country.capital);
            free(temp_country.continent);
            return ERR_INPUT;
        }
        temp_country.kind_tourism.beach.main_season = my_strdup(buf);
        printf("Введите температуру: ");
        if (scanf("%d", &temp_country.kind_tourism.beach.temperature) != 1)
        {
            printf("Ошибка ввода!\n");
            free(temp_country.country_name);
            free(temp_country.capital);
            free(temp_country.continent);
            free(temp_country.kind_tourism.beach.main_season);
            return ERR_INPUT;
        }
    }
    else if (strcmp(buf, "sports") == 0)
    {
        temp_country.type = SPORTS;
        printf("Введите вид спорта: ");
        if (scanf("%s", buf) != 1 || is_alpha_or_space_only(buf) == 0)
        {
            printf("Ошибка ввода!\n");
            free(temp_country.country_name);
            free(temp_country.capital);
            free(temp_country.continent);
            return ERR_INPUT;
        }
        temp_country.kind_tourism.sports.kind_sport = my_strdup(buf);
    }
    else
    {
        printf("Неверный вид туризма! Доступны: excursion, beach, sports\n");
        free(temp_country.country_name);
        free(temp_country.capital);
        free(temp_country.continent);
        return ERR_INPUT;
    }

    country *temp = realloc(*countries, (*count + 1) * sizeof(country));
    if (!temp)
    {
        printf("Ошибка выделения памяти!\n");
        return ERR_MEMORY;
    }
    *countries = temp;
    (*countries)[*count] = temp_country;
    (*count)++;
    printf("\nУспешно добавлено\n");

    return 0;
}

int delete_by_field(country **countries, int *count)
{
    if (*count == 0)
    {
        printf("Таблица пуста!\n");
        return ERR_INPUT;
    }

    while (getchar() != '\n')
        ;

    int field_num;
    printf("Выберите поле для удаления:\n");
    printf("0 - Название страны\n");
    printf("1 - Столица\n");
    printf("2 - Материк\n");
    printf("3 - Виза (0/1)\n");
    printf("4 - Время полета\n");
    printf("5 - Минимальная стоимость\n");
    printf("6 - Вид туризма\n");
    printf("Введите номер поля: ");
    if (scanf("%d", &field_num) != 1 || field_num < 0 || field_num > 6)
    {
        printf("Неверный номер поля!\n");
        return ERR_INPUT;
    }

    char value[256];
    int int_value = 0;

    if (field_num >= 3 && field_num <= 5)
    {
        printf("Введите значение для удаления: ");
        if (scanf("%d", &int_value) != 1)
        {
            printf("Ошибка ввода!\n");
            return ERR_INPUT;
        }
    }
    else
    {
        printf("Введите значение для удаления: ");
        if (scanf("%s", value) != 1)
        {
            printf("Ошибка ввода!\n");
            return ERR_INPUT;
        }
        for (int i = 0; value[i]; i++)
            value[i] = tolower(value[i]);
    }

    int deleted = 0;
    for (int i = 0; i < *count; i++)
    {
        char temp[256];
        int match = 0;

        if (field_num == 0)
        {
            strcpy(temp, (*countries)[i].country_name);
            for (int j = 0; temp[j]; j++)
                temp[j] = tolower(temp[j]);
            match = (strcmp(temp, value) == 0);
        }
        else if (field_num == 1)
        {
            strcpy(temp, (*countries)[i].capital);
            for (int j = 0; temp[j]; j++)
                temp[j] = tolower(temp[j]);
            match = (strcmp(temp, value) == 0);
        }
        else if (field_num == 2)
        {
            strcpy(temp, (*countries)[i].continent);
            for (int j = 0; temp[j]; j++)
                temp[j] = tolower(temp[j]);
            match = (strcmp(temp, value) == 0);
        }
        else if (field_num == 3)
        {
            match = ((*countries)[i].visa_avaibility == int_value);
        }
        else if (field_num == 4)
        {
            match = ((*countries)[i].time_to_flight == int_value);
        }
        else if (field_num == 5)
        {
            match = ((*countries)[i].min_cost == int_value);
        }
        else if (field_num == 6)
        {
            if (strcmp(value, "excursion") == 0)
                match = ((*countries)[i].type == EXCURSION);
            else if (strcmp(value, "beach") == 0)
                match = ((*countries)[i].type == BEACH);
            else if (strcmp(value, "sports") == 0)
                match = ((*countries)[i].type == SPORTS);
        }

        if (match)
        {
            // Освобождаем память удаляемой записи
            free((*countries)[i].country_name);
            free((*countries)[i].capital);
            free((*countries)[i].continent);
            if ((*countries)[i].type == EXCURSION)
                free((*countries)[i].kind_tourism.excursion.main_object);
            else if ((*countries)[i].type == BEACH)
                free((*countries)[i].kind_tourism.beach.main_season);
            else if ((*countries)[i].type == SPORTS)
                free((*countries)[i].kind_tourism.sports.kind_sport);

            // Сдвигаем элементы
            for (int j = i; j < *count - 1; j++)
                (*countries)[j] = (*countries)[j + 1];

            (*count)--;
            i--;
            deleted++;
        }
    }

    if (deleted > 0)
    {
        printf("\nУдалено записей: %d\n", deleted);
        return 0;
    }
    else
    {
        printf("\nЗаписи не найдены!\n");
        return ERR_INPUT;
    }
}

void sort_table_keys(key **keys, int count)
{
    for (int i = 0; i < count; i++)
        for (int j = 0; j < count - i - 1; j++)
            if ((*keys)[j].min_cost > (*keys)[j + 1].min_cost)
            {
                key temp = (*keys)[j];
                (*keys)[j] = (*keys)[j + 1];
                (*keys)[j + 1] = temp;
            }
}

void sort_table_countries(country **countries, int count)
{
    for (int i = 0; i < count; i++)
        for (int j = 0; j < count - i - 1; j++)
            if ((*countries)[j].min_cost > (*countries)[j + 1].min_cost)
            {
                country temp = (*countries)[j];
                (*countries)[j] = (*countries)[j + 1];
                (*countries)[j + 1] = temp;
            }
}
