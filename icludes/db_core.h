#ifndef DB_CORE_H
#define DB_CORE_H

/* ========================================================
 *  db_core.h — DatabaseManager: подключение к БД
 *  Таблицы создаются заранее (Vertabelo), здесь только connect/close.
 * ======================================================== */

#include "sqlite3.h"

/* Открыть базу данных. Возвращает 0 при успехе. */
int db_connect(const char *filename, sqlite3 **db);

/* Корректно закрыть соединение. */
void db_close(sqlite3 *db);

#endif /* DB_CORE_H */
