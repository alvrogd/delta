/**
 * @file math_functions.h
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief This file contains the utilities through which delta supports
 *        mathematical functions.
 *
 * @details
 *  This file contains the utilities through which delta supports mathematical
 *  functions.
 *
 *  Currently, the only supported math function is that which operates on
 *  decimal numbers. It must take exactly 1 "double" argument, and return a
 *  "double" value.
 *
 *  Some functions from the "math.h" C library are preloaded when launching
 *  delta. More specifically:
 *
 *    - atan().
 *    - cos().
 *    - exp().
 *    - log().
 *    - sin().
 *    - sqrt().
 *
 *  Furthermore, the user will be able to load more math functions through
 *  built-in commands, as long as they follow the previously explained
 *  interface.
 */

#ifndef D_MATH_FUNCTIONS
#define D_MATH_FUNCTIONS


/** Template for the math functions that operate on decimal numbers. */
typedef double (*dec_function) (double);


/** Which names the user may specify to call the preloaded math functions. */
extern const char *D_MATH_FUNCTIONS_NAMES[6];

/** Pointers to the implementations of the preloaded math functions. */
extern const dec_function D_MATH_FUNCTIONS_IMPLEMENTATIONS[6];


#endif //D_MATH_FUNCTIONS
