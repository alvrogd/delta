#include <stdio.h>
#include <stdlib.h>

#include "analyzers/lexical.h"
#include "analyzers/synsem.h"
#include "common/errors.h"
#include "common/symbol_table.h"


int main(int argc, char *argv[])
{
    /* Initialization */

    // The main function is responsible for initializing all the components of
    // delta

    if(d_symbol_table_initialize() != 0) {
        exit(EXIT_FAILURE);
    }


    if(d_lexical_analyzer_initialize() != 0) {
        exit(EXIT_FAILURE);   
    }

    if(d_lexical_analyzer_prepare_for_parsing(argv[1]) != 0) {
        exit(EXIT_FAILURE);
    }


    if(d_syntactic_analyzer_initialize() != 0) {
        exit(EXIT_FAILURE);
    }


    /* Input file parsing */

    // Showing what keywords the symbol table contains due to its
    // initialization
    if(d_symbol_table_show() != 0) {
        exit(EXIT_FAILURE);
    }

    // Running the syntactic analyzer, which will call by itself the lexical
    // analyzer
    if(d_syntactic_analyzer_parse() != 0) {
        exit(EXIT_FAILURE);
    }

    printf("\n");

    // Showing once again the keywords, as well as any identifiers that may
    // have been registered while parsing
    if(d_symbol_table_show() != 0) {
        exit(EXIT_FAILURE);
    }


    /* Clean up */

    d_syntactic_analyzer_destroy();

    d_lexical_analyzer_destroy();

    d_symbol_table_destroy();
    

    return 0;
}
