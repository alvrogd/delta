/**
 * @file lexical.c
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief Implementation of analyzers/lexical.h
 *
 * @details
 *  All regular expressions that have been used here are detailed in
 *  0_lexical_components
 */


#include "analyzers/lexical.h"

#include "common/lexical_components.h" 
#include "common/errors.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Character categorizing functions such as "isalpha()"
#include <ctype.h>
#include <string.h>


// ═══════════════════════════════════════════════════════════════════════════
// ════════════════════════ SECTION: Finite Automatas ════════════════════════
// ═══════════════════════════════════════════════════════════════════════════

/** Generic function definition that allows transition functions to use
    pointers to themselves. A "d_transition_function" cannot be present in its
    prototype definition due to recursion definition errors. */
typedef int (*d_generic_function)();
/** Prototype definition for every function that represents a finite 
    automata. **/
typedef int (*d_transition_function)(int, unsigned char, size_t, size_t,
                                     d_generic_function *, int *, int *,
                                     int *, int *, int *);


/**
 * @brief Processes a certain character according to the comment and division
 *        operator automata.
 *
 * @details
 *  Processes a certain character according to the finite automata which 
 *  recognizes comments and division operators. The decision is also
 *  determined depending on the automata's current state.
 * 
 * @param[in] current_state Which state the automata is currently in.
 * @param[in] input_symbol Which symbol the automata must process.
 * @param[in] input_symbol_line In which line of the input file the symbol has
 *                              been found.
 * @param[in] input_symbol_character Which character the symbol is in its line
 *                                   in the input file.
 * @param[out] new_transition_function Which automata will be able to continue
 *                                     processing the input.
 * @param[out] new_state To which state the automata has transitioned.
 * @param[out] continue_parsing True if the automata has not recognized a
 *                              lexeme yet (or is trying to recognize a longer
 *                              one).
 * @param[out] return_character If the character that has been processed must
 *                              be returned to the I/O system in order to
 *                              process it later on.
 * @param[out] save_lexeme If the lexeme of the identified component needs to
 *                         be preserved along it (just literals and IDs/
                           keywords as of now).
 * @param[out] add_to_symbol_table If the identified component must be checked
 *                                 against the symbol table, in order to add
 *                                 its entry if not present yet (just IDs/
 *                                 keywords as of now).
 *
 * @return < 0 if no lexical component has been recognized, or its value
 *         otherwise, according to lexical_components.h
 */
int _d_lexical_analyzer_automata_comment_and_div(
    int current_state,
    unsigned char input_symbol,
    size_t input_symbol_line,
    size_t input_symbol_character,
    d_generic_function *transition_function,
    int *new_state,
    int *continue_parsing,
    int *return_character,
    int *save_lexeme,
    int *add_to_symbol_table
)
{
    // Because a standard finite automata will not be able to detect nested
    // comments
    static int active_nested_comments = 0;


    switch (current_state) {
        
        case 0:

            switch (input_symbol) {
            
                case '/':
                    // go to state 10
                    *new_state = 10;
                    *continue_parsing = 1;

                    return -1;

                case '*':
                    // go to state 20
                    *new_state = 20;
                    *continue_parsing = 1;

                    return -1;

                case '+':
                    ++active_nested_comments;

                    // go to state 30
                    *new_state = 30;
                    *continue_parsing = 1;

                    return -1;


                default:
                    // Successful recognition
                    *continue_parsing = 0;
                    *return_character = 1;
                    
                    return D_LC_OP_ARITHMETIC_DIV;
            }


        case 10:

            switch (input_symbol) {
            
                case '\n':
                    // Successful recognition
                    *continue_parsing = 0;
                    
                    return D_LC_WHITESPACE_COMMENT;

                default:
                    // loop
                    *new_state = 10;
                    *continue_parsing = 1;

                    return -1;
            }


        case 20:

            switch (input_symbol) {
            
                case '*':
                    // go to state 21
                    *new_state = 21;
                    *continue_parsing = 1;

                    return -1;

                default:
                    // loop
                    *new_state = 20;
                    *continue_parsing = 1;

                    return -1;
            }


        case 21:

            switch (input_symbol) {
            
                case '/':
                    // Successful recognition
                    *continue_parsing = 0;
                    
                    return D_LC_WHITESPACE_COMMENT;

                case '*':
                    // loop
                    *new_state = 21;
                    *continue_parsing = 1;

                    return -1;

                default:
                    // go to state 20
                    *new_state = 20;
                    *continue_parsing = 1;

                    return -1;
            }


        case 30:

            switch (input_symbol) {
            
                case '+':
                    // go to state 31
                    *new_state = 31;
                    *continue_parsing = 1;

                    return -1;

                case '/':
                    // go to state 32
                    *new_state = 32;
                    *continue_parsing = 1;

                    return -1;

                default:
                    // loop
                    *new_state = 30;
                    *continue_parsing = 1;

                    return -1;
            }


        case 31:

            switch (input_symbol) {
            
                case '/':
                    --active_nested_comments;

                    if(active_nested_comments == 0) {

                        // Successful recognition
                        *continue_parsing = 0;
                        
                        return D_LC_WHITESPACE_COMMENT;
                    }

                    else {
                        // go to state 30
                        *new_state = 30;
                        *continue_parsing = 1;

                        return -1;
                    }

                case '+':
                    // loop
                    *new_state = 31;
                    *continue_parsing = 1;

                    return -1;
                    
                default:
                    // go to state 30
                    *new_state = 30;
                    *continue_parsing = 1;

                    return -1;
            }


        case 32:

            switch (input_symbol) {
            
                case '+':
                    ++active_nested_comments;

                    // go to state 30
                    *new_state = 30;
                    *continue_parsing = 1;

                    return -1;

                default:
                    // go to state 30
                    *new_state = 30;
                    *continue_parsing = 1;

                    return -1;
            }


        default: // Just so that the compiler does not panic

            // Failure
            *continue_parsing = 0;
            *return_character = 1;

            return -1;
    }
}


