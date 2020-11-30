/**
 * @file dec_numbers.h
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief This file contains the utilities on which delta relies to represent
 *        base 10 numbers.
 *
 * @details
 *  This file contains the utilities on which delta relies to represent
 *  base 10 numbers. These may be both integer and floating point.
 *
 *  It is true that all base 10 numbers could be represent using just a single
 *  floating data type. However, delta is intented to be a calculator, and in
 *  order to do it properly, it should avoid any C precision related errors
 *  while only integer arithmetic is required by the user.
 */
 

#ifndef D_DEC_NUMBERS
#define D_DEC_NUMBERS


// uint8_t & int64_t data types
#include <stdint.h>


/**
 * @brief Groups all possible data types of a base 10 number.
 * 
 * @details
 *  Groups all possible data types of a base 10 number as a single element,
 *   so that the "d_dec_number" structure may carry both types of values at
 *   the same time.
 */
typedef union {
    int64_t integer;
    double floating;
} d_dec_number_values;


/**
 * @brief Represents a base 10 number.
 *
 * @details
 *  Contains all the attributes that are needed to represent a base 10 number.
 */
struct d_dec_number {
    /** If the represented number is a floating point or an integer one. */
    uint8_t is_floating;

    /** The actual value of the number. */
    d_dec_number_values values;
};


/**
 * @brief Returns the negative value of the given base 10 number.
 * 
 * @param[in] dec_number The base 10 number.
 *
 * @return Its negative value, 0-initialized structure if failed (only when
 *         dec_number is NULL).
 */
struct d_dec_number d_dec_numbers_get_negated_value(
    struct d_dec_number *dec_number
);


/**
 * @brief Returns the floating point value that corresponds the given base 10
 *        number.
 *
 * @details
 *  Returns the floating point value that corresponds the given base 10
 *  number. If the number is an integer, its value is converted.
 * 
 * @param[in] dec_number The base 10 number.
 *
 * @return Its corresponding floating point value, or -1.0 if failed (only
 *         when dec_number is NULL).
 */
double d_dec_numbers_get_floating_value(
    struct d_dec_number *dec_number
);


/**
 * @brief Checks if any of the two given base 10 numbers is of the floating
 *        point type.
 *
 * @param[in] dec_number_1 The first base 10 number.
 * @param[in] dec_number_2 The second base 10 number.
 *
 * @return 0 if false, any positive value if true, -1 if failed (only when
 *         dec_number_1 or dec_number_2 is NULL).
 */
int d_dec_numbers_any_floating_value(
    struct d_dec_number *dec_number_1,
    struct d_dec_number *dec_number_2
);


/**
 * @brief Performs the requested mathematical operation over the two given
 *        numbers.
 *
 * @details
 *  Performs the requested performs the requested mathematical operation over
 *  the two given numbers. The supported operations are:
 *
 *    - Addition.
 *    - Substraction.
 *    - Multiplication.
 *    - Division.
 *    - Exponentiation.
 *
 *  The data type of the resulting base 10 number depends on the type of both
 *  input numbers. If any of them is a floating point number, the returned
 *  number will also be one. Otherwise, it will be an integer number.
 * 
 * @param[in] operation The symbol that represents the mathematical operation
 *                      (+, -, *, /, ^).
 * @param[in] dec_value_1 The first base 10 number.
 * @param[in] dec_value_2 The second base 10 number.
 *
 * @return The resulting base 10 number after applying the mathematical
 *         operation, 0-initialized structure if failed.
 */
struct d_dec_number d_dec_numbers_compute_operation(
    char operation,
    struct d_dec_number *dec_number_1,
    struct d_dec_number *dec_number_2
);


#endif //D_DEC_NUMBERS
