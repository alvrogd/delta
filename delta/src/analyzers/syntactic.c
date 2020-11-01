/**
 * @file syntactic.c
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief Implementation of analyzers/syntactic.h
 */


#include "analyzers/syntactic.h"
#include "common/lexical_components.h"
#include "common/errors.h"

#include <stdio.h>
#include <stdlib.h>


/**
 * @brief Represents a syntactic analyzer.
 *
 * @details
 *  Data type which represents a syntactic analyzer.
 */
struct d_syntactic_analyzer {
    /** Which I/O system will provide the source file that will be
        analyzed. */
    struct d_io_system *io_system;
    /** Which symbol table will be used during the compilation process. */
    struct d_symbol_table *symbol_table;
    /** Which lexical analyzer will provide all the lexical components in the
        input file. */
    struct d_lexical_analyzer *lexical_analyzer;
};


/**
 * @brief Implementation of syntactic.h/d_syntactic_analyzer_initialize
 */
int d_syntactic_analyzer_initialize(
    struct d_syntactic_analyzer **syntactic_analyzer
)
{
    if(syntactic_analyzer == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL, "syntactic.c",
                               "d_syntactic_analyzer_initialize",
                               "'syntactic_analyzer'");
        return -1;
    }


    // The structure that represents the syntactic analyzer must be allocated
    if((*syntactic_analyzer = malloc(sizeof(struct d_syntactic_analyzer))) ==
       NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED, "syntactic.c",
                               "d_syntactic_analyzer_initialize",
                               "'malloc' for struct d_syntactic_analyzer");
        return -1;
    }

    // The I/O system, symbol table and lexical analyzer will be provided
    // later on.


    return 0;
}


/**
 * @brief Implementation of
 *        syntactic.h/d_syntactic_analyzer_prepare_for_parsing
 */
int d_syntactic_analyzer_prepare_for_parsing(
    struct d_syntactic_analyzer *syntactic_analyzer,
    struct d_io_system *io_system,
    struct d_symbol_table *symbol_table,
    struct d_lexical_analyzer *lexical_analyzer
)
{
    if(syntactic_analyzer == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL, "syntactic.c",
                               "d_syntactic_analyzer_prepare_for_parsing",
                               "'syntactic_analyzer'");
        return -1;
    }

    if(io_system == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL, "syntactic.c",
                               "d_syntactic_analyzer_prepare_for_parsing",
                               "'io_system'");
        return -1;
    }

    if(symbol_table == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL, "syntactic.c",
                               "d_syntactic_analyzer_prepare_for_parsing",
                               "'symbol_table'");
        return -1;
    }

    if(lexical_analyzer == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL, "syntactic.c",
                               "d_syntactic_analyzer_prepare_for_parsing",
                               "'lexical_analyzer'");
        return -1;
    }


    // The referenced utilities just need to be stored in the lexical analyzer
    syntactic_analyzer->io_system = io_system;
    syntactic_analyzer->symbol_table = symbol_table;
    syntactic_analyzer->lexical_analyzer = lexical_analyzer;


    return 0;
}


/**
 * @brief Implementation of syntactic.h/d_syntactic_analyzer_parse
 */
int d_syntactic_analyzer_parse(
    struct d_syntactic_analyzer *syntactic_analyzer
)
{
    struct d_lexical_component tmp_lexical_component;


    if(syntactic_analyzer == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL, "syntactic.c",
                               "d_syntactic_analyzer_parse",
                               "'syntactic_analyzer'");
        return -1;
    }


    // Printing out all lexical components:
    //   - String that represents the category.
    //   - Corresponding lexeme if it had to be saved
    while(!d_io_system_is_eof(syntactic_analyzer->io_system)) {

        if(d_lexical_analyzer_get_next_lexical_comp(
                                         syntactic_analyzer->lexical_analyzer,
                                                    &tmp_lexical_component)
           == 0)
        {

            printf("<%s", d_lc_to_string(tmp_lexical_component.category));


            // The lexeme will be stored as its attribute, if any
            if(tmp_lexical_component.attributes != NULL) {

                // If its attribute is its lexeme
                if(tmp_lexical_component.category / D_LC_DISTANCE_CATEGORY ==
                D_LC_LITERAL / D_LC_DISTANCE_CATEGORY) {

                    printf(", %s", (const char *)
                                   tmp_lexical_component.attributes);
                }

                // If its attribute is a pointer to its entry in the symbol table
                else if(tmp_lexical_component.category /
                            D_LC_DISTANCE_CATEGORY ==
                        D_LC_KEYWORD / D_LC_DISTANCE_CATEGORY ||
                        
                        tmp_lexical_component.category /
                            D_LC_DISTANCE_CATEGORY ==
                        D_LC_IDENTIFIER / D_LC_DISTANCE_CATEGORY) {

                    printf(", %s",
                        ((struct d_symbol_table_entry *)
                         tmp_lexical_component.attributes)->lexeme);
                }
            }


            printf(">\n");

            d_lexical_analyzer_destroy_lexical_com(
                                         syntactic_analyzer->lexical_analyzer,
                                                   &tmp_lexical_component);
        }
    }


    return 0;
}


/**
 * @brief Implementation of syntactic.h/d_syntactic_analyzer_destroy
 */
int d_syntactic_analyzer_destroy(
    struct d_syntactic_analyzer **syntactic_analyzer
)
{
    if(syntactic_analyzer == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL, "syntactic.c",
                               "d_syntactic_analyzer_destroy",
                               "'syntactic_analyzer'");
        return -1;
    }


    // The structure that represents the syntactic analyzer needs to be freed
    free(*syntactic_analyzer);


    return 0;
}
