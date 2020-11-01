/**
 * @file errors.c
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief Implementation of common/lexical_components.h
 */


#include "common/lexical_components.h"
#include "common/errors.h"


/**
 * @brief Implementation of lexical_components.h/d_lc_to_string
 */
const char *d_lc_to_string(
    int lexical_component
)
{
    switch (lexical_component) {
    
        /* Category: keyword */
        case D_LC_KEYWORD:
            return "LC_KEYWORD";


        /* Category: identifier */
        case D_LC_IDENTIFIER:
            return "LC_IDENTIFIER";


        /* Category: literal */
        case D_LC_LITERAL:
            return "LC_LITERAL";

        // Subcategory: floating point literal
        case D_LC_LITERAL_FP:
            return "LC_LITERAL_FP";

        // Subcategory: integer point literal
        case D_LC_LITERAL_INT:
            return "LC_LITERAL_INT";

        // Subcategory: string point literal
        case D_LC_LITERAL_STR:
            return "LC_LITERAL_STR";


        /* Category: operator */
        case D_LC_OP:
            return "LC_OP";

        // Subcategory: arithmetic operator
        case D_LC_OP_ARITHMETIC:
            return "LC_OP_ARITHMETIC";
        case D_LC_OP_ARITHMETIC_PLUS:
            return "LC_OP_ARITHMETIC_PLUS";
        case D_LC_OP_ARITHMETIC_MINUS:
            return "LC_OP_ARITHMETIC_MINUS";
        case D_LC_OP_ARITHMETIC_TIMES:
            return "LC_OP_ARITHMETIC_TIMES";
        case D_LC_OP_ARITHMETIC_DIV:
            return "LC_OP_ARITHMETIC_DIV";
        case D_LC_OP_ARITHMETIC_INCREMENT:
            return "LC_OP_ARITHMETIC_INCREMENT";
        case D_LC_OP_ARITHMETIC_DECREMENT:
            return "LC_OP_ARITHMETIC_DECREMENT";

        // Subcategory: relational operator
        case D_LC_OP_RELATIONAL:
            return "LC_OP_RELATIONAL";
        case D_LC_OP_RELATIONAL_LESS_THAN:
            return "LC_OP_RELATIONAL_LESS_THAN";
        case D_LC_OP_RELATIONAL_GREATER_THAN:
            return "LC_OP_RELATIONAL_GREATER_THAN";
        case D_LC_OP_RELATIONAL_EQUALS:
            return "LC_OP_RELATIONAL_EQUALS";
        case D_LC_OP_RELATIONAL_NOT_EQUALS:
            return "LC_OP_RELATIONAL_NOT_EQUALS";

        // Subcategory: assignment operator
        case D_LC_OP_ASSIGNMENT:
            return "LC_OP_ASSIGNMENT";
        case D_LC_OP_ASSIGNMENT_ASSIGN:
            return "LC_OP_ASSIGNMENT_ASSIGN";
        case D_LC_OP_ASSIGNMENT_PLUS_ASSIGN:
            return "LC_OP_ASSIGNMENT_PLUS_ASSIGN";
        case D_LC_OP_ASSIGNMENT_MINUS_ASSIGN:
            return "LC_OP_ASSIGNMENT_MINUS_ASSIGN";
        case D_LC_OP_ASSIGNMENT_TIMES_ASSIGN:
            return "LC_OP_ASSIGNMENT_TIMES_ASSIGN";
        case D_LC_OP_ASSIGNMENT_DIV_ASSIGN:
            return "LC_OP_ASSIGNMENT_DIV_ASSIGN";

        // Subcategory: access operator
        case D_LC_OP_ACCESS:
            return "LC_OP_ACCESS";
        case D_LC_OP_ACCESS_DOT:
            return "LC_OP_ACCESS_DOT";
        case D_LC_OP_ACCESS_L_BRACKET:
            return "LC_OP_ACCESS_L_BRACKET";
        case D_LC_OP_ACCESS_R_BRACKET:
            return "LC_OP_ACCESS_R_BRACKET";


        /* Category: separator */
        case D_LC_SEPARATOR:
            return "LC_SEPARATOR";
        case D_LC_SEPARATOR_COMMA:
            return "LC_SEPARATOR_COMMA";
        case D_LC_SEPARATOR_SEMICOL:
            return "LC_SEPARATOR_SEMICOL";
        case D_LC_SEPARATOR_L_PARENTHESIS:
            return "LC_SEPARATOR_L_PARENTHESIS";
        case D_LC_SEPARATOR_R_PARENTHESIS:
            return "LC_SEPARATOR_R_PARENTHESIS";
        case D_LC_SEPARATOR_L_CURLY:
            return "LC_SEPARATOR_L_CURLY";
        case D_LC_SEPARATOR_R_CURLY:
            return "LC_SEPARATOR_R_CURLY";


        /* Category: whitespace */
        case D_LC_WHITESPACE:
            return "LC_WHITESPACE";
        case D_LC_WHITESPACE_COMMENT:
            return "LC_WHITESPACE_COMMENT";
        case D_LC_WHITESPACE_EOL:
            return "LC_WHITESPACE_EOL";


        default:
            d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_INVALID,
                                   "lexical_components.c", "d_lc_to_string",
                                   "'lexical_component' ID not registered");
            return "LC_NOT_SUPPORTED";
    }
}
