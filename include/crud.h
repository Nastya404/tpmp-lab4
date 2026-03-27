#ifndef CRUD_H
#define CRUD_H

#include <sqlite3.h>
#include "models.h"

// Полное соответствие блоку Repository
int add_driver(sqlite3 *db, Driver driver);
int add_car(sqlite3 *db, Car car);
int add_order(sqlite3 *db, Order order);

int update_car(sqlite3 *db, Car car);
int update_driver(sqlite3 *db, Driver driver);
int update_order(sqlite3 *db, Order order);
int update_mileage(sqlite3 *db, const char *car_number, int new_val);

int delete_order(sqlite3 *db, int id);
int delete_car(sqlite3 *db, const char *number);
int delete_driver(sqlite3 *db, int id);

// Доп. метод для поиска (нужен для BusinessLogic согласно Sequence Diagram)
int get_car_by_number(sqlite3 *db, const char *number, Car *car);
int authenticate_user(sqlite3 *db, const char *login, const char *password, User *out_user);

#endif
