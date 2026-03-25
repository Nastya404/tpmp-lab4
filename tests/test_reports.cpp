#include <gtest/gtest.h>

extern "C" {
#include "../include/reports.h"
#include "../include/db_core.h"
}

class ReportsTest : public ::testing::Test {
protected:
    sqlite3 *db;

    void SetUp() override {
        db_connect("test.db", &db);

        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS DRIVERS (personnel_number INT, surname TEXT);", NULL, NULL, NULL);
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS ORDERS (date TEXT, driver_id INT, car_number TEXT, cargo_mass REAL, cost REAL, distance INT);", NULL, NULL, NULL);
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS CARS (number TEXT, brand TEXT, initial_mileage INT, current_mileage INT, capacity INT);", NULL, NULL, NULL);
    }

    void TearDown() override {
        db_close(db);
    }
};

TEST_F(ReportsTest, DriverOrdersInvalidID) {
    EXPECT_EQ(get_driver_orders(db, -1, "2024-01-01", "2024-12-31"), -1);
}

TEST_F(ReportsTest, MaxMileageCar) {
    EXPECT_EQ(get_max_mileage_car(db), SQLITE_OK);
}

TEST_F(ReportsTest, AllDriversStats) {
    EXPECT_EQ(get_all_drivers_statistics(db), SQLITE_OK);
}
