#ifndef BUSINESS_LOGIC_H
#define BUSINESS_LOGIC_H

#include <sqlite3.h>
#include "models.h"

#define STATUS_OK 0
#define CARGO_LIMIT_EXCEEDED 1

int check_cargo_limit(sqlite3 *db, const char *car_number, double mass);
int process_new_order(sqlite3 *db, Order order);
int calculate_salaries_to_db(sqlite3 *db, const char *period); 

#endif