/**
 * @brief Processes a certain character according to the double quoted string
 *        automata.
 *
 * @details
 *  Processes a certain character according to the finite automata which 
 *  recognizes double quoted strings. The decision is also determined
 *  depending on the automata's current state.
 * 
 * @param[in] current_state Which state the automata is currently in.
 * @param[in] input_symbol Which symbol the automata must process.
 * @param[in] input_symbol_line In which line of the input file the symbol has
 *                              been found.
 * @param[in] input_symbol_character Which character the symbol is in its line
 *                                   in the input file.
 * @param[out] new_transition_function Which automata will be able to continue
 *                                     processing the input.
 * @param[out] new_state To which state the automata has transitioned.
 * @param[out] continue_parsing True if the automata has not recognized a
 *                              lexeme yet (or is trying to recognize a longer
 *                              one).
 * @param[out] return_character If the character that has been processed must
 *                              be returned to the I/O system in order to
 *                              process it later on.
 * @param[out] save_lexeme If the lexeme of the identified component needs to
 *                         be preserved along it (just literals and IDs/
                           keywords as of now).
 * @param[out] add_to_symbol_table If the identified component must be checked
 *                                 against the symbol table, in order to add
 *                                 its entry if not present yet (just IDs/
 *                                 keywords as of now).
 *
 * @return < 0 if no lexical component has been recognized, or its value
 *         otherwise, according to lexical_components.h
 */
int _d_lexical_analyzer_automata_double_quoted_string(
    int current_state,
    unsigned char input_symbol,
    size_t input_symbol_line,
    size_t input_symbol_character,
    d_generic_function *transition_function,
    int *new_state,
    int *continue_parsing,
    int *return_character,
    int *save_lexeme,
    int *add_to_symbol_table
)
{
    switch (current_state) {
        
        case 0:

            if(input_symbol != '"') {

                if(input_symbol == '\\') {
                    // go to state 2
                    *new_state = 2;
                    *continue_parsing = 1;

                    return -1;
                }

                else {
                    // go to state 1
                    *new_state = 1;
                    *continue_parsing = 1;

                    return -1;
                }
            }

            else {
                // Failure
                continue_parsing = 0;
                
                d_errors_parse_show(3, D_ERR_LEX_LITERALS_STRING_EMPTY,
                                    input_symbol_line,
                                    input_symbol_character);

                return -1;
            }

        case 1:

            if(input_symbol != '"') {

                if(input_symbol == '\\') {
                    // go to state 2
                    *new_state = 2;
                    *continue_parsing = 1;

                    return -1;
                }

                else {
                    // loop
                    *new_state = 1;
                    *continue_parsing = 1;

                    return -1;
                }
            }

            else {
                // Successful recognition
                *continue_parsing = 0;
                *save_lexeme = 1;
                
                return D_LC_LITERAL_STR;
            }

        case 2:

            // No matter what character goes in, it will always be valid

            // go to state 1
            *new_state = 1;
            *continue_parsing = 1;

            return -1;

        default: // Just so that the compiler does not panic

            // Failure
            *continue_parsing = 0;
            *return_character = 1;

            return -1;
    }
}


/**
 * @brief Processes a certain character according to the equals and assignment
 *        automata.
 *
 * @details
 *  Processes a certain character according to the finite automata which 
 *  recognizes equals and assigment operators. The decision is also determined
 *  depending on the automata's current state.
 * 
 * @param[in] current_state Which state the automata is currently in.
 * @param[in] input_symbol Which symbol the automata must process.
 * @param[in] input_symbol_line In which line of the input file the symbol has
 *                              been found.
 * @param[in] input_symbol_character Which character the symbol is in its line
 *                                   in the input file.
 * @param[out] new_transition_function Which automata will be able to continue
 *                                     processing the input.
 * @param[out] new_state To which state the automata has transitioned.
 * @param[out] continue_parsing True if the automata has not recognized a
 *                              lexeme yet (or is trying to recognize a longer
 *                              one).
 * @param[out] return_character If the character that has been processed must
 *                              be returned to the I/O system in order to
 *                              process it later on.
 * @param[out] save_lexeme If the lexeme of the identified component needs to
 *                         be preserved along it (just literals and IDs/
                           keywords as of now).
 * @param[out] add_to_symbol_table If the identified component must be checked
 *                                 against the symbol table, in order to add
 *                                 its entry if not present yet (just IDs/
 *                                 keywords as of now).
 *
 * @return < 0 if no lexical component has been recognized, or its value
 *         otherwise, according to lexical_components.h
 */
int _d_lexical_analyzer_automata_equals_and_assign(
    int current_state,
    unsigned char input_symbol,
    size_t input_symbol_line,
    size_t input_symbol_character,
    d_generic_function *transition_function,
    int *new_state,
    int *continue_parsing,
    int *return_character,
    int *save_lexeme,
    int *add_to_symbol_table
)
{
    switch (current_state) {
        
        case 0:

            switch (input_symbol) {
            
                case '=':
                    // Successful "equals" recognition
                    *continue_parsing = 0;

                    return D_LC_OP_RELATIONAL_EQUALS;

                default:
                    // Successful "assignment" recognition
                    *continue_parsing = 0;
                    *return_character = 1;

                    return D_LC_OP_ASSIGNMENT_ASSIGN;
            }


        default: // Just so that the compiler does not panic

            // Failure
            *continue_parsing = 0;
            *return_character = 1;

            return -1;
    }
}


/**
 * @brief Processes a certain character according to the increment and
 *        plus-assignment automata.
 *
 * @details
 *  Processes a certain character according to the finite automata which 
 *  recognizes increment and plus-assigment operators. The decision is also
 *  determined depending on the automata's current state.
 * 
 * @param[in] current_state Which state the automata is currently in.
 * @param[in] input_symbol Which symbol the automata must process.
 * @param[in] input_symbol_line In which line of the input file the symbol has
 *                              been found.
 * @param[in] input_symbol_character Which character the symbol is in its line
 *                                   in the input file.
 * @param[out] new_transition_function Which automata will be able to continue
 *                                     processing the input.
 * @param[out] new_state To which state the automata has transitioned.
 * @param[out] continue_parsing True if the automata has not recognized a
 *                              lexeme yet (or is trying to recognize a longer
 *                              one).
 * @param[out] return_character If the character that has been processed must
 *                              be returned to the I/O system in order to
 *                              process it later on.
 * @param[out] save_lexeme If the lexeme of the identified component needs to
 *                         be preserved along it (just literals and IDs/
                           keywords as of now).
 * @param[out] add_to_symbol_table If the identified component must be checked
 *                                 against the symbol table, in order to add
 *                                 its entry if not present yet (just IDs/
 *                                 keywords as of now).
 *
 * @return < 0 if no lexical component has been recognized, or its value
 *         otherwise, according to lexical_components.h
 */
