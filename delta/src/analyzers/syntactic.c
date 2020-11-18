/**
 * @file syntactic.c
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief Implementation of analyzers/syntactic.h
 */


#include "analyzers/syntactic.h"
#include "common/errors.h"

#include <stdio.h>
#include <stdlib.h>


/**
 * @brief Globally accessible syntactic analyzer that will be used.
 */
struct d_syntactic_analyzer *syntactic_analyzer = NULL;


/**
 * @brief Represents a syntactic analyzer.
 *
 * @details
 *  Data type which represents a syntactic analyzer.
 */
struct d_syntactic_analyzer {
    // Empty as of now
};


/**
 * @brief Implementation of syntactic.h/d_syntactic_analyzer_initialize
 */
int d_syntactic_analyzer_initialize()
{
    // The structure that represents the syntactic analyzer must be allocated
    if((syntactic_analyzer = malloc(sizeof(struct d_syntactic_analyzer))) ==
       NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED, "syntactic.c",
                               "d_syntactic_analyzer_initialize",
                               "'malloc' for struct d_syntactic_analyzer");
        return -1;
    }


    return 0;
}


/**
 * @brief Implementation of syntactic.h/d_syntactic_analyzer_parse
 */
int d_syntactic_analyzer_parse()
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
    d_lexical_analyzer_get_next_lexical_comp(&tmp_lexical_component);

    while(tmp_lexical_component.category != D_LC_EOF) {

        d_lexical_analyzer_show_lexical_comp(&tmp_lexical_component);
        d_lexical_analyzer_destroy_lexical_com(&tmp_lexical_component);
        
        d_lexical_analyzer_get_next_lexical_comp(&tmp_lexical_component);
    }


    return 0;
}


/**
 * @brief Implementation of syntactic.h/d_syntactic_analyzer_destroy
 */
int d_syntactic_analyzer_destroy()
{
    if(syntactic_analyzer == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL, "syntactic.c",
                               "d_syntactic_analyzer_destroy",
                               "'syntactic_analyzer'");
        return -1;
    }


    // The structure that represents the syntactic analyzer needs to be freed
    free(syntactic_analyzer);


    return 0;
}
