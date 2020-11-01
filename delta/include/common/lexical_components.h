/**
 * @file lexical_components.h
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief This file lists a subset of the lexical components in the D
          language.
 *
 * @details
 *  This file lists all the lexical components in the D language that can be
 *  found in regression.d  
 *
 *  Each lexical component will be represented by a natural number [0, +inf).
 *  Furthermore, as they will be grouped into categories (i.e integer
 *  literals, floating literals...), each category will be a thousand apart
 *  from the sorrounding ones, so that:
 *
 *    - When working with a certain lexical component, it can be abstracted
 *      into its corresponding category by diving its value by 1000.
 *
 *    - There is enough room in each category to add a bunch of new lexical
 *      components (up to 1000 in total), without altering the previous
 *      feature.
 *
 *  This will also be reproduced for each category's subcategories, distancing
 *  them by hundreds.
 */


#ifndef D_LEXICAL_COMPONENTS
#define D_LEXICAL_COMPONENTS


#define D_LC_DISTANCE_CATEGORY 1000
#define D_LC_DISTANCE_SUBCATEGORY 100


/* Category: keyword */
#define D_LC_KEYWORD 1000


/* Category: identifier */
#define D_LC_IDENTIFIER 2000


/* Category: literal */
#define D_LC_LITERAL 3000

// Subcategory: floating point literal
#define D_LC_LITERAL_FP 3100

// Subcategory: integer point literal
#define D_LC_LITERAL_INT 3200

// Subcategory: string point literal
#define D_LC_LITERAL_STR 3300


/* Category: operator */
#define D_LC_OP 4000

// Subcategory: arithmetic operator
#define D_LC_OP_ARITHMETIC 4100
#define D_LC_OP_ARITHMETIC_PLUS 4101
#define D_LC_OP_ARITHMETIC_MINUS 4102
#define D_LC_OP_ARITHMETIC_TIMES 4103
#define D_LC_OP_ARITHMETIC_DIV 4104
#define D_LC_OP_ARITHMETIC_INCREMENT 4105
#define D_LC_OP_ARITHMETIC_DECREMENT 4106

// Subcategory: relational operator
#define D_LC_OP_RELATIONAL 4200
#define D_LC_OP_RELATIONAL_LESS_THAN 4201
#define D_LC_OP_RELATIONAL_GREATER_THAN 4202
#define D_LC_OP_RELATIONAL_EQUALS 4203
#define D_LC_OP_RELATIONAL_NOT_EQUALS 4204

// Subcategory: assignment operator
#define D_LC_OP_ASSIGNMENT 4300
#define D_LC_OP_ASSIGNMENT_ASSIGN 4301
#define D_LC_OP_ASSIGNMENT_PLUS_ASSIGN 4302
#define D_LC_OP_ASSIGNMENT_MINUS_ASSIGN 4303
#define D_LC_OP_ASSIGNMENT_TIMES_ASSIGN 4304
#define D_LC_OP_ASSIGNMENT_DIV_ASSIGN 4305

// Subcategory: access operator
#define D_LC_OP_ACCESS 4400
#define D_LC_OP_ACCESS_DOT 4401
#define D_LC_OP_ACCESS_L_BRACKET 4402
#define D_LC_OP_ACCESS_R_BRACKET 4403


/* Category: separator */
#define D_LC_SEPARATOR 5000
#define D_LC_SEPARATOR_COMMA 5001
#define D_LC_SEPARATOR_SEMICOL 5002
#define D_LC_SEPARATOR_L_PARENTHESIS 5003
#define D_LC_SEPARATOR_R_PARENTHESIS 5004
#define D_LC_SEPARATOR_L_CURLY 5005
#define D_LC_SEPARATOR_R_CURLY 5006


/* Category: whitespace */
#define D_LC_WHITESPACE 6000
#define D_LC_WHITESPACE_COMMENT 6001
#define D_LC_WHITESPACE_EOL 6002


/**
 * @brief Returns the string that represents the specified lexical component
 *        (sub)category.
 * 
 * @param lexical_component The category.
 * @return const char* The string that represents the category.
 */
const char *d_lc_to_string(
    int lexical_component
);


#endif //D_LEXICAL_COMPONENTS