int _d_lexical_analyzer_automata_increment_and_plus_assign(
    int current_state,
    unsigned char input_symbol,
    size_t input_symbol_line,
    size_t input_symbol_character,
    d_generic_function *transition_function,
    int *new_state,
    int *continue_parsing,
    int *return_character,
    int *save_lexeme,
    int *add_to_symbol_table
)
{
    switch (current_state) {
        
        case 0:

            switch (input_symbol) {
            
                case '+':
                    // Successful "increment" recognition
                    *continue_parsing = 0;

                    return D_LC_OP_ARITHMETIC_INCREMENT;

                case '=':
                    // Successful "plus-assignment" recognition
                    *continue_parsing = 0;

                    return D_LC_OP_ASSIGNMENT_PLUS_ASSIGN;

                default:
                    // failure
                    *continue_parsing = 0;
                    *return_character = 1;

                    d_errors_parse_show(4, D_ERR_LEX_OPERATORS_UNSUPPORTED,
                                       input_symbol_line,
                                       input_symbol_character, "'++' -OR- "
                                                               "'+='");

                    return -1;
            }


        default: // Just so that the compiler does not panic

            // Failure
            *continue_parsing = 0;
            *return_character = 1;

            return -1;
    }
}


/**
 * @brief Processes a certain character according to the whitespace automata.
 *
 * @details
 *  Processes a certain character according to the finite automata which 
 *  recognizes whitespace characters ('\f', '\n', '\r', '\t', '\v', ' '). The
 *  decision is also determined depending on the automata's current state.
 * 
 * @param[in] current_state Which state the automata is currently in.
 * @param[in] input_symbol Which symbol the automata must process.
 * @param[in] input_symbol_line In which line of the input file the symbol has
 *                              been found.
 * @param[in] input_symbol_character Which character the symbol is in its line
 *                                   in the input file.
 * @param[out] new_transition_function Which automata will be able to continue
 *                                     processing the input.
 * @param[out] new_state To which state the automata has transitioned.
 * @param[out] continue_parsing True if the automata has not recognized a
 *                              lexeme yet (or is trying to recognize a longer
 *                              one).
 * @param[out] return_character If the character that has been processed must
 *                              be returned to the I/O system in order to
 *                              process it later on.
 * @param[out] save_lexeme If the lexeme of the identified component needs to
 *                         be preserved along it (just literals and IDs/
                           keywords as of now).
 * @param[out] add_to_symbol_table If the identified component must be checked
 *                                 against the symbol table, in order to add
 *                                 its entry if not present yet (just IDs/
 *                                 keywords as of now).
 *
 * @return < 0 if no lexical component has been recognized, or its value
 *         otherwise, according to lexical_components.h
 */
int _d_lexical_analyzer_automata_whitespace(
    int current_state,
    unsigned char input_symbol,
    size_t input_symbol_line,
    size_t input_symbol_character,
    d_generic_function *transition_function,
    int *new_state,
    int *continue_parsing,
    int *return_character,
    int *save_lexeme,
    int *add_to_symbol_table
)
{
    switch (current_state) {
        
        case 0:

            if(isblank(input_symbol) | isspace(input_symbol)) {
                // loop
                *new_state = 0;
                *continue_parsing = 1;

                return -1;
            }

            else {
                // Successful recognition
                *continue_parsing = 0;
                *return_character = 1;

                return D_LC_WHITESPACE;
            }


        default: // Just so that the compiler does not panic

            // Failure
            *continue_parsing = 0;
            *return_character = 1;

            return -1;
    }
}


/**
 * @brief Processes a certain character according to the identifiers and
 *        keywords operator automata.
 *
 * @details
 *  Processes a certain character according to the finite automata which 
 *  recognizes both identifiers and keywords operators. The decision is also
 *  determined depending on the automata's current state.
 * 
 * @param[in] current_state Which state the automata is currently in.
 * @param[in] input_symbol Which symbol the automata must process.
 * @param[in] input_symbol_line In which line of the input file the symbol has
 *                              been found.
 * @param[in] input_symbol_character Which character the symbol is in its line
 *                                   in the input file.
 * @param[out] new_transition_function Which automata will be able to continue
 *                                     processing the input.
 * @param[out] new_state To which state the automata has transitioned.
 * @param[out] continue_parsing True if the automata has not recognized a
 *                              lexeme yet (or is trying to recognize a longer
 *                              one).
 * @param[out] return_character If the character that has been processed must
 *                              be returned to the I/O system in order to
 *                              process it later on.
 * @param[out] save_lexeme If the lexeme of the identified component needs to
 *                         be preserved along it (just literals and IDs/
                           keywords as of now).
 * @param[out] add_to_symbol_table If the identified component must be checked
 *                                 against the symbol table, in order to add
 *                                 its entry if not present yet (just IDs/
 *                                 keywords as of now).
 *
 * @return < 0 if no lexical component has been recognized, or its value
 *         otherwise, according to lexical_components.h
 */
int _d_lexical_analyzer_automata_id_and_kwd(
    int current_state,
    unsigned char input_symbol,
    size_t input_symbol_line,
    size_t input_symbol_character,
    d_generic_function *transition_function,
    int *new_state,
    int *continue_parsing,
    int *return_character,
    int *save_lexeme,
    int *add_to_symbol_table
)
{
    switch (current_state) {
        
        case 0:

            if(isalnum(input_symbol) || input_symbol == '_') {
                // loop
                *new_state = 0;
                *continue_parsing = 1;

                return -1;
            }

            else {
                // Successful recognition
                *continue_parsing = 0;
                *return_character = 1;
                *save_lexeme = 1;
                *add_to_symbol_table = 1;

                // The calling function must take care of telling keywords
                // apart from IDs, depending on what the symbol table reports
                return 0;
            }


        default: // Just so that the compiler does not panic

            // Failure
            *continue_parsing = 0;
            *return_character = 1;

            return -1;
    }
}


