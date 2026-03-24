#include <stdio.h>
#include "reports.h"

// Вспомогательная функция для печати заголовков и строк таблицы
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s: %s | ", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

// 1. Список заказов водителя
int get_driver_orders(sqlite3 *db, int driver_id, const char *start, const char *end) {
    char sql[512];
    sprintf(sql, "SELECT * FROM ORDERS WHERE driver_id = %d AND date BETWEEN '%s' AND '%s';", driver_id, start, end);
    printf("\n--- Заказы водителя #%d за период %s - %s ---\n", driver_id, start, end);
    return sqlite3_exec(db, sql, callback, 0, NULL);
}

// 2. Машина с макс. пробегом
int get_max_mileage_car(sqlite3 *db) {
    const char *sql = "SELECT number, brand, current_mileage FROM CARS ORDER BY current_mileage DESC LIMIT 1;";
    printf("\n--- Машина с максимальным пробегом ---\n");
    return sqlite3_exec(db, sql, callback, 0, NULL);
}

// 3. Поиск водителя с мин. числом поездок
int get_lazy_driver_info(sqlite3 *db) {
    const char *sql = 
        "SELECT d.surname, d.personnel_number, COUNT(o.order_id) as trips_count "
        "FROM DRIVERS d LEFT JOIN ORDERS o ON d.personnel_number = o.driver_id "
        "GROUP BY d.personnel_number ORDER BY trips_count ASC LIMIT 1;";
    printf("\n--- Водитель с минимальным количеством поездок ---\n");
    return sqlite3_exec(db, sql, callback, 0, NULL);
}

// 4. Статистика по машине (суммарный вес, стоимость, дистанция)
int get_car_statistics(sqlite3 *db, const char *car_number) {
    char sql[512];
    sprintf(sql, "SELECT car_number, COUNT(*) as total_orders, SUM(distance) as total_km, "
                 "SUM(cargo_mass) as total_mass, SUM(cost) as total_profit "
                 "FROM ORDERS WHERE car_number = '%s';", car_number);
    printf("\n--- Статистика по машине %s ---\n", car_number);
    return sqlite3_exec(db, sql, callback, 0, NULL);
}

// 5. Статистика по водителю
int get_driver_statistics(sqlite3 *db, int driver_id) {
    char sql[512];
    sprintf(sql, "SELECT driver_id, COUNT(*) as orders_done, SUM(distance) as km_driven, "
                 "SUM(cost) as total_revenue FROM ORDERS WHERE driver_id = %d;", driver_id);
    printf("\n--- Статистика водителя #%d ---\n", driver_id);
    return sqlite3_exec(db, sql, callback, 0, NULL);
}

// 6. Сводный отчет по всем
int get_all_drivers_statistics(sqlite3 *db) {
    const char *sql = 
        "SELECT d.surname, COUNT(o.order_id) as orders, SUM(o.distance) as km "
        "FROM DRIVERS d JOIN ORDERS o ON d.personnel_number = o.driver_id "
        "GROUP BY d.personnel_number;";
    printf("\n--- Сводный отчет по всем водителям ---\n");
    return sqlite3_exec(db, sql, callback, 0, NULL);
}

// 7. Зарплаты за период
int get_salary_records(sqlite3 *db, const char *start, const char *end) {
    char sql[512];
    sprintf(sql, "SELECT r.*, d.surname FROM SALARY_RECORDS r "
                 "JOIN DRIVERS d ON r.driver_id = d.personnel_number "
                 "WHERE r.start_date = '%s' AND r.end_date = '%s';", start, end);
    printf("\n--- Отчет по зарплатам за период %s - %s ---\n", start, end);
    return sqlite3_exec(db, sql, callback, 0, NULL);
}
