/**
 * @file math_constants.c
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief Implementation of math/math_constants.h
 */


#include "math/math_constants.h"


/** Which names the user may specify to reference the math constants. */
const char *D_MATH_CONSTANTS_NAMES[] = {
    "e",
    "pi",
};

/** Pointers to the implementations of the 1 arg built-in commands. */
const double D_MATH_CONSTANTS_VALUES[] = {
    2.7182818284590452354,
    3.14159265358979323846,
};
