#include <stdio.h>
#include <stdlib.h>
#include "db_core.h"

int db_connect(const char *filename, sqlite3 **db) {
    int rc = sqlite3_open(filename, db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[DB] Ошибка открытия '%s': %s\n", filename, sqlite3_errmsg(*db));
        return -1;
    }
    sqlite3_exec(*db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);
    printf("[DB] Подключение к '%s' установлено.\n", filename);
    return 0;
}

static char* read_sql_file(const char *filepath) {
    FILE *f = fopen(filepath, "rb");
    if (!f) {
        perror("[DB] Не удалось открыть SQL файл");
        return NULL;
    }

    // Определяем размер файла
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buffer = malloc(length + 1);
    if (buffer) {
        fread(buffer, 1, length, f);
        buffer[length] = '\0';
    }
    fclose(f);
    return buffer;
}

int db_init_from_file(sqlite3 *db, const char *sql_filepath) {
    char *sql = read_sql_file(sql_filepath);
    if (!sql) return -1;

    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "[DB] Ошибка выполнения схемы из файла: %s\n", err_msg);
        sqlite3_free(err_msg);
        free(sql);
        return -1;
    }

    printf("[DB] Схема из файла '%s' успешно применена.\n", sql_filepath);
    free(sql);
    return 0;
}

void db_close(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
        printf("[DB] Соединение закрыто.\n");
    }
}
