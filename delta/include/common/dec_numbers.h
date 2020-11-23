/**
 * @file dec_numbers.h
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief This file lists a subset of the keywords in the D language.
 *
 * @details
 *  This file lists all the keywords in the D language that can be found in
 *  regression.d  
 */

/* TODO update */

#ifndef D_DEC_NUMBERS
#define D_DEC_NUMBERS


// More integer data types
#include <stdint.h>


/**
 * @brief Groups all possible data types of a decimal number.
 * 
 * @details
 *  Groups all possible data types of a decimal number as a single element,
 *   so that the "d_commmand" structure may carry both types of pointers at
 *   the same time.
 */
typedef union {
    int64_t integer;
    double floating;
} d_dec_number_values;


/**
 * @brief Represents the attributes of a delta decimal number.
 *
 * @details
 *  Contains any attributes of a delta decimal number that the symbol table
 *  needs to hold in order to invoke it.
 */
struct d_dec_number {
    /** Pointer to which the command's implementation resides. */
    d_dec_number_values values;

    /** If the variable holds a floating point value, or an integer one. */
    uint8_t is_floating;
};


double d_dec_number_get_negated_value(
    struct d_dec_number *dec_number
);

double d_dec_number_get_floating_value(
    struct d_dec_number *dec_number
);

int d_dec_number_any_floating_value(
    struct d_dec_number *dec_number_1,
    struct d_dec_number *dec_number_2
);

int64_t d_dec_number_integer_op(
    char operation,
    int64_t dec_value_1,
    int64_t dec_value_2
);

double d_dec_number_floating_op(
    char operation,
    double dec_value_1,
    double dec_value_2
);


#endif //D_DEC_NUMBERS
