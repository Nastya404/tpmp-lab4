#include <gtest/gtest.h>
#include <cstring>

extern "C"
{
#include "../include/business_logic.h"
#include "../include/crud.h"
#include "../include/db_core.h"
#include "../include/models.h"
}

class BranchTest : public ::testing::Test
{
protected:
    sqlite3 *db;

    void SetUp() override
    {
        db_connect(":memory:", &db);

        sqlite3_exec(db, "CREATE TABLE CARS ("
                         "number TEXT PRIMARY KEY, brand TEXT, "
                         "initial_mileage INT, current_mileage INT, capacity INT);",
                     NULL, NULL, NULL);

        sqlite3_exec(db, "CREATE TABLE DRIVERS ("
                         "personnel_number INT PRIMARY KEY, surname TEXT, "
                         "category TEXT, experience INT, address TEXT, birth_year INT);",
                     NULL, NULL, NULL);

        sqlite3_exec(db, "CREATE TABLE ORDERS ("
                         "order_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "date TEXT, driver_id INT, car_number TEXT, "
                         "distance INT, cargo_mass REAL, cost REAL);",
                     NULL, NULL, NULL);

        sqlite3_exec(db, "CREATE TABLE SALARY_RECORDS ("
                         "record_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "start_date TEXT, end_date TEXT, total_salary REAL, "
                         "calculated_at TEXT, driver_id INT);",
                     NULL, NULL, NULL);

        sqlite3_exec(db, "CREATE TABLE AUTOPARK_USERS ("
                         "user_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "login TEXT UNIQUE, password_hash TEXT, "
                         "role TEXT, personnel_number INT);",
                     NULL, NULL, NULL);
    }

    void TearDown() override
    {
        db_close(db);
    }

    void insertCar(const char *num, int capacity)
    {
        char sql[256];
        snprintf(sql, sizeof(sql),
                 "INSERT INTO CARS VALUES ('%s','Brand',0,0,%d);", num, capacity);
        sqlite3_exec(db, sql, NULL, NULL, NULL);
    }

    void insertDriver(int id, const char *surname)
    {
        char sql[256];
        snprintf(sql, sizeof(sql),
                 "INSERT INTO DRIVERS VALUES (%d,'%s','B',5,'Addr',1990);",
                 id, surname);
        sqlite3_exec(db, sql, NULL, NULL, NULL);
    }
};


//машина не найдена → capacity=0 → mass > 0 → возврат 1
TEST_F(BranchTest, CheckCargoLimit_CarNotFound)
{
    EXPECT_EQ(check_cargo_limit(db, "UNKNOWN", 1.0), 1);
}

// mass == capacity (граничное значение) → возврат 0
TEST_F(BranchTest, CheckCargoLimit_ExactlyAtLimit)
{
    insertCar("X1", 500);
    EXPECT_EQ(check_cargo_limit(db, "X1", 500.0), 0);
}

//mass == 0 → всегда <= capacity → возврат 0
TEST_F(BranchTest, CheckCargoLimit_ZeroMass)
{
    insertCar("X2", 500);
    EXPECT_EQ(check_cargo_limit(db, "X2", 0.0), 0);
}

// Ветвь: process_new_order — cargo OK → add_order выполняется (возврат 0)
TEST_F(BranchTest, ProcessNewOrder_Success)
{
    insertCar("X3", 1000);
    insertDriver(10, "Ivanov");

    Order o = {0};
    strcpy(o.date, "2024-06-01");
    o.driver_id = 10;
    strcpy(o.car_number, "X3");
    o.distance = 100;
    o.cargo_mass = 500.0;
    o.cost = 300.0;

    EXPECT_EQ(process_new_order(db, o), 0);
}

// Ветвь: calculate_salaries — нет водителей → INSERT ничего не вставляет,
//         возврат 0 (SQLITE_DONE)
TEST_F(BranchTest, CalculateSalaries_NoDrivers)
{
    EXPECT_EQ(calculate_salaries_to_db(db, "2024-01-01", "2024-12-31"), 0);
}

//повторный вызов calculate_salaries — DELETE отрабатывает по
//        существующим записям, затем INSERT
TEST_F(BranchTest, CalculateSalaries_Recalculate)
{
    insertDriver(20, "Petrov");
    sqlite3_exec(db,
                 "INSERT INTO ORDERS (date,driver_id,car_number,distance,cargo_mass,cost)"
                 " VALUES ('2024-03-01',20,'X3',50,100,400);",
                 NULL, NULL, NULL);

    EXPECT_EQ(calculate_salaries_to_db(db, "2024-01-01", "2024-12-31"), 0);
    EXPECT_EQ(calculate_salaries_to_db(db, "2024-01-01", "2024-12-31"), 0);
}

