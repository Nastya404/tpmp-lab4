#include <stdio.h>
#include "crud.h"

// --- Методы для CARS ---

int add_car(sqlite3 *db, Car car) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO CARS (number, brand, initial_mileage, current_mileage, capacity) VALUES (?, ?, ?, ?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, car.number, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, car.brand, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, car.initial_mileage);
    sqlite3_bind_int(stmt, 4, car.current_mileage);
    sqlite3_bind_int(stmt, 5, car.capacity);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

int update_car(sqlite3 *db, Car car) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE CARS SET brand=?, initial_mileage=?, current_mileage=?, capacity=? WHERE number=?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, car.brand, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, car.initial_mileage);
    sqlite3_bind_int(stmt, 3, car.current_mileage);
    sqlite3_bind_int(stmt, 4, car.capacity);
    sqlite3_bind_text(stmt, 5, car.number, -1, SQLITE_STATIC);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

int update_mileage(sqlite3 *db, const char *car_number, int new_val) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE CARS SET current_mileage=? WHERE number=?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, new_val);
    sqlite3_bind_text(stmt, 2, car_number, -1, SQLITE_STATIC);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

int delete_car(sqlite3 *db, const char *number) {
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM CARS WHERE number=?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, number, -1, SQLITE_STATIC);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

// --- Методы для DRIVERS ---

int add_driver(sqlite3 *db, Driver d) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO DRIVERS (personnel_number, surname, category, experience, address, birth_year) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, d.personnel_number);
    sqlite3_bind_text(stmt, 2, d.surname, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, d.category, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, d.experience);
    sqlite3_bind_text(stmt, 5, d.address, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, d.birth_year);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

int update_driver(sqlite3 *db, Driver d) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE DRIVERS SET surname=?, category=?, experience=?, address=?, birth_year=? WHERE personnel_number=?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, d.surname, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, d.category, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, d.experience);
    sqlite3_bind_text(stmt, 4, d.address, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, d.birth_year);
    sqlite3_bind_int(stmt, 6, d.personnel_number);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

int delete_driver(sqlite3 *db, int id) {
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM DRIVERS WHERE personnel_number=?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, id);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

// --- Методы для ORDERS ---

int add_order(sqlite3 *db, Order o) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO ORDERS (date, driver_id, car_number, distance, cargo_mass, cost) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, o.date, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, o.driver_id);
    sqlite3_bind_text(stmt, 3, o.car_number, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, o.distance);
    sqlite3_bind_double(stmt, 5, o.cargo_mass);
    sqlite3_bind_double(stmt, 6, o.cost);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

int update_order(sqlite3 *db, Order o) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE ORDERS SET date=?, driver_id=?, car_number=?, distance=?, cargo_mass=?, cost=? WHERE order_id=?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, o.date, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, o.driver_id);
    sqlite3_bind_text(stmt, 3, o.car_number, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, o.distance);
    sqlite3_bind_double(stmt, 5, o.cargo_mass);
    sqlite3_bind_double(stmt, 6, o.cost);
    sqlite3_bind_int(stmt, 7, o.order_id);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

int delete_order(sqlite3 *db, int id) {
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM ORDERS WHERE order_id=?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, id);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 0 : -1;
}
