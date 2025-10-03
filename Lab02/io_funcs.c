#include "io_funcs.h"
#include "errors.h"
#include "struct.h"
#include "table_funcs.h"

#define MAX_LINE_LENGTH 1024

int parse_args(int *choice)
{
    printf("=== Главное меню ===\n"
           "1 - Добавить запись в конец таблицы\n"
           "2 - Удалить записи по полю (Введите номер поля и значение)\n"
           "3 - Показать всю таблицу стран\n"
           "4 - Показать всю таблицу ключей (ключ = мин цена отдыха)\n"
           "5 - Отсортировать таблицу по цене отдыха\n"
           "6 - Отсортировать таблицу ключей по цене отдыха\n"
           "7 - Показать таблицу в порядке ключей (оригинал не меняется)\n"
           "8 - Поиск (по заданию)\n"
           "9 - Провести замеры (время сортировок и память)\n"
           "0 - Выход\n"
           "Ваш выбор: ");
    if (scanf("%d", choice) != 1 || *choice < 0 || *choice > 9)
    {
        printf("Неправильный пункт меню!\n");
        return ERR_MENU;
    }
    return 0;
}

void print_table_for_keys(country *countries, key *keys, int count)
{
    printf("%-15s %-15s %-15s %-5s %-6s %-8s %-12s %-15s\n", "Country", "Capital", "Continent", "Visa", "Flight",
           "Min Cost", "Tour Type", "Details");
    printf("%-15s %-15s %-15s %-5s %-6s %-8s %-12s %-15s\n", "-------", "-------", "---------", "----", "------",
           "--------", "--------", "-------");

    for (int i = 0; i < count; i++)
    {
        int index = keys[i].index;
        const char *tourism_type = "";
        char details[64] = "";

        // Проверяем тип туризма и читаем соответствующие поля
        if (countries[index].type == EXCURSION)
        {
            tourism_type = "Excursion";
            snprintf(details, sizeof(details), "%d obj, %s", countries[index].kind_tourism.excursion.count_object,
                     countries[index].kind_tourism.excursion.main_object);
        }
        else if (countries[index].type == BEACH)
        {
            tourism_type = "Beach";
            snprintf(details, sizeof(details), "%s, %dC", countries[index].kind_tourism.beach.main_season,
                     countries[index].kind_tourism.beach.temperature);
        }
        else if (countries[index].type == SPORTS)
        {
            tourism_type = "Sports";
            snprintf(details, sizeof(details), "%s", countries[index].kind_tourism.sports.kind_sport);
        }

        printf("%-15s %-15s %-15s %-5s %-6d %-8d %-12s %-15s\n", countries[index].country_name,
               countries[index].capital, countries[index].continent, countries[index].visa_avaibility ? "Yes" : "No",
               countries[index].time_to_flight, countries[index].min_cost, tourism_type, details);
    }
}

int print_findest_struct_for_task(country *countries, int count)
{
    country temp_country = {0};
    char buf[100];
    printf("Введите материк для поиска: ");
    if (scanf("%s", buf) != 1 || is_alpha_or_space_only(buf) == 0)
    {
        printf("Ошибка ввода материка!\n");
        return ERR_INPUT;
    }
    temp_country.type = SPORTS;
    temp_country.continent = my_strdup(buf);

    printf("Введите вид спорта для поиска: ");
    if (scanf("%s", buf) != 1 || is_alpha_or_space_only(buf) == 0)
    {
        printf("Ошибка ввода вида спорта!\n");
        free(temp_country.country_name);
        free(temp_country.capital);
        return ERR_INPUT;
    }
    temp_country.kind_tourism.sports.kind_sport = my_strdup(buf);

    printf("Введите минимальную стоимость отдыха для поиска: ");
    if (scanf("%d", &temp_country.min_cost) != 1)
    {
        printf("Ошибка ввода минимальной стоимости!\n");
        free(temp_country.country_name);
        free(temp_country.capital);
        free(temp_country.continent);
        return ERR_INPUT;
    }
    int flag = 0;
    for (int i = 0; i < count; i++)
    {
        if (strcmp(countries[i].continent, temp_country.continent) == 0 &&
            (countries[i].type == SPORTS &&
             strcmp(countries[i].kind_tourism.sports.kind_sport, temp_country.kind_tourism.sports.kind_sport) == 0) &&
            countries[i].min_cost < temp_country.min_cost)
        {
            flag = 1;
            printf("%-15s %-15s %-15s %-5s %-6d %-8d %-12s %-15s\n", countries[i].country_name, countries[i].capital,
                   countries[i].continent, countries[i].visa_avaibility ? "Yes" : "No", countries[i].time_to_flight,
                   countries[i].min_cost, "Sports", countries[i].kind_tourism.sports.kind_sport);
        }
    }
    if (flag == 0) printf("Искомые записи не найдены!\n");
    return 0;
}

void print_countries_table(country *countries, int count)
{
    printf("%-15s %-15s %-15s %-5s %-6s %-8s %-12s %-15s\n", "Country", "Capital", "Continent", "Visa", "Flight",
           "Min Cost", "Tour Type", "Details");
    printf("%-15s %-15s %-15s %-5s %-6s %-8s %-12s %-15s\n", "-------", "-------", "---------", "----", "------",
           "--------", "--------", "-------");

    for (int i = 0; i < count; i++)
    {
        const char *tourism_type = "";
        char details[64] = "";

        // Проверяем тип туризма и читаем соответствующие поля
        if (countries[i].type == EXCURSION)
        {
            tourism_type = "Excursion";
            snprintf(details, sizeof(details), "%d obj, %s", countries[i].kind_tourism.excursion.count_object,
                     countries[i].kind_tourism.excursion.main_object);
        }
        else if (countries[i].type == BEACH)
        {
            tourism_type = "Beach";
            snprintf(details, sizeof(details), "%s, %dC", countries[i].kind_tourism.beach.main_season,
                     countries[i].kind_tourism.beach.temperature);
        }
        else if (countries[i].type == SPORTS)
        {
            tourism_type = "Sports";
            snprintf(details, sizeof(details), "%s", countries[i].kind_tourism.sports.kind_sport);
        }

        printf("%-15s %-15s %-15s %-5s %-6d %-8d %-12s %-15s\n", countries[i].country_name, countries[i].capital,
               countries[i].continent, countries[i].visa_avaibility ? "Yes" : "No", countries[i].time_to_flight,
               countries[i].min_cost, tourism_type, details);
    }
}

void print_keys_table(key *keys, int count)
{
    printf("%-15s %-15s\n", "Index", "Min_cost");
    printf("%-15s %-15s\n", "-------", "-------");
    for (int i = 0; i < count; i++)
    {
        printf("%-15d %-15d\n", keys[i].index, keys[i].min_cost);
    }
}
