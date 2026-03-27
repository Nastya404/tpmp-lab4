#include <gtest/gtest.h>
#include <cstring>

extern "C" {
#include "../include/business_logic.h"
#include "../include/crud.h"
#include "../include/db_core.h"
#include "../include/models.h"
}

class ClosedDbTest : public ::testing::Test {
protected:
    sqlite3 *db;

    void SetUp() override {
        sqlite3_open(":memory:", &db);
        sqlite3_close(db);
    }

};


TEST_F(ClosedDbTest, CalculateSalaries_PrepareDeleteFails) {
    int rc = calculate_salaries_to_db(db, "2024-01-01", "2024-12-31");
    EXPECT_EQ(rc, -1);
}

class SalaryOnlyDbTest : public ::testing::Test {
protected:
    sqlite3 *db;

    void SetUp() override {
        db_connect(":memory:", &db);
        sqlite3_exec(db,
            "CREATE TABLE SALARY_RECORDS ("
            "record_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "start_date TEXT, end_date TEXT, total_salary REAL,"
            "calculated_at TEXT, driver_id INT);",
            NULL, NULL, NULL);
    }

    void TearDown() override { db_close(db); }
};

TEST_F(SalaryOnlyDbTest, CalculateSalaries_PrepareInsertFails) {
    int rc = calculate_salaries_to_db(db, "2024-01-01", "2024-12-31");
    EXPECT_EQ(rc, -1);
}

TEST_F(ClosedDbTest, UpdateOrder_PrepareFailsReturnsMinusOne) {
    Order o = {0};
    strcpy(o.date, "2024-01-01");
    o.driver_id  = 1;
    strcpy(o.car_number, "A1");
    o.distance   = 10;
    o.cargo_mass = 100.0;
    o.cost       = 200.0;

    EXPECT_EQ(update_order(db, o), -1);
}

TEST_F(ClosedDbTest, AuthenticateUser_PrepareFailsReturnsMinusOne) {
    User u = {0};
    EXPECT_EQ(authenticate_user(db, "login", "pass", &u), -1);
}


class IntegrityTest : public ::testing::Test {
protected:
    sqlite3 *db;

    void SetUp() override {
        db_connect(":memory:", &db);
        sqlite3_exec(db,
            "CREATE TABLE DRIVERS ("
            "personnel_number INT PRIMARY KEY NOT NULL,"
            "surname TEXT NOT NULL, category TEXT NOT NULL,"
            "experience INT, address TEXT, birth_year INT);",
            NULL, NULL, NULL);
        sqlite3_exec(db,
            "CREATE TABLE CARS ("
            "number TEXT PRIMARY KEY NOT NULL,"
            "brand TEXT, initial_mileage INT,"
            "current_mileage INT, capacity INT);",
            NULL, NULL, NULL);
        sqlite3_exec(db,
            "CREATE TABLE ORDERS ("
            "order_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "date TEXT, driver_id INT, car_number TEXT,"
            "distance INT, cargo_mass REAL, cost REAL);",
            NULL, NULL, NULL);
        sqlite3_exec(db,
            "CREATE TABLE AUTOPARK_USERS ("
            "user_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "login TEXT UNIQUE NOT NULL,"
            "password_hash TEXT, role TEXT, personnel_number INT);",
            NULL, NULL, NULL);
    }

    void TearDown() override { db_close(db); }
};

// add_driver дубликат PK
TEST_F(IntegrityTest, AddDriver_FailOnDuplicate) {
    Driver d = {0};
    d.personnel_number = 1;
    strcpy(d.surname, "A"); strcpy(d.category, "B");
    d.experience = 1; strcpy(d.address, "X"); d.birth_year = 1990;

    EXPECT_EQ(add_driver(db, d), 0);
    EXPECT_EQ(add_driver(db, d), -1);
}

// add_car дубликат PK
TEST_F(IntegrityTest, AddCar_FailOnDuplicate) {
    Car c = {0};
    strcpy(c.number, "AA1"); strcpy(c.brand, "X");
    c.initial_mileage = 0; c.current_mileage = 0; c.capacity = 100;

    EXPECT_EQ(add_car(db, c), 0);
    EXPECT_EQ(add_car(db, c), -1);
}

TEST(AddOrderClosedDb, FailOnClosedDb) {
    sqlite3 *db;
    sqlite3_open(":memory:", &db);

    sqlite3_exec(db,
        "CREATE TABLE ORDERS ("
        "order_id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "date TEXT, driver_id INT, car_number TEXT,"
        "distance INT, cargo_mass REAL, cost REAL);",
        NULL, NULL, NULL);
    sqlite3_close(db);

    Order o = {0};
    strcpy(o.date, "2024-01-01");
    o.driver_id = 1; strcpy(o.car_number, "A1");
    o.distance = 10; o.cargo_mass = 50.0; o.cost = 100.0;

    int rc = add_order(db, o);
    EXPECT_EQ(rc, -1);
}

// update_driver на закрытой БД
TEST(UpdateDriverClosedDb, FailOnClosedDb) {
    sqlite3 *db;
    sqlite3_open(":memory:", &db);
    sqlite3_close(db);

    Driver d = {0};
    d.personnel_number = 1;
    strcpy(d.surname, "X"); strcpy(d.category, "B");
    d.experience = 1; strcpy(d.address, "Y"); d.birth_year = 1990;

    EXPECT_EQ(update_driver(db, d), -1);
}

// update_car на закрытой БД
TEST(UpdateCarClosedDb, FailOnClosedDb) {
    sqlite3 *db;
    sqlite3_open(":memory:", &db);
    sqlite3_close(db);

    Car c = {0};
    strcpy(c.number, "Z1"); strcpy(c.brand, "X");
    c.initial_mileage = 0; c.current_mileage = 0; c.capacity = 100;

    EXPECT_EQ(update_car(db, c), -1);
}

// update_mileage на закрытой БД
TEST(UpdateMileageClosedDb, FailOnClosedDb) {
    sqlite3 *db;
    sqlite3_open(":memory:", &db);
    sqlite3_close(db);

    EXPECT_EQ(update_mileage(db, "Z1", 999), -1);
}

// delete_driver на закрытой БД
TEST(DeleteDriverClosedDb, FailOnClosedDb) {
    sqlite3 *db;
    sqlite3_open(":memory:", &db);
    sqlite3_close(db);

    EXPECT_EQ(delete_driver(db, 1), -1);
}

// delete_car на закрытой БД
TEST(DeleteCarClosedDb, FailOnClosedDb) {
    sqlite3 *db;
    sqlite3_open(":memory:", &db);
    sqlite3_close(db);

    EXPECT_EQ(delete_car(db, "Z1"), -1);
}

// delete_order на закрытой БД
TEST(DeleteOrderClosedDb, FailOnClosedDb) {
    sqlite3 *db;
    sqlite3_open(":memory:", &db);
    sqlite3_close(db);

    EXPECT_EQ(delete_order(db, 1), -1);
}
