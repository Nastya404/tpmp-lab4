#ifndef DB_CORE_H
#define DB_CORE_H
#include "sqlite3.h"


int db_connect(const char *filename, sqlite3 **db);
int db_init_from_file(sqlite3 *db, const char *sql_filepath);
void db_close(sqlite3 *db);

#endif 
