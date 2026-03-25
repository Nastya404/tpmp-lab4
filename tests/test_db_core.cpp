#include <gtest/gtest.h>

extern "C" {
#include "../include/db_core.h"
}

TEST(DBCoreTest, ConnectSuccess) {
    sqlite3 *db;
    int rc = db_connect("test.db", &db);
    EXPECT_EQ(rc, 0);
    db_close(db);
}

TEST(DBCoreTest, InitFromFileFail) {
    sqlite3 *db;
    db_connect("test.db", &db);

    int rc = db_init_from_file(db, "non_existing.sql");
    EXPECT_EQ(rc, -1);

    db_close(db);
}

TEST(DBCoreTest, CloseDB) {
    sqlite3 *db;
    db_connect("test.db", &db);

    db_close(db);
    SUCCEED(); // если не упало — ок
}
