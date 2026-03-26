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

int calculate_salaries_to_db(sqlite3 *db, const char *start_date, const char *end_date) {
    int rc;

    {
        sqlite3_stmt *del_stmt = NULL;
        const char *del_sql = "DELETE FROM SALARY_RECORDS WHERE start_date=? AND end_date=?;";
        if (sqlite3_prepare_v2(db, del_sql, -1, &del_stmt, NULL) != SQLITE_OK) return -1;
        sqlite3_bind_text(del_stmt, 1, start_date, -1, SQLITE_STATIC);
        sqlite3_bind_text(del_stmt, 2, end_date, -1, SQLITE_STATIC);
        rc = sqlite3_step(del_stmt);
        sqlite3_finalize(del_stmt);
        if (rc != SQLITE_DONE) return -1;
    }

    {
        sqlite3_stmt *ins_stmt = NULL;
        const char *ins_sql =
            "INSERT INTO SALARY_RECORDS (start_date, end_date, total_salary, calculated_at, driver_id) "
            "SELECT ?, ?, COALESCE(SUM(o.cost * 0.2), 0), datetime('now'), d.personnel_number "
            "FROM DRIVERS d "
            "LEFT JOIN ORDERS o "
            "  ON o.driver_id = d.personnel_number "
            " AND o.date BETWEEN ? AND ? "
            "GROUP BY d.personnel_number;";
        if (sqlite3_prepare_v2(db, ins_sql, -1, &ins_stmt, NULL) != SQLITE_OK) return -1;

        sqlite3_bind_text(ins_stmt, 1, start_date, -1, SQLITE_STATIC);
        sqlite3_bind_text(ins_stmt, 2, end_date, -1, SQLITE_STATIC);
        sqlite3_bind_text(ins_stmt, 3, start_date, -1, SQLITE_STATIC);
        sqlite3_bind_text(ins_stmt, 4, end_date, -1, SQLITE_STATIC);

        rc = sqlite3_step(ins_stmt);
        sqlite3_finalize(ins_stmt);
        return (rc == SQLITE_DONE) ? 0 : -1;
    }
}
