cmake_minimum_required(VERSION 3.16)
project(Autopark C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Включаем поддиректории
add_subdirectory(src)
add_subdirectory(tests)

