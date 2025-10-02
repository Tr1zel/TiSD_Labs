#ifndef STRUCT_H
#define STRUCT_H

typedef enum
{
    EXCURSION,
    BEACH,
    SPORTS
} tourism_type;

typedef struct
{
    char *country_name;
    char *capital;       // столица
    char *continent;     // материк
    int visa_avaibility; // наличие визы [0,1]
    int time_to_flight;
    int min_cost;      // минимальная цена отдыха
    tourism_type type; // отдельное поле для хранени ТИПА туризма
    union {
        struct
        {
            int count_object;
            char *main_object; // Основной вид объектов (природа, искусство, история)
        } excursion;
        struct
        {
            char *main_season;
            int temperature;
        } beach;
        struct
        {
            char *kind_sport; // вид спорта
        } sports;
    } kind_tourism;
} country;

typedef struct
{
    int index;
    int min_cost; // главный ключ - минимальная стоимость отдыха
} key;

#endif
