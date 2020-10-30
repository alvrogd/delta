#include <stdio.h>


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


    // Initializing the I/O system
    d_io_system_initialize(&io_system, 64);
    d_io_system_open_file(io_system, "regression_w_more_errors.d");

    // Initializing the symbol table
    d_symbol_table_initialize(&symbol_table);

    // Initializing the lexical analyzer
    d_lexical_analyzer_initialize(&lexical_analyzer);
    d_lexical_analyzer_prepare_for_parsing(lexical_analyzer, io_system,
                                           symbol_table);

    // Initializing the syntactic analyzer
    d_syntactic_analyzer_initialize(&syntactic_analyzer);
    d_syntactic_analyzer_prepare_for_parsing(syntactic_analyzer, io_system,
                                             symbol_table, lexical_analyzer);


    // Running the syntactic analyzer, which will call by itself the lexical
    // analyzer
    d_syntactic_analyzer_parse(syntactic_analyzer);


    return 0;
}