/**
 * @brief Processes a certain character according to the binary integer
 *        automata.
 *
 * @details
 *  Processes a certain character according to the finite automata which 
 *  recognizes binary integer numbers. The decision is also  determined
 *  depending on the automata's current state.
 * 
 * @param[in] current_state Which state the automata is currently in.
 * @param[in] input_symbol Which symbol the automata must process.
 * @param[in] input_symbol_line In which line of the input file the symbol has
 *                              been found.
 * @param[in] input_symbol_character Which character the symbol is in its line
 *                                   in the input file.
 * @param[out] new_transition_function Which automata will be able to continue
 *                                     processing the input.
 * @param[out] new_state To which state the automata has transitioned.
 * @param[out] continue_parsing True if the automata has not recognized a
 *                              lexeme yet (or is trying to recognize a longer
 *                              one).
 * @param[out] return_character If the character that has been processed must
 *                              be returned to the I/O system in order to
 *                              process it later on.
 * @param[out] save_lexeme If the lexeme of the identified component needs to
 *                         be preserved along it (just literals and IDs/
                           keywords as of now).
 * @param[out] add_to_symbol_table If the identified component must be checked
 *                                 against the symbol table, in order to add
 *                                 its entry if not present yet (just IDs/
 *                                 keywords as of now).
 *
 * @return < 0 if no lexical component has been recognized, or its value
 *         otherwise, according to lexical_components.h
 */
int _d_lexical_analyzer_automata_binary_number(
    int current_state,
    unsigned char input_symbol,
    size_t input_symbol_line,
    size_t input_symbol_character,
    d_generic_function *transition_function,
    int *new_state,
    int *continue_parsing,
    int *return_character,
    int *save_lexeme,
    int *add_to_symbol_table
)
{
    switch (current_state) {

        case 11:

            switch (input_symbol) {
            
                case '_':
                    // loop
                    *new_state = 11;
                    *continue_parsing = 1;

                    return -1;

                case '0':
                case '1':
                    // go to state 12
                    *new_state = 12;
                    *continue_parsing = 1;

                    return -1;

                default:
                    // Failure
                    *continue_parsing = 0;
                    *return_character = 1;

                    d_errors_parse_show(4,
                                       D_ERR_LEX_LITERALS_INTEGERS_BINARY_BAD,
                                        input_symbol_line,
                                        input_symbol_character,
                                        "a binary number needs at least one "
                                        "'0' -OR- '1' (i.e. 0b1)");

                    return -1;
            }


        case 12:

            if(input_symbol == '0' || input_symbol == '1' || input_symbol ==
               '_') {

                // loop
                *new_state = 12;
                *continue_parsing = 1;

                return -1;
            }

            else if(!isalpha(input_symbol)) {

                // Successful "binary integer" recognition
                *continue_parsing = 0;
                *return_character = 1;
                *save_lexeme = 1;

                return D_LC_LITERAL_INT;
            }

            else {
                // Failure
                *continue_parsing = 0;
                *return_character = 1;

                d_errors_parse_show(4, D_ERR_LEX_LITERALS_INTEGERS_BINARY_BAD,
                                    input_symbol_line,
                                    input_symbol_character,
                                    "a binary number may only have '0' -AND- "
                                    "'1' -AND- '_'");

                return -1;
            }


        default: // Just so that the compiler does not panic

            // Failure
            *continue_parsing = 0;
            *return_character = 1;

            return -1;
    }
}


/**
 * @brief Processes a certain character according to the floating number
 *        automata.
 *
 * @details
 *  Processes a certain character according to the finite automata which 
 *  recognizes floating point numbers. The decision is also determined
 *  depending on the automata's current state.
 * 
 * @param[in] current_state Which state the automata is currently in.
 * @param[in] input_symbol Which symbol the automata must process.
 * @param[in] input_symbol_line In which line of the input file the symbol has
 *                              been found.
 * @param[in] input_symbol_character Which character the symbol is in its line
 *                                   in the input file.
 * @param[out] new_transition_function Which automata will be able to continue
 *                                     processing the input.
 * @param[out] new_state To which state the automata has transitioned.
 * @param[out] continue_parsing True if the automata has not recognized a
 *                              lexeme yet (or is trying to recognize a longer
 *                              one).
 * @param[out] return_character If the character that has been processed must
 *                              be returned to the I/O system in order to
 *                              process it later on.
 * @param[out] save_lexeme If the lexeme of the identified component needs to
 *                         be preserved along it (just literals and IDs/
                           keywords as of now).
 * @param[out] add_to_symbol_table If the identified component must be checked
 *                                 against the symbol table, in order to add
 *                                 its entry if not present yet (just IDs/
 *                                 keywords as of now).
 *
 * @return < 0 if no lexical component has been recognized, or its value
 *         otherwise, according to lexical_components.h
 */
int _d_lexical_analyzer_automata_floating_number(
    int current_state,
    unsigned char input_symbol,
    size_t input_symbol_line,
    size_t input_symbol_character,
    d_generic_function *transition_function,
    int *new_state,
    int *continue_parsing,
    int *return_character,
    int *save_lexeme,
    int *add_to_symbol_table
)
{
    switch (current_state) {

        case 30:

            if(isdigit(input_symbol)) {
                // go to state 31
                *new_state = 31;
                *continue_parsing = 1;

                return -1;
            }

            else if(!isalpha(input_symbol)) {

                // Successful "decimal float" recognition
                *continue_parsing = 0;
                *return_character = 1;
                *save_lexeme = 1;

                return D_LC_LITERAL_FP;
            }

            else {
                // Failure
                *continue_parsing = 0;
                *return_character = 1;

                d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                    input_symbol_line, input_symbol_character,
                                    "the only letter that may follow a "
                                    "float's decimal dot is 'E' -OR- 'e', "
                                    "providing that there is at least a "
                                    "digit beforehand (i.e. 10.3E+15)");

                return -1;
            }


        case 31:

            if(isdigit(input_symbol) || input_symbol == '_') {
                // loop
                *new_state = 31;
                *continue_parsing = 1;    
            
                return -1;
            }

            else if(input_symbol == 'E' || input_symbol == 'e') {
                // go to state 32
                *new_state = 32;
                *continue_parsing = 1;

                return -1;
            }

            else if(!isalpha(input_symbol)) {

                // Successful "decimal float" recognition
                *continue_parsing = 0;
                *return_character = 1;
                *save_lexeme = 1;

                return D_LC_LITERAL_FP;
            }

            else {
                // Failure
                *continue_parsing = 0;
                *return_character = 1;

                d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                    input_symbol_line, input_symbol_character,
                                    "the only letter that may be in a "
                                    "float's decimal part is 'E' -OR- 'e' "
                                    "for its exponent (i.e. 10.3E+15)");

                return -1;
            }


        case 32:

            if(isdigit(input_symbol)) {
                // go to state 34
                *new_state = 34;
                *continue_parsing = 1;

                return -1;
            }

            else if(input_symbol == '+' || input_symbol == '-') {
                // go to state 33
                *new_state = 33;
                *continue_parsing = 1;

                return -1;
            }

            else {
                // Failure
                *continue_parsing = 0;
                *return_character = 1;

                d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                    input_symbol_line, input_symbol_character,
                                    "the only characters that may follow a "
                                    "float's 'E' -OR- 'e' are '+' -OR- '-' "
                                    "-OR- 'a digit' (i.e. 10.3E+15, "
                                    "10.3E10)");

                return -1;
            }


        case 33:

            if(isdigit(input_symbol)) {
                // go to state 34
                *new_state = 34;
                *continue_parsing = 1;

                return -1;
            }

            else {
                // Failure
                *continue_parsing = 0;
                *return_character = 1;

                d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                    input_symbol_line, input_symbol_character,
                                    "the only characters that may follow a "
                                    "float's exponent and sign declaration "
                                    "are digits (i.e. 10.3E+15, 10.3E10)");

                return -1;
            }


        case 34:

            if(isdigit(input_symbol) || input_symbol == '_') {
                // loop
                *new_state = 34;
                *continue_parsing = 1;    
            
                return -1;
            }

            else if(!isalpha(input_symbol)) {

                // Successful "decimal float" recognition
                *continue_parsing = 0;
                *return_character = 1;
                *save_lexeme = 1;

                return D_LC_LITERAL_FP;
            }

            else {
                // Failure
                *continue_parsing = 0;
                *return_character = 1;

                d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                    input_symbol_line,
                                    input_symbol_character,
                                    "a float's decimal number may only have "
                                    "'digits' -OR- '_'");

                return -1;
            }


        default: // Just so that the compiler does not panic

            // Failure
            *continue_parsing = 0;
            *return_character = 1;

            return -1;
    }
}


