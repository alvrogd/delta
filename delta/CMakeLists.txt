cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_C_COMPILER "gcc")

project(delta VERSION 0.1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=c99 -O3 -I../include")

add_executable(delta src/main.c src/io/io_system.c)

