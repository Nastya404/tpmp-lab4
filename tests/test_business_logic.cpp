#include <gtest/gtest.h>

extern "C" {
#include "../include/business_logic.h"
#include "../include/crud.h"
#include "../include/db_core.h"
#include "../include/models.h"
}

class BusinessTest : public ::testing::Test {
protected:
    sqlite3 *db;

    void SetUp() override {
        db_connect("test.db", &db);

        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS CARS (number TEXT PRIMARY KEY, capacity REAL);", NULL, NULL, NULL);
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS ORDERS (date TEXT, driver_id INT, car_number TEXT, cargo_mass REAL, cost REAL, distance INT);", NULL, NULL, NULL);

        sqlite3_exec(db, "INSERT INTO CARS VALUES ('A1', 500);", NULL, NULL, NULL);
    }

    void TearDown() override {
        db_close(db);
    }
};

TEST_F(BusinessTest, CargoLimitOK) {
    EXPECT_EQ(check_cargo_limit(db, "A1", 200), 0);
}

TEST_F(BusinessTest, CargoLimitExceeded) {
    EXPECT_EQ(check_cargo_limit(db, "A1", 600), 1);
}

TEST_F(BusinessTest, ProcessOrderFail) {
    Order o = {"2024-01-01", 1, "A1", 1000, 500, 100};
    EXPECT_EQ(process_new_order(db, o), 1);
}
