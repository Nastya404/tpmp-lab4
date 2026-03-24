#ifndef BUSINESS_LOGIC_H
#define BUSINESS_LOGIC_H

#include "models.h"

// Коды возврата
#define STATUS_OK 0
#define CARGO_LIMIT_EXCEEDED 1
#define DB_ERROR -1
#define NOT_FOUND -2


int check_cargo_limit(const char* car_number, float mass);
int process_new_order(Order order);
int calculate_salaries_to_db(const char* start_date, const char* end_date);

#endif // BUSINESS_LOGIC_H
