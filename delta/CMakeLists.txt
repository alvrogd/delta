cmake_minimum_required(VERSION 3.0.0)


# Welcome to the Delta project!
project(delta VERSION 0.0.1)
set(CMAKE_BUILD_TYPE Release)


# Flex will be used to generate Delta's lexical analyzer
find_package(FLEX)
# Its specification file is "lexical.l"
# The resulting file will just be written to the build directory as it is just
# an auxiliary one
FLEX_TARGET(
    deltalex src/analyzers/lexical.l ${CMAKE_CURRENT_BINARY_DIR}/lexical.c
    COMPILE_FLAGS "--perf-report --perf-report"
)

# Bison will be used to generate Delta's syntactic and semantic analyzer
find_package(BISON)
# Its specification file is "synsem.y"
BISON_TARGET(
    deltaparser src/analyzers/synsem.y ${CMAKE_CURRENT_BINARY_DIR}/synsem.c
    COMPILE_FLAGS
        "--defines=include/common/lexical_comp.h --debug -Wall --report=solved"
)
ADD_FLEX_BISON_DEPENDENCY(deltalex deltaparser)


# You could also try another compiler, such as Clang
# (I actually have not tried any other than GCC)
set(CMAKE_C_COMPILER "gcc")

# How to compile...
set(CMAKE_C_STANDARD 99)
set(
    CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -Wall -Wextra -lm -std=c99 -O3 -Wno-unused-parameter -Wl,--no-as-needed -ldl"
)


# All header files and source files
# Includes the source file that results from building the lexical analyzer
# with Flex
set(
    HEADERS
    include/analyzers/lexical.h include/analyzers/synsem.h include/common/lexical_comp.h
    include/common/commands.h include/common/errors.h include/common/symbol_table.h
    include/lib/uthash.h
    include/math/dec_numbers.h include/math/math_constants.h include/math/math_functions.h
)
set(
    SOURCES
    src/main.c
    ${FLEX_deltalex_OUTPUTS} ${BISON_deltaparser_OUTPUTS}
    src/common/commands.c src/common/errors.c src/common/symbol_table.c
    src/math/dec_numbers.c src/math/math_constants.c src/math/math_functions.c
)


# The main target is the Delta's executable
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} m) # math.h


# There are two additional targets, one for each dynamic library which is
# provided for testing purposes
add_library(dl_radians SHARED test/dynamic_libraries/dl_radians.c)
add_library(dl_degrees SHARED test/dynamic_libraries/dl_degrees.c)
