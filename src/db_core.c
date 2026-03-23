#include <stdio.h>
#include <string.h>
#include "db_core.h"

int db_connect(const char *filename, sqlite3 **db)
{
    int rc = sqlite3_open(filename, db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "[DB] Ошибка открытия '%s': %s\n",
                filename, sqlite3_errmsg(*db));
        return -1;
    }
    sqlite3_exec(*db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);
    printf("[DB] Подключение к '%s' установлено.\n", filename);
    return 0;
}

void db_close(sqlite3 *db)
{
    if (db)
    {
        sqlite3_close(db);
        printf("[DB] Соединение закрыто.\n");
    }
}