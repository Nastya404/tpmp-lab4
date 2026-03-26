# Включаем FetchContent для GTest
include(FetchContent)

FetchContent_Declare(
    googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.0.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

# Файлы тестов
set(TEST_SOURCES
    test_db_core.cpp
    test_crud.cpp
    test_business_logic.cpp
    test_reports.cpp
)

# Создаём исполняемый файл тестов
add_executable(runTests ${TEST_SOURCES})

# Линкуем с GTest и библиотекой autopark
target_link_libraries(runTests
    PRIVATE
    autopark
    gtest_main
    pthread
)

# Пути к заголовкам
target_include_directories(runTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

# Добавляем тесты в CTest
include(GoogleTest)
gtest_discover_tests(runTests)

