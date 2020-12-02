/**
 * @file math_functions.c
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief Implementation of math/math_functions.h
 */


#include "math/math_functions.h"

// The preloaded math functions are directly taken from this library
#include <math.h>


/** Which names the user may specify to call the preloaded math functions. */
const char *D_MATH_FUNCTIONS_NAMES[] = {
    
    // Generic functions
    "fabs",

    // Exponential functions
    "exp",
    "exp2",
    "log",
    "log2",
    "log10",

    // Power functions
    "cbrt",
    "sqrt",

    // Trigonometric functions
    "acos",
    "asin",
    "atan",
    "cos",
    "sin",
    "tan",

    // Hyperbolic functions
    "acosh",
    "asinh",
    "atanh",
    "cosh",
    "sinh",
    "tanh",
};

/** Pointers to the implementations of the preloaded math functions. */
const d_dec_function D_MATH_FUNCTIONS_IMPLEMENTATIONS[] = {
    
    // Generic functions
    &fabs,

    // Exponential functions
    &exp,
    &exp2,
    &log,
    &log2,
    &log10,

    // Power functions
    &cbrt,
    &sqrt,

    // Trigonometric functions
    &acos,
    &asin,
    &atan,
    &cos,
    &sin,
    &tan,

    // Hyperbolic functions
    &acosh,
    &asinh,
    &atanh,
    &cosh,
    &sinh,
    &tanh,
};
