# Исходники
set(SRC_FILES
    main.c        # Если main нужен
    db_core.c
    crud.c
    business_logic.c
    reports.c
)

# Создаём статическую библиотеку из исходников
add_library(autopark STATIC ${SRC_FILES})

# Пути к заголовкам
target_include_directories(autopark PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Подключаем SQLite3
find_package(SQLite3 REQUIRED)
target_include_directories(autopark PUBLIC ${SQLite3_INCLUDE_DIRS})
target_link_libraries(autopark PUBLIC SQLite::SQLite3)

# Если нужен исполняемый файл autopark с main
add_executable(autopark_exe main.c)
target_link_libraries(autopark_exe PRIVATE autopark)
target_include_directories(autopark_exe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