/**
 * @brief Processes a certain character according to the number and dot
 *        operator automata.
 *
 * @details
 *  Processes a certain character according to the finite automata which 
 *  recognizes both numbers and dot operators. The decision is also
 *  determined depending on the automata's current state.
 * 
 * @param[in] current_state Which state the automata is currently in.
 * @param[in] input_symbol Which symbol the automata must process.
 * @param[in] input_symbol_line In which line of the input file the symbol has
 *                              been found.
 * @param[in] input_symbol_character Which character the symbol is in its line
 *                                   in the input file.
 * @param[out] new_transition_function Which automata will be able to continue
 *                                     processing the input.
 * @param[out] new_state To which state the automata has transitioned.
 * @param[out] continue_parsing True if the automata has not recognized a
 *                              lexeme yet (or is trying to recognize a longer
 *                              one).
 * @param[out] return_character If the character that has been processed must
 *                              be returned to the I/O system in order to
 *                              process it later on.
 * @param[out] save_lexeme If the lexeme of the identified component needs to
 *                         be preserved along it (just literals and IDs/
                           keywords as of now).
 * @param[out] add_to_symbol_table If the identified component must be checked
 *                                 against the symbol table, in order to add
 *                                 its entry if not present yet (just IDs/
 *                                 keywords as of now).
 *
 * @return < 0 if no lexical component has been recognized, or its value
 *         otherwise, according to lexical_components.h
 */
int _d_lexical_analyzer_automata_number_and_dot(
    int current_state,
    unsigned char input_symbol,
    size_t input_symbol_line,
    size_t input_symbol_character,
    d_generic_function *transition_function,
    int *new_state,
    int *continue_parsing,
    int *return_character,
    int *save_lexeme,
    int *add_to_symbol_table
)
{
    switch (current_state) {
        
        case 0:

            if(isdigit(input_symbol)) {
                // go to state 31 (and switch to another automata)
                *transition_function = (d_generic_function)
                                 _d_lexical_analyzer_automata_floating_number;
                *new_state = 31;
                *continue_parsing = 1;

                return -1;
            }

            else {
                // Successful "dot operator" recognition
                *continue_parsing = 0;
                *return_character = 1;

                return D_LC_OP_ACCESS_DOT;
            }


        case 10:

            if(input_symbol == 'B' || input_symbol == 'b') {
                // go to state 11 (and switch to another automata)
                *transition_function = (d_generic_function)
                                   _d_lexical_analyzer_automata_binary_number;
                *new_state = 11;
                *continue_parsing = 1;

                return -1;
            }

            else if(isdigit(input_symbol) || input_symbol == '_') {
                // go to state 20
                *new_state = 20;
                *continue_parsing = 1;    
            
                return -1;
            }

            else if(input_symbol == '.') {
                // go to state 30 (and switch to another automata)
                *transition_function = (d_generic_function)
                                 _d_lexical_analyzer_automata_floating_number;
                *new_state = 30;
                *continue_parsing = 1; 
            
                return -1;
            }

            else if(input_symbol == 'E' || input_symbol == 'e') {
                // go to state 32 (and switch to another automata)
                *transition_function = (d_generic_function)
                                 _d_lexical_analyzer_automata_floating_number;
                *new_state = 32;
                *continue_parsing = 1;

                return -1;
            }

            else if(!isalpha(input_symbol)) {

                // Successful "decimal integer" recognition
                *continue_parsing = 0;
                *return_character = 1;
                *save_lexeme = 1;

                return D_LC_LITERAL_INT;
            }

            else {
                // Failure
                *continue_parsing = 0;
                *return_character = 1;

                d_errors_parse_show(4, D_ERR_LEX_LITERALS_INTEGERS_BAD,
                                    input_symbol_line, input_symbol_character,
                                    "the only letters that may follow an "
                                    "integer number are 'E' -OR- 'e' for "
                                    "floats (i.e. 10E+15) -AND- 'B' -OR- 'b' "
                                    " for binaries (i.e. 0b011)");

                return -1;
            }


        case 20:

            if(isdigit(input_symbol) || input_symbol == '_') {
                // loop
                *new_state = 20;
                *continue_parsing = 1;    
            
                return -1;
            }

            else if(input_symbol == '.') {
                // go to state 30 (and switch to another automata)
                *transition_function = (d_generic_function)
                                 _d_lexical_analyzer_automata_floating_number;
                *new_state = 30;
                *continue_parsing = 1;    
            
                return -1;
            }

            else if(input_symbol == 'E' || input_symbol == 'e') {
                // go to state 32 (and switch to another automata)
                *transition_function = (d_generic_function)
                                 _d_lexical_analyzer_automata_floating_number;
                *new_state = 32;
                *continue_parsing = 1;

                return -1;
            }

            else if(!isalpha(input_symbol)) {

                // Successful "decimal integer" recognition
                *continue_parsing = 0;
                *return_character = 1;
                *save_lexeme = 1;

                return D_LC_LITERAL_INT;
            }

            else {
                // Failure
                *continue_parsing = 0;
                *return_character = 1;

                d_errors_parse_show(4, D_ERR_LEX_LITERALS_INTEGERS_DECIMAL_BAD,
                                    input_symbol_line, input_symbol_character,
                                    "the only letter supported a decimal "
                                    "integer number is 'E' -OR- 'e' for "
                                    "floats (i.e. 10E+15); if you would like "
                                    "a binary number, it must start by '0b' "
                                    "(i.e. 0b011)");

                return -1;
            }


        default: // Just so that the compiler does not panic

            // Failure
            *continue_parsing = 0;
            *return_character = 1;

            return -1;
    }
}


