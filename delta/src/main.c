#include <stdio.h>

#include "io/io_system.h"
#include "common/symbol_table.h"


int main(int argc, char *argv[])
{
    struct d_io_system *io_system = NULL;
    struct d_symbol_table *symbol_table = NULL;

    int i = 0;
    unsigned char tmp_char = 0;


    // Initializing the I/O system
    d_io_system_initialize(&io_system, 8192);
    d_io_system_open_file(io_system, "regression.d");

    // Initializing the symbol table
    d_symbol_table_initialize(&symbol_table);


    // Some testing
    printf("Excuse me\n");

    printf("Searching for \"double\": %d\n",
           d_symbol_table_search(symbol_table, "double") != NULL);
    printf("Searching for \"int\": %d\n",
           d_symbol_table_search(symbol_table, "int") != NULL);
    printf("Searching for \"potato\": %d\n",
           d_symbol_table_search(symbol_table, "potato") != NULL);

    for(i = 0; i < 20; ++i) {
        d_io_system_get_next_char(io_system, &tmp_char);
        printf("Char %d: %c\n", i, tmp_char);
    }


    return 0;
}
