/**
 * @file dec_numbers.c
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief Implementation of common/dec_numbers.h
 */


#include "common/dec_numbers.h"

#include "common/errors.h"


// pow
#include <math.h>
// NULL
#include <stddef.h>
// memset
#include <string.h>


/**
 * @brief Implementation of dec_numbers.h/d_dec_number_get_negated_value 
 */
struct d_dec_number d_dec_numbers_get_negated_value(
    struct d_dec_number *dec_number
)
{
    struct d_dec_number negated_value;


    memset(&negated_value, 0, sizeof(struct d_dec_number));


    if(dec_number == NULL) {
        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "dec_numbers.c",
                               "d_dec_numbers_get_negated_value",
                               "'dec_number'");
        return negated_value;
    }


    if(dec_number->is_floating) {
        negated_value.is_floating = 1;
        negated_value.values.floating = -(dec_number->values.floating);
    }

    else {
        negated_value.is_floating = 0;
        negated_value.values.integer = -(dec_number->values.integer);
    }


    return negated_value;
}


/**
 * @brief Implementation of dec_numbers.h/d_dec_numbers_get_floating_value 
 */
double d_dec_numbers_get_floating_value(
    struct d_dec_number *dec_number
)
{
    if(dec_number == NULL) {
        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "dec_numbers.c",
                               "d_dec_numbers_get_floating_value",
                               "'dec_number'");
        return -1.0;
    }


    if(dec_number->is_floating) {
        return dec_number->values.floating;
    }

    else {
        return (double) dec_number->values.integer;
    }
}


/**
 * @brief Implementation of dec_numbers.h/d_dec_numbers_any_floating_value 
 */
int d_dec_numbers_any_floating_value(
    struct d_dec_number *dec_number_1,
    struct d_dec_number *dec_number_2
)
{
    if(dec_number_1 == NULL) {
        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "dec_numbers.c",
                               "d_dec_numbers_any_floating_value",
                               "'dec_number_1'");
        return -1;
    }

    if(dec_number_2 == NULL) {
        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "dec_numbers.c",
                               "d_dec_numbers_any_floating_value",
                               "'dec_number_2'");
        return -1;
    }


    return (dec_number_1->is_floating || dec_number_2->is_floating) ? 1 : 0;
}


/**
 * @brief Implementation of dec_numbers.h/d_dec_numbers_compute_operation 
 */
struct d_dec_number d_dec_numbers_compute_operation(
    char operation,
    struct d_dec_number *dec_number_1,
    struct d_dec_number *dec_number_2
)
{
    struct d_dec_number result;

    double double_value_1 = 0.0;
    double double_value_2 = 0.0;


    memset(&result, 0, sizeof(struct d_dec_number));


    if(dec_number_1 == NULL) {
        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "dec_numbers.c",
                               "d_dec_numbers_compute_operation",
                               "'dec_number_1'");
        return result;
    }

    if(dec_number_2 == NULL) {
        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "dec_numbers.c",
                               "d_dec_numbers_compute_operation",
                               "'dec_number_2'");
        return result;
    }


    // Checks whether the resulting number will be an integer or a floating
    // point one
    if(d_dec_numbers_any_floating_value(dec_number_1, dec_number_2)) {

        result.is_floating = 1;

        double_value_1 = d_dec_numbers_get_floating_value(dec_number_1);
        double_value_2 = d_dec_numbers_get_floating_value(dec_number_2);


        switch(operation) {
            case '+':
                result.values.floating =  double_value_1 + double_value_2;
                break;

            case '-':
                result.values.floating =  double_value_1 - double_value_2;
                break;

            case '*':
                result.values.floating =  double_value_1 * double_value_2;
                break;

            case '/':
                result.values.floating =  double_value_1 / double_value_2;
                break;

            case '^':
                result.values.floating =  pow(double_value_1, double_value_2);
                break;

            default:
                d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_INVALID,
                                       "dec_numbers.c",
                                       "d_dec_numbers_compute_operation",
                                       "the specified operation is not "
                                       "supported");
                return result;
        }
    }


    // If none of the input numbers is a floating one, the resulting number
    // will be an integer
    else {

        result.is_floating = 0;
        
        switch(operation) {
            case '+':
                result.values.integer =  dec_number_1->values.integer +
                                         dec_number_2->values.integer;
                break;

            case '-':
                result.values.integer =  dec_number_1->values.integer -
                                         dec_number_2->values.integer;
                break;

            case '*':
                result.values.integer =  dec_number_1->values.integer *
                                         dec_number_2->values.integer;
                break;

            case '/':
                result.values.integer =  dec_number_1->values.integer /
                                         dec_number_2->values.integer;
                break;

            case '^':
                result.values.integer =  (int64_t) pow(
                                            dec_number_1->values.integer,
                                            dec_number_2->values.integer
                                         );
                break;

            default:
                d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_INVALID,
                                       "dec_numbers.c",
                                       "d_dec_numbers_compute_operation",
                                       "the specified operation is not "
                                       "supported");
                return result;
        }
    }


    return result;
}
