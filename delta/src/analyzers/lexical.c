/**
 * @file lexical.c
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief Implementation of analyzers/lexical.h
 */


#include "analyzers/lexical.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


/**
 * @brief Represents a lexical analyzer.
 *
 * @details
 *  Data type which represents a lexical analyzer.
 */
struct d_lexical_analyzer {
    /** How many eol have been seen in the input file that is being
        analyzed. */
    size_t current_line;
    /** How many characters have been seen in the current line. */
    size_t current_character;
    /** Which I/O system will provide the source file that will be
        analyzed. */
    struct d_io_system *io_system;
    /** Which symbol table will be used during the compilation process. */
    struct d_symbol_table *symbol_table;
};


/**
 * @brief Implementation of lexical.h/d_lexical_analyzer_initialize
 */
int d_lexical_analyzer_initialize(
    struct d_lexical_analyzer **lexical_analyzer
)
{
    if(lexical_analyzer == NULL) {

        perror("ERROR::LEXICAL_ANALYZER::Reference to struct "
               "d_lexical_analyzer is NULL");
        return -1;
    }


    // The structure that represents the lexical analyzer must be allocated
    if((*lexical_analyzer = malloc(sizeof(struct d_lexical_analyzer))) ==
       NULL) {

        perror("ERROR::LEXICAL_ANALYZER::Could not allocate a struct "
               "d_lexical_analyzer");
        return -1;
    }

    // The "current_line" and "current_character" members will be initialized
    // each time that the lexical analyzer parses a certain source file.

    // The I/O system and symbol table will be provided later on.


    return 0;
}


/**
 * @brief Implementation of lexical.h/d_lexical_analyzer_prepare_for_parsing
 */
int d_lexical_analyzer_prepare_for_parsing(
    struct d_lexical_analyzer *lexical_analyzer,
    struct d_io_system *io_system,
    struct d_symbol_table *symbol_table
)
{
    if(lexical_analyzer == NULL) {

        perror("ERROR::LEXICAL_ANALYZER::Reference to lexical analyzer is "
               "NULL");
        return -1;
    }

    if(io_system == NULL) {

        perror("ERROR::LEXICAL_ANALYZER::Reference to I/O system is NULL");
        return -1;
    }

    if(symbol_table == NULL) {

        perror("ERROR::LEXICAL_ANALYZER::Reference to symbol table is NULL");
        return -1;
    }


    // The current parsing stats are reset
    lexical_analyzer->current_line = 1;
    lexical_analyzer->current_character = 1;

    // The referenced utilities just need to be stored in the lexical analyzer
    lexical_analyzer->io_system = io_system;
    lexical_analyzer->symbol_table = symbol_table;


    return 0;
}


/**
 * @brief Implementation of lexical.h/d_lexical_analyzer_get_next_lexical_comp
 */
int d_lexical_analyzer_get_next_lexical_comp(
    struct d_lexical_analyzer *lexical_analyzer,
    struct d_lexical_component *lexical_component
)
{
    unsigned char character;


    if(lexical_analyzer == NULL) {

        perror("ERROR::LEXICAL_ANALYZER::Reference to lexical analyzer is "
               "NULL");
        return -1;
    }

    if(lexical_component == NULL) {

        perror("ERROR::LEXICAL_ANALYZER::Reference to lexical component is "
               "NULL");
        return -1;
    }


    while(!d_io_system_is_eof(lexical_analyzer->io_system)) {

        d_io_system_get_next_char(lexical_analyzer->io_system,
                                  &character);

        printf("[D_LA] Line %zu, Col %zu: %c\n",
               lexical_analyzer->current_line,
               lexical_analyzer->current_character, character);

        // TODO automatas


        // Useful info for error raising purposes
        // These stats are updated to reflect the next character's ones
        if(character == '\n') {
            ++(lexical_analyzer->current_line);
            lexical_analyzer->current_character = 1;
        }

        else {
            ++(lexical_analyzer->current_character);
        }
    }


    return 0;
}


/**
 * @brief Implementation of lexical.h/d_lexical_analyzer_destroy
 */
int d_lexical_analyzer_destroy(
    struct d_lexical_analyzer **lexical_analyzer
)
{
    if(lexical_analyzer == NULL) {

        perror("ERROR::LEXICAL_ANALYZER::Reference to struct "
               "d_lexical_analyzer is NULL");
        return -1;
    }


    // The structure that represents the lexical analyzer needs to be freed
    free(*lexical_analyzer);


    return 0;
}
