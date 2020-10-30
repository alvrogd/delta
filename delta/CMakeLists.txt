cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(delta VERSION 0.0.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=c99 -O3 -Wno-unused-parameter")

set(HEADERS include/io/io_system.h include/common/symbol_table.h include/common/errors.h include/analyzers/lexical.h include/common/keywords.h include/common/lexical_components.h include/lib/uthash.h)
set(SOURCES src/main.c src/io/io_system.c src/common/symbol_table.c src/common/errors.c src/analyzers/lexical.c src/common/lexical_components.c)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)