// ═══════════════════════════════════════════════════════════════════════════
// ═════════════════════ END OF SECTION: Finite Automatas ════════════════════
// ═══════════════════════════════════════════════════════════════════════════



// ═══════════════════════════════════════════════════════════════════════════
// ════════════════════════ SECTION: Lexical Analyzer ════════════════════════
// ═══════════════════════════════════════════════════════════════════════════

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
 * @brief Updates the parsing stats depending on the last read character.
 *
 * @details
 *  Updates the parsing stats depending on the last read character. These
 *  stats are useful for error raising purposes, in order to point out to
 *  the user the error's precise location:
 *
 *    - Current line in the input file.
 *    - Current character in the current line.
 *
 *  The stats reflect the next character's properties; i.e. if an EOL is
 *  recognized, the line counter will already be incremented by 1 so that if
 *  the stats are used anytime in the future when parsing that next character,
 *  its corresponding line number is properly set.
 *
 * @param[in,out] lexical_analyzer The lexical analyzer.
 * @param[in] character The last character that has been read.
 * @param[in] has_been_returned If the last character had to be returned.
 */
void _d_lexical_analyzer_update_parsing_stats(
    struct d_lexical_analyzer *lexical_analyzer,
    unsigned char character,
    int has_been_returned
)
{
    // No error checking as this function is only called in this translation
    // unit, whose public functions have already made any relevant checks


    // If the character had to be returned, there is no need to update the
    // parsing stats, as they belong to that character, which will be read
    // once again later on
    if(!has_been_returned) {

        if(character == '\n') {
            ++(lexical_analyzer->current_line);
            lexical_analyzer->current_character = 1;
        }

        else {
            ++(lexical_analyzer->current_character);
        }
    }   
}





/**
 * @brief Tries to recognize a certain lexical component via a predefined
 *        automata.
 *
 * @details
 *  A specified finite automata is used to try to recognize its corresponding
 *  lexical components, by parsing the next characters that the I/O system
 *  provides.
 *
 *  This function will also take care of storing the component's lexeme and/or
 *  creating its entry in the symbol table if needed.
 *
 * @param[in,out] lexical_analyzer The lexical analyzer.
 * @param[out] lexical_analyzer Pointer to a struct d_lexical_analyzer to
 *                              which the lexical component's data will be
 *                              written.
 * @param[in] transition_function Pointer to the function that contains all
 *            the logical processes of the automata.
 * @param[in] initial_state Starting state of the automata.
 */
