#include <stdio.h>
#include "business_logic.h"
#include "crud.h"

int check_cargo_limit(sqlite3 *db, const char *car_number, double mass) {
    sqlite3_stmt *stmt;
    double capacity = 0;
    const char *sql = "SELECT capacity FROM CARS WHERE number = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, car_number, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        capacity = sqlite3_column_double(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return (mass <= capacity) ? 0 : 1;
}

int process_new_order(sqlite3 *db, Order order) {
    if (check_cargo_limit(db, order.car_number, order.cargo_mass) != 0) {
        return 1; // Превышен лимит
    }
    return add_order(db, order);
}

int calculate_salaries_to_db(sqlite3 *db, const char *period) {
    // В расчете используется один аргумент period (например "2026-03") как на диаграмме
    char sql[512];
    sprintf(sql, "INSERT INTO SALARY_RECORDS (start_date, end_date, total_salary, calculated_at, driver_id) "
                 "SELECT '%s-01', '%s-31', SUM(cost * 0.2), datetime('now'), driver_id FROM ORDERS "
                 "WHERE date LIKE '%s%%' GROUP BY driver_id;", period, period, period);
    return sqlite3_exec(db, sql, NULL, NULL, NULL);
}
