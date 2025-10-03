/*
Ввести список стран, содержащий название страны, столицу, материк,
необходимость наличия визы, время полета до страны, минимальную
стоимость отдыха, основной вид туризма:
1. Экскурсионный:
    a. Количество объектов
    b. Основной вид объектов (природа, искусство, история)
2. Пляжный:
    a. Основной сезон
    b. Температура воздуха и воды
3. Спортивный:
    a. Вид спорта (горные лыжи, серфинг, восхождения)
Вывести список стран на выбранном материке, где можно заняться
указанным видом спорта, со стоимостью отдыха меньше указанной
*/

/*
1 cтруктура
внутри которой есть основные поля: содержащий название страны, столицу, материк,
необходимость наличия визы, время полета до страны, минимальную
стоимость отдыха
и 1 обьединение которое содержит 3 структуры: (Экскурсионный Пляжный Спортивный)
*/

#include "errors.h"
#include "io_funcs.h"
#include "struct.h"
#include "table_funcs.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Недостаточно аргументов!\n");
        return ERR_ARGS;
    }

    FILE *in_file = fopen(argv[1], "r");
    if (in_file == NULL)
    {
        printf("Файла не существует!\n");
        return ERR_FILE;
    }

    country *countries = NULL;
    key *keys = NULL;
    int count_data = 0;
    parse_countries_file(&in_file, &countries, &count_data);
    create_keys_table(countries, &keys, count_data);
    int choice = -1;
    while (choice != 0)
    {
        if (parse_args(&choice) != 0)
            return ERR_MENU;

        if (choice == 0)
        {
            table_free(countries, count_data);
            return 0;
        }
        else
        {
            if (choice == 1)
            {
                int rc = add_line_end_in_table(&countries, &count_data);
                if (rc != 0)
                    return rc;
            }
            if (choice == 2)
            {
                int rc = delete_by_field(&countries, &count_data);
                if (rc != 0)
                    return rc;
            }
            if (choice == 3)
                print_countries_table(countries, count_data);
            if (choice == 4)
            {
                print_keys_table(keys, count_data);
            }
            if (choice == 5)
            {
                sort_table_countries(&countries, count_data);
            }
            if (choice == 6)
            {
                sort_table_keys(&keys, count_data);
            }
            if (choice == 7)
            {
                print_table_for_keys(countries, keys, count_data);
            }
            if (choice == 8)
            {
                print_findest_struct_for_task(countries, count_data);
            }
        }
    }

    table_free(countries, count_data);
    return 0;
}