//add_driver — дубликат personnel_number → возврат -1
TEST_F(BranchTest, AddDriver_DuplicateFails)
{
    Driver d = {0};
    d.personnel_number = 99;
    strcpy(d.surname, "Test");
    strcpy(d.category, "B");
    d.experience = 1;
    strcpy(d.address, "Addr");
    d.birth_year = 1990;

    EXPECT_EQ(add_driver(db, d), 0);
    EXPECT_EQ(add_driver(db, d), -1); // дубликат PK → ветвь rc != SQLITE_DONE
}

// add_car — дубликат number → возврат -1
TEST_F(BranchTest, AddCar_DuplicateFails)
{
    Car c = {0};
    strcpy(c.number, "DUP1");
    strcpy(c.brand, "BMW");
    c.initial_mileage = 0;
    c.current_mileage = 0;
    c.capacity = 500;

    EXPECT_EQ(add_car(db, c), 0);
    EXPECT_EQ(add_car(db, c), -1); // дубликат PK → ветвь rc != SQLITE_DONE
}

//add_order — несуществующий driver_id 
TEST_F(BranchTest, AddOrder_ReturnZeroOnSuccess)
{
    insertCar("O1", 1000);
    insertDriver(30, "Smirnov");

    Order o = {0};
    strcpy(o.date, "2024-07-01");
    o.driver_id = 30;
    strcpy(o.car_number, "O1");
    o.distance = 200;
    o.cargo_mass = 300.0;
    o.cost = 600.0;

    EXPECT_EQ(add_order(db, o), 0);
}

//update_driver — несуществующий ID → SQLITE_DONE всё равно (0 rows)
TEST_F(BranchTest, UpdateDriver_NonExistentReturnsZero)
{
    Driver d = {0};
    d.personnel_number = 9999;
    strcpy(d.surname, "Ghost");
    strcpy(d.category, "C");
    d.experience = 0;
    strcpy(d.address, "Nowhere");
    d.birth_year = 1990;

    EXPECT_EQ(update_driver(db, d), 0);
}

// update_car — несуществующий номер
TEST_F(BranchTest, UpdateCar_NonExistentReturnsZero)
{
    Car c = {0};
    strcpy(c.number, "GHOST");
    strcpy(c.brand, "Unknown");
    c.initial_mileage = 0;
    c.current_mileage = 0;
    c.capacity = 100;

    EXPECT_EQ(update_car(db, c), 0);
}

// update_order — несуществующая запись
TEST_F(BranchTest, UpdateOrder_NonExistentReturnsZero)
{
    Order o = {0};
    strcpy(o.date, "2000-01-01");
    o.driver_id = 9999;
    strcpy(o.car_number, "GHOST");
    o.distance = 1;
    o.cargo_mass = 1.0;
    o.cost = 1.0;

    EXPECT_EQ(update_order(db, o), 0);
}

// update_mileage — несуществующий номер (SQLITE_DONE, 0 rows)
TEST_F(BranchTest, UpdateMileage_NonExistentReturnsZero)
{
    EXPECT_EQ(update_mileage(db, "GHOST", 999), 0);
}

// delete_driver — несуществующий ID (SQLITE_DONE, 0 rows)
TEST_F(BranchTest, DeleteDriver_NonExistentReturnsZero)
{
    EXPECT_EQ(delete_driver(db, 9999), 0);
}

// delete_car — несуществующий номер
TEST_F(BranchTest, DeleteCar_NonExistentReturnsZero)
{
    EXPECT_EQ(delete_car(db, "GHOST"), 0);
}

//delete_order — несуществующий rowid
TEST_F(BranchTest, DeleteOrder_NonExistentReturnsZero)
{
    EXPECT_EQ(delete_order(db, 9999), 0);
}

// personnel_number = NULL в БД → out_user->personnel_number = 0
TEST_F(BranchTest, AuthenticateUser_NullPersonnelNumber)
{
    sqlite3_exec(db,
                 "INSERT INTO AUTOPARK_USERS (login, password_hash, role, personnel_number)"
                 " VALUES ('admin2', 'secret', 'ADMIN', NULL);",
                 NULL, NULL, NULL);

    User u = {0};
    EXPECT_EQ(authenticate_user(db, "admin2", "secret", &u), 0);
    EXPECT_EQ(u.personnel_number, 0); // ветвь SQLITE_NULL → 0
    EXPECT_STREQ(u.role, "ADMIN");
}

TEST_F(BranchTest, AuthenticateUser_WithPersonnelNumber)
{
    insertDriver(42, "Kozlov");
    sqlite3_exec(db,
                 "INSERT INTO AUTOPARK_USERS (login, password_hash, role, personnel_number)"
                 " VALUES ('driver42', 'pass42', 'DRIVER', 42);",
                 NULL, NULL, NULL);

    User u = {0};
    EXPECT_EQ(authenticate_user(db, "driver42", "pass42", &u), 0);
    EXPECT_EQ(u.personnel_number, 42); 
    EXPECT_STREQ(u.role, "DRIVER");
}