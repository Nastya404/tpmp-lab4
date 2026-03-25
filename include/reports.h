#ifndef REPORTS_H
#define REPORTS_H

#include <sqlite3.h>

int get_driver_orders(sqlite3 *db, int driver_id, const char *start_date, const char *end_date);
int get_driver_salary_info(sqlite3 *db, const char *surname, const char *start_date, const char *end_date);
int get_max_mileage_car(sqlite3 *db);
int get_lazy_driver_info(sqlite3 *db);
int get_car_statistics(sqlite3 *db, const char *car_number);
int get_driver_statistics(sqlite3 *db, int driver_id);
int get_all_drivers_statistics(sqlite3 *db);
int get_salary_records(sqlite3 *db, const char *start_date, const char *end_date);

#endif
