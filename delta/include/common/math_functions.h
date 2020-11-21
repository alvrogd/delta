/**
 * @file math_functions.h
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

#ifndef D_MATH_FUNCTIONS
#define D_MATH_FUNCTIONS


/** Template for the functions that operate on decimal numbers. */
typedef double (*dec_function) (double);

extern const char *D_MATH_FUNCTIONS_NAMES[6];

extern const dec_function D_MATH_FUNCTIONS_IMPLEMENTATIONS[6];


#endif //D_MATH_FUNCTIONS
