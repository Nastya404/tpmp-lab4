#ifndef CRUD_H
#define CRUD_H

#include <sqlite3.h>
#include "models.h"

// --- CARS ---
int add_car(sqlite3 *db, Car car);
int update_car(sqlite3 *db, Car car);
int update_mileage(sqlite3 *db, const char *car_number, int new_val);
int delete_car(sqlite3 *db, const char *number);

// --- DRIVERS ---
int add_driver(sqlite3 *db, Driver driver);
int update_driver(sqlite3 *db, Driver driver);
int delete_driver(sqlite3 *db, int id); // id = personnel_number

// --- ORDERS ---
int add_order(sqlite3 *db, Order order);
int update_order(sqlite3 *db, Order order);
int delete_order(sqlite3 *db, int id); // id = order_id

#endif
