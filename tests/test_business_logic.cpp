#include <gtest/gtest.h>
#include <cstring>  // Обязательно для strcpy

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
        db_connect(":memory:", &db);

        // Приводим структуру таблиц в соответствие с тем, что ожидает код
        sqlite3_exec(db, "DROP TABLE IF EXISTS CARS;", NULL, NULL, NULL);
        sqlite3_exec(db, "DROP TABLE IF EXISTS ORDERS;", NULL, NULL, NULL);

        sqlite3_exec(db, "CREATE TABLE CARS (number TEXT PRIMARY KEY, brand TEXT, initial_mileage INT, current_mileage INT, capacity INT);", NULL, NULL, NULL);
        sqlite3_exec(db, "CREATE TABLE ORDERS (order_id INTEGER PRIMARY KEY AUTOINCREMENT, date TEXT, driver_id INT, car_number TEXT, distance INT, cargo_mass REAL, cost REAL);", NULL, NULL, NULL);

        // Добавляем тестовую машину
        sqlite3_exec(db, "INSERT INTO CARS (number, brand, initial_mileage, current_mileage, capacity) VALUES ('A1', 'TestBrand', 0, 0, 500);", NULL, NULL, NULL);
    }

    void TearDown() override {
        db_close(db);
        remove("test.db");
    }
};

TEST_F(BusinessTest, CargoLimitOK) {
    EXPECT_EQ(check_cargo_limit(db, "A1", 200), 0);
}

TEST_F(BusinessTest, CargoLimitExceeded) {
    EXPECT_EQ(check_cargo_limit(db, "A1", 600), 1);
}

TEST_F(BusinessTest, ProcessOrderFail) {
    // Инициализируем структуру правильно, чтобы компилятор не ругался
    Order o = {0};
    o.order_id = 1;
    strcpy(o.date, "2024-01-01");
    o.driver_id = 1;
    strcpy(o.car_number, "A1");
    o.distance = 100;
    o.cargo_mass = 1000.0; // Вес больше лимита (500)
    o.cost = 500.0;

    // Ожидаем ошибку (1), так как вес (1000) превышает грузоподъемность машины A1 (500)
    EXPECT_EQ(process_new_order(db, o), 1);
}

TEST_F(BusinessTest, CalculateSalaries) {

   sqlite3_exec(db, 
   "CREATE TABLE IF NOT EXISTS SALARY_RECORDS "
   "(start_date TEXT, end_date TEXT, total_salary REAL, "
   "calculated_at TEXT, driver_id INT);", 
    NULL, NULL, NULL);

   sqlite3_exec(db,
   "INSERT INTO DRIVERS (personnel_number, surname, category, "
   "experience, address, birth_year) "
   "VALUES (1, 'Ivanov', 'B', 5, 'Minsk', 1990);",
   NULL, NULL, NULL);
  

   sqlite3_exec(db,
   "INSERT INTO ORDERS (date, driver_id, car_number, distance, "
   "cargo_mass, cost) VALUES ('2024-01-15', 1, 'A1', 100, 200, 500);",
   NULL, NULL, NULL);

    EXPECT_EQ(calculate_salaries_to_db(db, "2024-01-01", "2024-12-31"), 0);
}
