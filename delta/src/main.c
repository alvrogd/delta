#include <stdio.h>


#include "io/io_system.h"
#include "common/symbol_table.h"
#include "analyzers/lexical.h"
#include "common/lexical_components.h"
#include "common/errors.h"


int main(int argc, char *argv[])
{
    struct d_io_system *io_system = NULL;
    struct d_symbol_table *symbol_table = NULL;
    struct d_lexical_analyzer *lexical_analyzer = NULL;
    struct d_lexical_component tmp_lexical_component;

    int i = 0;
    unsigned char tmp_char = 0;


    // Initializing the I/O system
    d_io_system_initialize(&io_system, 64);
    d_io_system_open_file(io_system, "regression.d");

    // Initializing the symbol table
    d_symbol_table_initialize(&symbol_table);

    // Initializing the lexical analyzer
    d_lexical_analyzer_initialize(&lexical_analyzer);
    d_lexical_analyzer_prepare_for_parsing(lexical_analyzer, io_system,
                                           symbol_table);


    // Some tests

    // 1. If the keywords have been properly added to the symbol table
    // printf("Searching for \"double\": %d\n",
    //        d_symbol_table_search(symbol_table, "double") != NULL);
    // printf("Searching for \"int\": %d\n",
    //        d_symbol_table_search(symbol_table, "int") != NULL);
    // printf("Searching for \"potato\": %d\n",
    //        d_symbol_table_search(symbol_table, "potato") != NULL);


    // 2. Printing the first characters of the input file
    // for(i = 0; i < 20; ++i) {
    //     d_io_system_get_next_char(io_system, &tmp_char);
    //     printf("Char %d: %c\n", i, tmp_char);
    // }


    // 3. Printing all characters in the input file
    // while(!d_io_system_is_eof(io_system)) {

    //     d_io_system_get_next_char(io_system, &tmp_char);
    //     printf("%c", tmp_char);
    // }


    // 4. Print the first character
    //    For every remaining character
    //      Read it
    //      Turn it back
    //      Read it once more and print it

    // i = 0; // First character will not be returned

    // while(!d_io_system_is_eof(io_system)) {

    //     d_io_system_get_next_char(io_system, &tmp_char);

    //     // Print
    //     if(i == 0) {
    //         printf("%c", tmp_char);
    //         i = 1;
    //     }

    //     // Return
    //     else {
    //         d_io_system_return_char(io_system, tmp_char);
    //         i = 0;
    //     }
    // }


    // 5. Print out all lexical components as their category's ID
    // while(!d_io_system_is_eof(io_system)) {

    //     d_lexical_analyzer_get_next_lexical_comp(lexical_analyzer,
    //                                              &tmp_lexical_component);

    //     printf("Lex. comp.: %d\n", tmp_lexical_component.category);
    // }


    // 6. Print out all lexical components:
    //      - String that represents the category.
    //      - Corresponding lexeme if it had to be saved
    while(!d_io_system_is_eof(io_system)) {

        d_lexical_analyzer_get_next_lexical_comp(lexical_analyzer,
                                                 &tmp_lexical_component);

        printf("<%s", d_lc_to_string(tmp_lexical_component.category));

        // If its attribute is its lexeme
        if(tmp_lexical_component.category / D_LC_DISTANCE_CATEGORY ==
           D_LC_LITERAL / D_LC_DISTANCE_CATEGORY) {

            printf(", %s", (const char *)tmp_lexical_component.attributes);
        }

        // If its attribute is a pointer to its entry in the symbol table
        else if(tmp_lexical_component.category / D_LC_DISTANCE_CATEGORY ==
                D_LC_KEYWORD / D_LC_DISTANCE_CATEGORY ||
                tmp_lexical_component.category / D_LC_DISTANCE_CATEGORY ==
                D_LC_IDENTIFIER / D_LC_DISTANCE_CATEGORY) {

            printf(", %s",
                   ((struct d_symbol_table_entry *)tmp_lexical_component.attributes)->lexeme);
        }

        printf(">\n");
    }


    // 7. Error utility testing
    //d_errors_parse_show(4, 10, 2, 3, "una", "prueba");


    return 0;
}
