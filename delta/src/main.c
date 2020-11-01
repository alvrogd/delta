#include <stdio.h>
#include <stdlib.h>

#include "io/io_system.h"
#include "common/symbol_table.h"
#include "analyzers/lexical.h"
#include "analyzers/syntactic.h"
#include "common/lexical_components.h"
#include "common/errors.h"


int main(int argc, char *argv[])
{
    // The main function is responsible for initializing all the components of
    // the Delta compiler
    struct d_io_system *io_system = NULL;
    struct d_symbol_table *symbol_table = NULL;
    struct d_lexical_analyzer *lexical_analyzer = NULL;
    struct d_syntactic_analyzer *syntactic_analyzer = NULL;


    /* Initialization */

    if(d_io_system_initialize(&io_system, 4096) != 0) {
        exit(EXIT_FAILURE);
    }

    if(d_io_system_open_file(io_system, argv[1]) != 0) {
        exit(EXIT_FAILURE);
    }


    if(d_symbol_table_initialize(&symbol_table) != 0) {
        exit(EXIT_FAILURE);
    }


    if(d_lexical_analyzer_initialize(&lexical_analyzer) != 0) {
        exit(EXIT_FAILURE);   
    }

    if(d_lexical_analyzer_prepare_for_parsing(lexical_analyzer, io_system,
                                              symbol_table) != 0) {
        exit(EXIT_FAILURE);
    }


    if(d_syntactic_analyzer_initialize(&syntactic_analyzer) != 0) {
        exit(EXIT_FAILURE);
    }

    if(d_syntactic_analyzer_prepare_for_parsing(syntactic_analyzer, io_system,
                                                symbol_table,
                                                lexical_analyzer) != 0) {
        exit(EXIT_FAILURE);
    }


    /* Input file parsing */

    // Running the syntactic analyzer, which will call by itself the lexical
    // analyzer
    d_syntactic_analyzer_parse(syntactic_analyzer);


    /* Clean up */

    d_syntactic_analyzer_destroy(&syntactic_analyzer);

    d_lexical_analyzer_destroy(&lexical_analyzer);

    d_symbol_table_destroy(&symbol_table);

    d_io_system_destroy(&io_system);
    

    return 0;
}
