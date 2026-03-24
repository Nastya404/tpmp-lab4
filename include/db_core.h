#ifndef DB_CORE_H
#define DB_CORE_H
#include "sqlite3.h"


int db_connect(const char *filename, sqlite3 **db);

void db_close(sqlite3 *db);

#endif /* DB_CORE_H */