void _d_lexical_analyzer_run_automata(
    struct d_lexical_analyzer *lexical_analyzer,
    struct d_lexical_component *lexical_component,
    d_transition_function transition_function,
    int initial_state
)
{
    unsigned char current_character = 0;
    int return_character = 0;

    int current_automata_state = initial_state;

    int continue_parsing = 1; // As long as the automata desires to continue

    int save_lexeme = 0;
    const unsigned char *lexeme = NULL;

    int add_to_symbol_table = 0;
    struct d_symbol_table_entry entry;
    struct d_symbol_table_entry *entry_in_table = NULL;

    int lexical_component_id = 0;


    // No error checking as this function is only called in this translation
    // unit, whose public functions have already made any relevant checks


    if(d_io_system_is_eof(lexical_analyzer->io_system)) {

        perror("ERROR:LEXICAL_ANALYZER::EOF has been reached");
        continue_parsing = 0;
    }


    while(continue_parsing) {

        /* 0. Resetting some flags */
        return_character = 0;
        save_lexeme = 0;
        add_to_symbol_table = 0;
        lexical_component_id = 0;


        /* 1. Retrieve the next character */
        d_io_system_get_next_char(lexical_analyzer->io_system,
                                  &current_character);

        #ifdef D_DEBUG
        printf("[lexical_analyzer][run automata][continue parsing] Char "
               "%zu:%zu: %c\n", lexical_analyzer->current_line,
               lexical_analyzer->current_character, current_character);
        #endif


        /* 2. State transitioning depending on the retrieved character */
        lexical_component_id = (transition_function)(current_automata_state,
                                current_character,
                                lexical_analyzer->current_line,
                                lexical_analyzer->current_character,
                                (d_generic_function *) &transition_function,
                                &current_automata_state, &continue_parsing,
                                &return_character, &save_lexeme,
                                &add_to_symbol_table);

        if(return_character) {
            d_io_system_return_char(lexical_analyzer->io_system,
                                    current_character);
        }

        if(save_lexeme) {

            lexeme =
                 d_io_system_save_current_lexeme(lexical_analyzer->io_system);
                 
            // If the lexeme needed to be saved, at first it also needs to be
            // set as the component's attribute
            // If the component turns out to be an ID/keyword, this will just
            // get overwritten with a reference to its entry in the symbol
            // table, so everything would be still fine
            lexical_component->attributes = lexeme;

            #ifdef D_DEBUG
            printf("[lexical_analyzer][run automata][continue parsing] Saved "
               "lexeme: %s\n", lexeme);
            #endif
        }

        if(add_to_symbol_table) {

            // The symbol table requires the entry to be initialized before
            // adding it

            #ifdef D_DEBUG
            printf("[lexical_analyzer][run automata][continue parsing] "
                   "Symbol table entry key: %s\tKey length: %zu\n", lexeme,
                   strlen((const char *)lexeme));
            #endif
            // In order to do so, let's check first if a corresponding entry
            // is already present
            entry_in_table = d_symbol_table_search(
                                               lexical_analyzer->symbol_table,
                                               lexeme);

            if(entry_in_table == NULL) {

                #ifdef D_DEBUG
                printf("[lexical_analyzer][run automata][continue parsing] "
                       "Key not present yet\n");
                #endif

                // If it is not already present, it definitely cannot be a
                // keyword
                entry.lexeme = lexeme;
                entry.lexical_component = D_LC_IDENTIFIER;

                d_symbol_table_add(lexical_analyzer->symbol_table,
                                   &entry);

                // Due to "entry" being restricted to this function's scope
                entry_in_table = d_symbol_table_search(
                                               lexical_analyzer->symbol_table,
                                               lexeme);
            }

            // Anyways, if the component deserves an entry in the symbol
            // table, its attribute will always be a reference to that entry
            lexical_component->attributes = entry_in_table;

            lexical_component_id = entry_in_table->lexical_component;
        }

        // Updating the output lexical_component...
        // If a lexical component has been successfully recognized, the value
        // in component ID will be >0
        if(lexical_component_id > 0 ) {
            lexical_component->category = lexical_component_id;

            // Its attributes would already been set if required
        }

        
        /* 3. Update parsing stats */
        _d_lexical_analyzer_update_parsing_stats(lexical_analyzer,
                                                 current_character,
                                                 return_character);


        /* 4. Checking if the parsing process may continue */
        if(d_io_system_is_eof(lexical_analyzer->io_system)) {

            perror("ERROR:LEXICAL_ANALYZE::EOF has been reached");
            continue_parsing = 0;
        }
    }
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

    int parsing_stats_updated = 0;


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


    // If no more characters remain unparsed in the input file
    if(d_io_system_is_eof(lexical_analyzer->io_system)) {

        perror("ERROR:LEXICAL_ANALYZE::EOF has been reached");
        return -1;
    }


    // The lexical component's category is set to an invalid state for the
    // time being
    //
    // This value can be set to a valid state either by this function (which
    // recognizes 1-char components) or by a finite automata which gets called
    // by this function (these automatas recognize longer components)
    //
    // When this function is about to exit, the return value will be decided
    // by the following expression:
    //
    //   return lexical_analyzer->category < 0
    //
    // That is, the function will report a failure if it could not recognize a
    // new component, and no automata was able either
    lexical_component->category = -1;
    lexical_component->attributes = NULL;


    /* 1. Retrieve the next character */
    d_io_system_get_next_char(lexical_analyzer->io_system, &character);

    #ifdef D_DEBUG
    printf("[lexical_analyzer][get next component] Char %zu:%zu: %c\n",
           lexical_analyzer->current_line,
           lexical_analyzer->current_character, character);
    #endif


    /* 2. Parse the character using the global finite automata */

    // See docs/0_lexical_analysis/all_finite_automatas.pdf for visual
    // representationa of this section

    // Depending on the input character:
    //
    //   - A new lexical component could be inmediately recognized by this
    //     function if it had just 1 char.
    //
    //     Every 1 char component can be identified just by its category's
    //     value. Therefore, no attributes will be appended.
    //
    //   - A certain finite automata will be called in order to recognize a
    //     longer component.
    //
    //     Some of these components can also be identified by their
    //     categories; i.e. the "==" equals relational operator. However, most
    //     of the components will be identifiers, keywords and numbers. As
    //     these cannot be properly identified just by using their categories,
    //     the corresponding automatas will take care of appending any
    //     required attributes to them, just as explained in the header of
    //     lexical.h

    switch (character) {

            /** 1-char components **/

            case '-':
                lexical_component->category = D_LC_OP_ARITHMETIC_MINUS;
                break;

            case '*':
                lexical_component->category = D_LC_OP_ARITHMETIC_TIMES;
                break;

            case '<':
                lexical_component->category = D_LC_OP_RELATIONAL_LESS_THAN;
                break;

            case '[':
                lexical_component->category = D_LC_OP_ACCESS_L_BRACKET;
                break;

            case ']':
                lexical_component->category = D_LC_OP_ACCESS_R_BRACKET;
                break;

            case ',':
                lexical_component->category = D_LC_SEPARATOR_COMMA;
                break;

            case ';':
                lexical_component->category = D_LC_SEPARATOR_SEMICOL;                
                break;

            case '(':
                lexical_component->category = D_LC_SEPARATOR_L_PARENTHESIS;
                break;

            case ')':
                lexical_component->category = D_LC_SEPARATOR_R_PARENTHESIS;
                break;

            case '{':
                lexical_component->category = D_LC_SEPARATOR_L_CURLY;
                break;

            case '}':
                lexical_component->category = D_LC_SEPARATOR_R_CURLY;
                break;


            /** Multiple-char components */
            
            case '"':
                /* 3. Update parsing stats prematurely as the control is being
                      given up to an automata */
                _d_lexical_analyzer_update_parsing_stats(lexical_analyzer,
                                                         character, 0);
                parsing_stats_updated = 1;
                
                _d_lexical_analyzer_run_automata(lexical_analyzer,
                                                 lexical_component,
                                                 _d_lexical_analyzer_automata_double_quoted_string,
                                                 0);
                break;
                
            case '=':
                /* 3. Update parsing stats prematurely as the control is being
                      given up to an automata */
                _d_lexical_analyzer_update_parsing_stats(lexical_analyzer,
                                                         character, 0);
                parsing_stats_updated = 1;
                
                _d_lexical_analyzer_run_automata(lexical_analyzer,
                                                 lexical_component,
                                                 _d_lexical_analyzer_automata_equals_and_assign,
                                                 0);
                break;

            case '+':
                /* 3. Update parsing stats prematurely as the control is being
                      given up to an automata */
                _d_lexical_analyzer_update_parsing_stats(lexical_analyzer,
                                                         character, 0);
                parsing_stats_updated = 1;
                
                _d_lexical_analyzer_run_automata(lexical_analyzer,
                                                 lexical_component,
                                                 _d_lexical_analyzer_automata_increment_and_plus_assign,
                                                 0);
                break;

            case '/':
                /* 3. Update parsing stats prematurely as the control is being
                      given up to an automata */
                _d_lexical_analyzer_update_parsing_stats(lexical_analyzer,
                                                         character, 0);
                parsing_stats_updated = 1;
                
                _d_lexical_analyzer_run_automata(lexical_analyzer,
                                                 lexical_component,
                                                 _d_lexical_analyzer_automata_comment_and_div,
                                                 0);
                break;

            default:
                // (some of cases cannot be represented through a switch-case
                // construct)

                if(isalpha(character) || character == '_') {

                    /* 3. Update parsing stats prematurely as the control is
                          being given up to an automata */
                    _d_lexical_analyzer_update_parsing_stats(lexical_analyzer,
                                                             character, 0);
                    parsing_stats_updated = 1;
                
                    _d_lexical_analyzer_run_automata(lexical_analyzer,
                                                     lexical_component,
                                                     _d_lexical_analyzer_automata_id_and_kwd,
                                                     0);
                }

                else if(character == '0') {

                    /* 3. Update parsing stats prematurely as the control is
                          being given up to an automata */
                    _d_lexical_analyzer_update_parsing_stats(lexical_analyzer,
                                                             character, 0);
                    parsing_stats_updated = 1;
                
                    _d_lexical_analyzer_run_automata(lexical_analyzer,
                                                     lexical_component,
                                                     _d_lexical_analyzer_automata_number_and_dot,
                                                     10);
                }

                else if(character == '.') {

                    /* 3. Update parsing stats prematurely as the control is
                          being given up to an automata */
                    _d_lexical_analyzer_update_parsing_stats(lexical_analyzer,
                                                             character, 0);
                    parsing_stats_updated = 1;
                
                    _d_lexical_analyzer_run_automata(lexical_analyzer,
                                                     lexical_component,
                                                     _d_lexical_analyzer_automata_number_and_dot,
                                                     0);
                }

                else if(isdigit(character)) { // The remaning digits are [1,9]

                    /* 3. Update parsing stats prematurely as the control is
                          being given up to an automata */
                    _d_lexical_analyzer_update_parsing_stats(lexical_analyzer,
                                                             character, 0);
                    parsing_stats_updated = 1;
                
                    _d_lexical_analyzer_run_automata(lexical_analyzer,
                                                     lexical_component,
                                                     _d_lexical_analyzer_automata_number_and_dot,
                                                     20);
                }

                else if(isblank(character) | isspace(character)) {

                    #ifdef D_DEBUG
                    printf("[lexical_analyzer][get next component] Entering "
                           "in whitespace automata\n");
                    #endif

                    /* 3. Update parsing stats prematurely as the control is
                          being given up to an automata */
                    _d_lexical_analyzer_update_parsing_stats(lexical_analyzer,
                                                             character, 0);
                    parsing_stats_updated = 1;

                    _d_lexical_analyzer_run_automata(lexical_analyzer,
                                                     lexical_component,
                                                     _d_lexical_analyzer_automata_whitespace,
                                                     0);
                }

                else {
                    // If the parsing reaches this point, the current lexical
                    // component must be invalid or not supported yet
                    // Therefore, it will be nice to raise an error message
                    // warning about it
                    perror("ERROR::LEXICAL_ANALYZER::Wrong lex. comp.");
                    // TODO remove
                    // printf("Is %d,%c\n", character, character);
                    // The lexical component's category is already set to
                    // '-1', which is an invalid value

                    // NOTE: there will be no need to raise an error message
                    //       beyond this point in this function, as if any
                    //       automata were to fail, it is its duty to take
                    //       care of showing the error all by itself
                }                
        }

    // They will have already been updated if the control was given up to any
    // automata
    if(!parsing_stats_updated) {
        _d_lexical_analyzer_update_parsing_stats(lexical_analyzer, character,
                                                 0);
    }

    // No matter if:
    //
    //   - 1. The component has been instantly recognized in this function.
    //   - 2. The component has been recognized in any automata.
    //   - 3. Any automata has failed recognizing the automata.
    //   - 4. The component did not fit any condition and just failed in
    //        this function's huge switch-case.
    //
    // The I/O system must always move its "begin lexeme" pointer to were
    // the "forward" pointer currently is. That is, the lexical analyzer has
    // already decided which characters have been parsed and which not by
    // requesting and/or returning characters. Thus, the I/O system's internal
    // state must be left properly accordingly so that the lexical analysis
    // may proceed/ without additional inconveniences.
    d_io_system_current_lexeme_recognized(lexical_analyzer->io_system);

    // Particular situation:
    //
    // If the recognized lexical component turned out to be whitespace, the
    // analyzer will try to get the next component that will actually be
    // meaninful during the compilation process
    if(lexical_component->category / D_LC_DISTANCE_CATEGORY ==
       D_LC_WHITESPACE / D_LC_DISTANCE_CATEGORY) {

        #ifdef D_DEBUG
        printf("[lexical_analyzer][get next component] Last comp. was "
               "whitespace, trying again...\n");
        #endif

        return d_lexical_analyzer_get_next_lexical_comp(lexical_analyzer,
                                                        lexical_component);
    }

    else {
        return lexical_component->category < 0;
    }
}


