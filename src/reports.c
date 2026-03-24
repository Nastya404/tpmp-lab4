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
    if (driver_id != -1) {
        // Обычный поиск по ID (Пункт 8 меню)
        sprintf(sql, "SELECT * FROM ORDERS WHERE driver_id = %d AND date BETWEEN '%s' AND '%s';", 
                driver_id, start_date, end_date);
    } else {
        sprintf(sql, "SELECT o.* FROM ORDERS o JOIN DRIVERS d ON o.driver_id = d.personnel_number "
                     "WHERE d.surname = '%s' AND o.date BETWEEN '%s' AND '%s';", 
                start_date, end_date, (const char*)driver_id); // Здесь логика вызова в main изменится
    }
    sprintf(sql, "SELECT o.*, d.surname FROM ORDERS o JOIN DRIVERS d ON o.driver_id = d.personnel_number "
                 "WHERE (d.personnel_number = %d OR d.surname = '%s') "
                 "AND o.date BETWEEN '%s' AND '%s';", 
                 driver_id, start_date, end_date, (char*)driver_id); // Сложно без смены сигнатуры.
    
    char final_sql[1024];
    if (driver_id == -1) {
        sprintf(final_sql, "SELECT o.* FROM ORDERS o JOIN DRIVERS d ON o.driver_id = d.personnel_number "
                           "WHERE d.surname = '%s' AND o.date BETWEEN '%s' AND '%s';", 
                           start_date, end_date, (char*)driver_id); // В main передадим фамилию в start_date
    } else {
        sprintf(final_sql, "SELECT * FROM ORDERS WHERE driver_id = %d AND date BETWEEN '%s' AND '%s';", 
                           driver_id, start_date, end_date);
    }

    return sqlite3_exec(db, final_sql, callback, 0, NULL);
}

int get_max_mileage_car(sqlite3 *db) {
    return sqlite3_exec(db, "SELECT * FROM CARS ORDER BY current_mileage DESC LIMIT 1;", callback, 0, NULL);
}

int get_lazy_driver_info(sqlite3 *db) {
    const char *sql = "SELECT d.surname, COUNT(o.rowid) as trips FROM DRIVERS d "
                      "LEFT JOIN ORDERS o ON d.personnel_number = o.driver_id "
                      "GROUP BY d.personnel_number ORDER BY trips ASC LIMIT 1;";
    return sqlite3_exec(db, sql, callback, 0, NULL);
}

int get_car_statistics(sqlite3 *db, const char *car_number) {
    char sql[512];
    sprintf(sql, "SELECT car_number, SUM(distance), SUM(cost) FROM ORDERS WHERE car_number = '%s';", car_number);
    return sqlite3_exec(db, sql, callback, 0, NULL);
}

int get_driver_statistics(sqlite3 *db, int driver_id) {
    char sql[512];
    sprintf(sql, "SELECT driver_id, COUNT(*), SUM(cost) FROM ORDERS WHERE driver_id = %d;", driver_id);
    return sqlite3_exec(db, sql, callback, 0, NULL);
}

int get_all_drivers_statistics(sqlite3 *db) {
    return sqlite3_exec(db, "SELECT driver_id, COUNT(*), SUM(cost) FROM ORDERS GROUP BY driver_id;", callback, 0, NULL);
}

int get_salary_records(sqlite3 *db, const char *start_date, const char *end_date) {
    char sql[512];
    sprintf(sql, "SELECT * FROM SALARY_RECORDS WHERE start_date >= '%s' AND end_date <= '%s';", start_date, end_date);
    return sqlite3_exec(db, sql, callback, 0, NULL);
}
