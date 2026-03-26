#include <stdio.h>
#include "reports.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    (void)NotUsed;
    for (int i = 0; i < argc; i++) printf("%s: %s | ", azColName[i], argv[i] ? argv[i] : "NULL");
    printf("\n");
    return 0;
}

int get_driver_orders(sqlite3 *db, int driver_id, const char *start_date, const char *end_date) {
    char sql[1024];
    
    if (driver_id < 0) {
        printf("Для списка заказов нужно передать ID водителя.\n");
        return -1;
    }

    // П.2.1: список заказов конкретного водителя за период
    sprintf(sql,
            "SELECT o.date, d.surname, o.car_number, o.distance, o.cargo_mass, o.cost "
            "FROM ORDERS o "
            "JOIN DRIVERS d ON o.driver_id = d.personnel_number "
            "WHERE o.driver_id = %d AND o.date BETWEEN '%s' AND '%s' "
            "ORDER BY o.date, o.order_id;",
            driver_id, start_date, end_date);

    return sqlite3_exec(db, sql, callback, 0, NULL);
}

int get_driver_salary_info(sqlite3 *db, const char *surname, const char *start_date, const char *end_date) {
    char sql[1024];

    // П.2.6: все сведения и сколько денег получил водитель за период
    sprintf(sql,
            "SELECT d.personnel_number, d.surname, d.category, d.experience, d.address, d.birth_year, "
            "COUNT(o.order_id) AS trips, "
            "COALESCE(SUM(o.cargo_mass), 0) AS total_cargo_mass, "
            "COALESCE(SUM(o.cost * 0.2), 0) AS earned_money "
            "FROM DRIVERS d "
            "LEFT JOIN ORDERS o "
            "  ON o.driver_id = d.personnel_number "
            " AND o.date BETWEEN '%s' AND '%s' "
            "WHERE d.surname = '%s' "
            "GROUP BY d.personnel_number, d.surname, d.category, d.experience, d.address, d.birth_year "
            "ORDER BY d.personnel_number;",
            start_date, end_date, surname);

    return sqlite3_exec(db, sql, callback, 0, NULL);
}

int get_max_mileage_car(sqlite3 *db) {
    // П.2.5: машина с наибольшим общим пробегом (сумма дистанций из заказов + initial_mileage)
    return sqlite3_exec(db,
        "SELECT c.number, c.brand, c.initial_mileage, c.current_mileage, c.capacity, "
        "COALESCE(SUM(o.distance), 0) AS total_distance_from_orders, "
        "(c.initial_mileage + COALESCE(SUM(o.distance), 0)) AS total_mileage, "
        "COALESCE(SUM(o.cargo_mass), 0) AS total_cargo_mass "
        "FROM CARS c "
        "LEFT JOIN ORDERS o ON o.car_number = c.number "
        "GROUP BY c.number, c.brand, c.initial_mileage, c.current_mileage, c.capacity "
        "ORDER BY total_mileage DESC "
        "LIMIT 1;",
        callback, 0, NULL);
}

int get_lazy_driver_info(sqlite3 *db) {
    // П.2.4: водитель с наименьшим количеством поездок, все сведения + начисленные деньги
    const char *sql =
        "SELECT d.personnel_number, d.surname, d.category, d.experience, d.address, d.birth_year, "
        "COUNT(o.order_id) AS trips, "
        "COALESCE(SUM(o.cargo_mass), 0) AS total_cargo_mass, "
        "COALESCE(SUM(o.cost * 0.2), 0) AS earned_money "
        "FROM DRIVERS d "
        "LEFT JOIN ORDERS o ON d.personnel_number = o.driver_id "
        "GROUP BY d.personnel_number, d.surname, d.category, d.experience, d.address, d.birth_year "
        "ORDER BY trips ASC, d.personnel_number ASC "
        "LIMIT 1;";
    return sqlite3_exec(db, sql, callback, 0, NULL);
}

int get_car_statistics(sqlite3 *db, const char *car_number) {
    char sql[512];
    // П.2.2: для указанной машины общий пробег и общая масса грузов
    sprintf(sql,
            "SELECT c.number, c.brand, "
            "COALESCE(SUM(o.distance), 0) AS total_mileage, "
            "COALESCE(SUM(o.cargo_mass), 0) AS total_cargo_mass "
            "FROM CARS c "
            "LEFT JOIN ORDERS o ON o.car_number = c.number "
            "WHERE c.number = '%s' "
            "GROUP BY c.number, c.brand;",
            car_number);
    return sqlite3_exec(db, sql, callback, 0, NULL);
}

int get_driver_statistics(sqlite3 *db, int driver_id) {
    char sql[512];
    sprintf(sql,
            "SELECT d.personnel_number, d.surname, "
            "COUNT(o.order_id) AS trips, "
            "COALESCE(SUM(o.cargo_mass), 0) AS total_cargo_mass, "
            "COALESCE(SUM(o.cost * 0.2), 0) AS earned_money "
            "FROM DRIVERS d "
            "LEFT JOIN ORDERS o ON o.driver_id = d.personnel_number "
            "WHERE d.personnel_number = %d "
            "GROUP BY d.personnel_number, d.surname;",
            driver_id);
    return sqlite3_exec(db, sql, callback, 0, NULL);
}

int get_all_drivers_statistics(sqlite3 *db) {
    // П.2.3: по каждому водителю количество поездок, общая масса грузов и сумма заработанных денег
    return sqlite3_exec(db,
        "SELECT d.personnel_number, d.surname, "
        "COUNT(o.order_id) AS trips, "
        "COALESCE(SUM(o.cargo_mass), 0) AS total_cargo_mass, "
        "COALESCE(SUM(o.cost * 0.2), 0) AS earned_money "
        "FROM DRIVERS d "
        "LEFT JOIN ORDERS o ON o.driver_id = d.personnel_number "
        "GROUP BY d.personnel_number, d.surname "
        "ORDER BY d.personnel_number ASC;",
        callback, 0, NULL);
}

int get_salary_records(sqlite3 *db, const char *start_date, const char *end_date) {
    char sql[512];
    // Показываем записи за ровно запрошенный период
    sprintf(sql, "SELECT * FROM SALARY_RECORDS WHERE start_date = '%s' AND end_date = '%s';", start_date, end_date);
    return sqlite3_exec(db, sql, callback, 0, NULL);
}
