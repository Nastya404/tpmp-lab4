#ifndef MODELS_H
#define MODELS_H

typedef struct
{
    char number[21];
    char brand[101];
    int initial_mileage;
    int current_mileage;
    int capacity;
} Car;

typedef struct
{
    int id;
    char surname[101];
    char category[4];
    int experience;
    char address[201];
    int birth_year;
} Driver;

typedef struct
{
    int order_id;
    char date[11];
    int driver_id;
    char car_number[21];
    int distance;
    double cargo_mass;
    double cost;
} Order;

typedef struct
{
    int record_id;
    char start_date[11];
    char end_date[11];
    double total_salary;
    char calculated_at[20];
    int driver_id;
} SalaryRecord;

#endif /* MODELS_H */