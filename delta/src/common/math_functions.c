/**
 * @file math_functions.c
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief Implementation of common/math_functions.h
 */


#include "common/math_functions.h"

// The preloaded math functions are directly taken from this library
#include <math.h>


/** Which names the user may specify to call the preloaded math functions. */
const char *D_MATH_FUNCTIONS_NAMES[] = {
    "atan",
    "cos",
    "exp",
    "log",
    "sin",
    "sqrt",
};

/** Pointers to the implementations of the preloaded math functions. */
const d_dec_function D_MATH_FUNCTIONS_IMPLEMENTATIONS[] = {
    &atan,
    &cos,
    &exp,
    &log,
    &sin,
    &sqrt,
};
