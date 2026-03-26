#include <gtest/gtest.h>
#include <fstream>

extern "C" {
#include "../include/db_core.h"
}

TEST(DBCoreTest, ConnectSuccess) {
    sqlite3 *db;
    int rc = db_connect(":memory:", &db);
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

TEST(DBCoreTest, InitFromFileSuccess) {
    std::ofstream f("/tmp/test_schema.sql");
    f << "CREATE TABLE IF NOT EXISTS TEST_TABLE (id INTEGER PRIMARY KEY);";
    f.close();
    sqlite3 *db;
    db_connect(":memory:", &db);
    int rc = db_init_from_file(db, "/tmp/test_schema.sql");
    EXPECT_EQ(rc, 0);

    db_close(db);

}

TEST(DBCoreTest, ConnectFail) {
    sqlite3 *db;
    int rc = db_connect("/nonexistent/path/test.db", &db);
    EXPECT_NE(rc, 0);

}
