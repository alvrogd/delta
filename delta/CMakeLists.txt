cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(delta VERSION 0.1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=c99 -O3 -I../include")

add_executable(delta src/main.c src/io/io_system.c include/io/io_system.h src/common/symbol_table.c include/common/symbol_table.h src/analyzers/lexical.c include/analyzers/lexical.h include/common/keywords.h include/common/lexical_components.h include/common/uthash.h)

