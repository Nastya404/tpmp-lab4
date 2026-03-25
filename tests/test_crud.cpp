#include <gtest/gtest.h>

extern "C" {
#include "../include/crud.h"
#include "../include/db_core.h"
#include "../include/models.h"
}

class CrudTest : public ::testing::Test {
protected:
    sqlite3 *db;

    void SetUp() override {
        db_connect("test.db", &db);

        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS DRIVERS (personnel_number INT PRIMARY KEY, surname TEXT, category TEXT, experience INT, address TEXT, birth_year INT);", NULL, NULL, NULL);
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS CARS (number TEXT PRIMARY KEY, brand TEXT, initial_mileage INT, current_mileage INT, capacity INT);", NULL, NULL, NULL);
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS ORDERS (date TEXT, driver_id INT, car_number TEXT, cargo_mass REAL, cost REAL, distance INT);", NULL, NULL, NULL);
    }

    void TearDown() override {
        db_close(db);
    }
};

TEST_F(CrudTest, AddDriver) {
    Driver d = {1, "Ivanov", "B", 5, "Minsk", 1990};
    EXPECT_EQ(add_driver(db, d), 0);
}

TEST_F(CrudTest, AddCar) {
    Car c = {"A123", "BMW", 1000, 1000, 500};
    EXPECT_EQ(add_car(db, c), 0);
}

TEST_F(CrudTest, AddOrder) {
    Driver d = {2, "Petrov", "C", 3, "Minsk", 1995};
    add_driver(db, d);

    Car c = {"B123", "Audi", 1000, 1000, 1000};
    add_car(db, c);

    Order o = {"2024-01-01", 2, "B123", 200, 500, 100};
    EXPECT_EQ(add_order(db, o), 0);
}
