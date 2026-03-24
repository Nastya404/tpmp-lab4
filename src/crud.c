#include <stdio.h>
#include <string.h>
#include "crud.h"

// --- Репозиторий (CRUD операции) ---

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

int add_car(sqlite3 *db, Car c) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO CARS (number, brand, initial_mileage, current_mileage, capacity) VALUES (?, ?, ?, ?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, c.number, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, c.brand, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, c.initial_mileage);
    sqlite3_bind_int(stmt, 4, c.current_mileage);
    sqlite3_bind_int(stmt, 5, c.capacity);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

int add_order(sqlite3 *db, Order o) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO ORDERS (date, driver_id, car_number, cargo_mass, cost, distance) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, o.date, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, o.driver_id);
    sqlite3_bind_text(stmt, 3, o.car_number, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, o.cargo_mass);
    sqlite3_bind_double(stmt, 5, o.cost);
    sqlite3_bind_int(stmt, 6, o.distance);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 0 : -1;
}

int update_car(sqlite3 *db, Car c) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE CARS SET brand=?, initial_mileage=?, current_mileage=?, capacity=? WHERE number=?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, c.brand, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, c.initial_mileage);
    sqlite3_bind_int(stmt, 3, c.current_mileage);
    sqlite3_bind_int(stmt, 4, c.capacity);
    sqlite3_bind_text(stmt, 5, c.number, -1, SQLITE_STATIC);
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

int update_order(sqlite3 *db, Order o) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE ORDERS SET cargo_mass = ?, cost = ?, distance = ? "
                      "WHERE date = ? AND driver_id = ? AND car_number = ?;";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }

    sqlite3_bind_double(stmt, 1, o.cargo_mass);
    sqlite3_bind_double(stmt, 2, o.cost);
    sqlite3_bind_int(stmt, 3, o.distance);
    sqlite3_bind_text(stmt, 4, o.date, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, o.driver_id);
    sqlite3_bind_text(stmt, 6, o.car_number, -1, SQLITE_STATIC);

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

int delete_order(sqlite3 *db, int id) {
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM ORDERS WHERE rowid=?;"; // Используем rowid за неимением order_id в структуре
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, id);
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

int delete_driver(sqlite3 *db, int personnel_number) {
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM DRIVERS WHERE personnel_number=?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, personnel_number);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (rc == SQLITE_DONE) ? 0 : -1;
}
