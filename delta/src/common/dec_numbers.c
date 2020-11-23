/**
 * @file dec_numbers.c
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief Implementation of common/dec_numbers.h
 */


#include "common/dec_numbers.h"

#include <math.h>


double d_dec_number_get_negated_value(
    struct d_dec_number *dec_number
)
{
    if(dec_number->is_floating) {
        return -(dec_number->values.floating);
    }
    else {
        return (double) -(dec_number->values.integer);
    }
}

double d_dec_number_get_floating_value(
    struct d_dec_number *dec_number
)
{
    if(dec_number->is_floating) {
        return dec_number->values.floating;
    }
    else {
        return (double) dec_number->values.integer;
    }
}


int d_dec_number_any_floating_value(
    struct d_dec_number *dec_number_1,
    struct d_dec_number *dec_number_2
)
{
    return dec_number_1->is_floating || dec_number_2->is_floating;
}

int64_t d_dec_number_integer_op(
    char operation,
    int64_t dec_value_1,
    int64_t dec_value_2
)
{
    switch(operation) {
        case '+':
            return dec_value_1 + dec_value_2;

        case '-':
            return dec_value_1 - dec_value_2;

        case '*':
            return dec_value_1 * dec_value_2;

        case '/':
            return dec_value_1 / dec_value_2;

        case '^':
            return (int64_t) pow(dec_value_1, dec_value_2);

        default:
            /* TODO warn error */
            return 0;
    }
}

double d_dec_number_floating_op(
    char operation,
    double dec_value_1,
    double dec_value_2
)
{
    switch(operation) {
        case '+':
            return dec_value_1 + dec_value_2;

        case '-':
            return dec_value_1 - dec_value_2;

        case '*':
            return dec_value_1 * dec_value_2;

        case '/':
            return dec_value_1 / dec_value_2;

        case '^':
            return pow(dec_value_1, dec_value_2);

        default:
            /* TODO warn error */
            return 0;
    }
}