/**
 * @brief Implementation of lexical.h/d_lexical_analyzer_destroy_lexical_com
 *
 * @details
 *  Specifically, according to the different types of lexical components that
 *  have attributes:
 *    - Those that have an entry in the symbol table are already memory
 *      managed by it.
 *    - Regarding the other components that also have attributes, but no
 *      entry, these attributes have been reserved on purpose when recognizing
 *      the lex. comp. from the input file, so they will not be freed unless
 *      the consumer requests it.
 */
int d_lexical_analyzer_destroy_lexical_com(
    struct d_lexical_analyzer *lexical_analyzer,
    struct d_lexical_component *lexical_component
)
{
    if(lexical_analyzer == NULL) {

        perror("ERROR::LEXICAL_ANALYZER::Reference to struct "
               "d_lexical_analyzer is NULL");
        return -1;
    }

    if(lexical_component == NULL) {

        perror("ERROR::LEXICAL_ANALYZER::Reference to struct "
               "d_lexical_component is NULL");
        return -1;
    }


    if(lexical_component->attributes != NULL) {

        // If its attribute is its lexeme...
        //   no entry in symbol table -> must be freed by the lex. analyzer
        if(lexical_component->category / D_LC_DISTANCE_CATEGORY ==
           D_LC_LITERAL / D_LC_DISTANCE_CATEGORY) {

            free((void *) lexical_component->attributes);
        }

        // If its attribute is a pointer to its entry in the symbol table...
        //   entry in the symbol table -> the table will take care of it
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

// ═══════════════════════════════════════════════════════════════════════════
// ═════════════════════ END OF SECTION: Lexical Analyzer ════════════════════
// ═══════════════════════════════════════════════════════════════════════════
