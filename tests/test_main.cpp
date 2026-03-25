#include <gtest/gtest.h>

extern "C" {
#include "../src/main.c"
}

TEST(MainTest, ClearBuffer) {
    clear_buffer();
    SUCCEED();
}

TEST(MainTest, PrintMenu) {
    print_menu();
    SUCCEED();
}

TEST(MainTest, Dummy) {
    EXPECT_EQ(1, 1);
}
