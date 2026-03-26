#include <gtest/gtest.h>
#include <cstring>  // для strcpy

extern "C" {
#include "../include/crud.h"
#include "../include/db_core.h"
#include "../include/models.h"
}

class CrudTest : public ::testing::Test {
protected:
    sqlite3 *db;

    void SetUp() override {
        db_connect(":memory:", &db);
        // Создаем таблицы перед тестами
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS DRIVERS (personnel_number INT PRIMARY KEY, surname TEXT, category TEXT, experience INT, address TEXT, birth_year INT);", NULL, NULL, NULL);
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS CARS (number TEXT PRIMARY KEY, brand TEXT, initial_mileage INT, current_mileage INT, capacity INT);", NULL, NULL, NULL);
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS ORDERS (order_id INTEGER PRIMARY KEY AUTOINCREMENT, date TEXT, driver_id INT, car_number TEXT, distance INT, cargo_mass REAL, cost REAL);", NULL, NULL, NULL);
        
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS AUTOPARK_USERS ("
                     "user_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                     "login TEXT UNIQUE, "
                     "password_hash TEXT, "
                     "role TEXT, "
                     "personnel_number INT);", NULL, NULL, NULL);
    }

    void TearDown() override {
        db_close(db);
        remove("test.db"); // Удаляем БД после тестов
    }
};

// --- Добавление ---

TEST_F(CrudTest, AddDriver) {
    Driver d = {0}; // Зануляем всё
    d.personnel_number = 1;
    strcpy(d.surname, "Ivanov");
    strcpy(d.category, "B");
    d.experience = 5;
    strcpy(d.address, "Minsk");
    d.birth_year = 1990;

    EXPECT_EQ(add_driver(db, d), 0);
}

TEST_F(CrudTest, AddCar) {
    Car c = {0};
    strcpy(c.number, "A123");
    strcpy(c.brand, "BMW");
    c.initial_mileage = 1000;
    c.current_mileage = 1000;
    c.capacity = 500;

    EXPECT_EQ(add_car(db, c), 0);
}

TEST_F(CrudTest, AddOrder) {
    // Сначала нужны водитель и машина для внешних ключей (если они есть)
    Driver d = {2, "Petrov", "C", 3, "Minsk", 1995};
    add_driver(db, d);

    Car c = {0};
    strcpy(c.number, "B123");
    strcpy(c.brand, "Audi");
    c.initial_mileage = 1000;
    c.current_mileage = 1000;
    c.capacity = 1000;
    add_car(db, c);

    // Инициализация Order согласно models.h: 
    // order_id, date, driver_id, car_number, distance, cargo_mass, cost
    Order o = {0};
    o.order_id = 1;
    strcpy(o.date, "2024-01-01");
    o.driver_id = 2;
    strcpy(o.car_number, "B123");
    o.distance = 100;
    o.cargo_mass = 200.0;
    o.cost = 500.0;

    EXPECT_EQ(add_order(db, o), 0);
}

// --- Обновление ---

TEST_F(CrudTest, UpdateDriver) {
    Driver d = {3, "Ivanov", "B", 5, "Minsk", 1990};
    add_driver(db, d);

    strcpy(d.surname, "Sidorov");
    d.experience = 10;
    EXPECT_EQ(update_driver(db, d), 0);
}

TEST_F(CrudTest, UpdateCar) {
    Car c = {0};
    strcpy(c.number, "C123");
    strcpy(c.brand, "Toyota");
    c.initial_mileage = 1000;
    c.current_mileage = 1000;
    c.capacity = 500;
    add_car(db, c);

    strcpy(c.brand, "Mercedes");
    c.capacity = 600;
    EXPECT_EQ(update_car(db, c), 0);
}

TEST_F(CrudTest, UpdateOrder) {
    Driver d = {4, "Petrov", "B", 4, "Minsk", 1992};
    add_driver(db, d);

    Car c = {0};
    strcpy(c.number, "D123");
    strcpy(c.brand, "Ford");
    add_car(db, c);

    Order o = {0};
    o.order_id = 2;
    strcpy(o.date, "2024-02-01");
    o.driver_id = 4;
    strcpy(o.car_number, "D123");
    o.distance = 50;
    o.cargo_mass = 100.0;
    o.cost = 300.0;
    add_order(db, o);

    o.cargo_mass = 120.0;
    o.cost = 350.0;
    o.distance = 60;
    EXPECT_EQ(update_order(db, o), 0);
}

// --- Обновление пробега ---

TEST_F(CrudTest, UpdateMileage) {
    Car c = {0};
    strcpy(c.number, "E123");
    strcpy(c.brand, "Honda");
    c.initial_mileage = 1000;
    c.current_mileage = 1000;
    add_car(db, c);

    EXPECT_EQ(update_mileage(db, "E123", 1500), 0);
}

// --- Удаление ---

TEST_F(CrudTest, DeleteDriver) {
    Driver d = {5, "Kuznetsov", "B", 5, "Minsk", 1990};
    add_driver(db, d);

    EXPECT_EQ(delete_driver(db, 5), 0);
}

TEST_F(CrudTest, DeleteCar) {
    Car c = {0};
    strcpy(c.number, "F123");
    strcpy(c.brand, "Kia");
    add_car(db, c);

    EXPECT_EQ(delete_car(db, "F123"), 0);
}

TEST_F(CrudTest, DeleteOrder) {
    Driver d = {6, "Smirnov", "B", 5, "Minsk", 1990};
    add_driver(db, d);

    Car c = {0};
    strcpy(c.number, "G123");
    strcpy(c.brand, "Mazda");
    add_car(db, c);

    Order o = {0};
    o.order_id = 10; // Явный ID для теста удаления
    strcpy(o.date, "2024-03-01");
    o.driver_id = 6;
    strcpy(o.car_number, "G123");
    add_order(db, o);

    EXPECT_EQ(delete_order(db, 10), 0);
  
}

    // --- Тесты авторизации ---

TEST_F(CrudTest, AuthenticateUserSuccess) {
    // 1. Подготовка: добавляем тестового пользователя в БД напрямую
    // В реальной жизни тут был бы хеш, но для лабы обычно используем простой текст
    sqlite3_exec(db, "INSERT INTO AUTOPARK_USERS (login, password_hash, role, personnel_number) "
                     "VALUES ('admin', '1234', 'ADMIN', 1);", NULL, NULL, NULL);

    User out_user = {0};
    
    // 2. Проверка успешного входа
    int result = authenticate_user(db, "admin", "1234", &out_user);
    
    EXPECT_EQ(result, 0);
    EXPECT_STREQ(out_user.login, "admin");
    EXPECT_STREQ(out_user.role, "ADMIN");
    EXPECT_EQ(out_user.personnel_number, 1);
}

TEST_F(CrudTest, AuthenticateUserWrongPassword) {
    sqlite3_exec(db, "INSERT INTO AUTOPARK_USERS (login, password_hash, role) "
                     "VALUES ('driver1', 'pass', 'DRIVER');", NULL, NULL, NULL);

    User out_user = {0};
    
    // Попытка входа с неверным паролем
    int result = authenticate_user(db, "driver1", "wrong_pass", &out_user);
    
    EXPECT_NE(result, 0); // Должно вернуть ошибку (не 0)
}

TEST_F(CrudTest, AuthenticateUserNotFound) {
    User out_user = {0};
    
    // Попытка входа несуществующего пользователя
    int result = authenticate_user(db, "non_existent", "any", &out_user);
    
    EXPECT_NE(result, 0);
}
