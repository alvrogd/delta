/**
 * @file math_functions.c
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief Implementation of common/math_functions.h
 */


#include "common/math_functions.h"

#include <math.h>


const char *D_MATH_FUNCTIONS_NAMES[] = {
    "atan",
    "cos",
    "exp",
    "log",
    "sin",
    "sqrt",
};

const dec_function D_MATH_FUNCTIONS_IMPLEMENTATIONS[] = {
    &atan,
    &cos,
    &exp,
    &log,
    &sin,
    &sqrt,
};
