/**
 * @file errors.h
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief This file contains the utilities on which delta relies to show its
 *        error messages.
 *
 * @details
 *  All possible error messages that delta may show fall into two categories:
 *
 *    - On the one hand, there are the error messages that happen due to
 *      parsing input setences. They are lexical, syntatic and semantic
 *      errors.
 *
 *    - On the other hand, delta can also raise internal errors, such as those
 *      that come from failed system calls.
 *
 *  No matter from where an error comes from, there will be a template for it.
 *  Moreover, if a template allows one or more custom values to be inserted
 *  (i.e. to point out to which variable the error affects), delta may also
 *  provide those values, and they will be printed out just in the right
 *  places. Custom values are represented by "[..]" segments in the templates.
 */


#ifndef D_ERRORS
#define D_ERRORS

// For functions with a variable amout of arguments
#include <stdarg.h>


#define D_ERR_DISTANCE_CATEGORY 1000
#define D_ERR_DISTANCE_SUBCATEGORY 100


/* Category: internal errors */
#define D_ERR_INTERN 1000

// Subcategory: argument errors
#define D_ERR_INTERN_ARGUMENT 1100
#define D_ERR_INTERN_ARGUMENT_INVALID 1101
#define D_ERR_INTERN_ARGUMENT_NULL 1102

// Subcategory: syscall errors
#define D_ERR_INTERN_SYSCALL 1200
#define D_ERR_INTERN_SYSCALL_FAILED 1201

// Subcategory: inner workings' logic errors
#define D_ERR_INTERN_LOGIC 1300


/* Category: lexical analysis errors */
#define D_ERR_LEX 2000
#define D_ERR_LEX_CHARACTER_NOT_SUPPORTED 2001

// Subcategory: literals errors
#define D_ERR_LEX_LITERALS 2300
#define D_ERR_LEX_LITERALS_FLOATS 2310
#define D_ERR_LEX_LITERALS_FLOATS_BAD 2311
#define D_ERR_LEX_INTEGERS 2320
#define D_ERR_LEX_LITERALS_INTEGERS_BAD 2321
#define D_ERR_LEX_LITERALS_INTEGERS_BINARY_BAD 2322
#define D_ERR_LEX_LITERALS_STRINGS 2330
#define D_ERR_LEX_LITERALS_STRING_EMPTY 2331


/* Category: lexical analysis errors */
#define D_ERR_SYN 3000
#define D_ERR_SYN_WRITE_CONSTANT 3001
#define D_ERR_SYN_UNMATCHED_PARENTHESIS 3002


/* Category: semantic analysis errors */
#define D_ERR_SEM 4000
#define D_ERR_SEM_DIVISION_BY_ZERO 4001
#define D_ERR_SEM_INCORRECT_ARG_COUNT 4002
#define D_ERR_SEM_INCORRECT_ARG_TYPE 4003


/* Category: other user errors */
#define D_ERR_USER 5000

// Subcategory: I/O errors
#define D_ERR_USER_IO 5100
#define D_ERR_USER_IO_FILE_INACCESSIBLE 5101
#define D_ERR_USER_IO_FUNCTION_INACCESSIBLE 5102
#define D_ERR_USER_IO_NO_DYN_LIBRARY_SELECTED 5103
#define D_ERR_USER_IO_FUNCTION_NAME_TAKEN 5104


/**
 * @brief Returns the template that has been defined for the specified error
 *        code.
 * 
 * @param error_code The error code.
 *
 * @return The string that represents the template.
 */
const char *d_errors_get_template(
    int error_code
);


/**
 * @brief Outputs a parsing error message.
 *
 * @details
 *  A given parsing error message is shown through the standard output.
 *
 *  Its arguments are:
 *
 *    - [mandatory] The error code which uniquely identifies the error type.
 *
 *    - [mandatory] From which line in the source file the error comes from.
 *
 *    - [mandatory] From which column in the source file the error comes from.

 *    - [optional] If the template allows one or more custom values to be
 *                 inserted (i.e. to point out to which variable the error
 *                 affects), you may also provide those values, and they will
 *                 be printed out just in the right places, accordingly to the
 *                 error template.
 *
 *  However, it is worth noting that this function's prototype does not match
 *  the previous explanation, as its follows the one required by the
 *  "stdarg.h" library which will allow recognizing a variable amount of
 *  custom values.
 *
 * @param[in] arg_count How many arguments are in total.
 * @param[in] ... The arguments themselves.
 */
void d_errors_parse_show(
    int arg_count,
    ...
);


/**
 * @brief Outputs an internal error message.
 *
 * @details
 *  A given internal error message is shown through the standard output.
 *
 *  Its arguments are:
 *
 *    - [mandatory] The error code which identifies the template in which the
 *                  error fits.
 *
 *    - [mandatory] From which source file the error comes from, for debugging
 *                  purposes.
 *
 *    - [mandatory] From which function the error comes from, for debugging
 *                  purposes.
 *
 *    - [optional] If the template allows one or more custom values to be
 *                 inserted (i.e. to point out which variable turned out to be
 *                 NULL but was not expected to), you may also provide those
 *                 values, and they will be printed out just in the right
 *                 places, accordingly to the error template.
 *
 *  However, it is worth noting that this function's prototype does not match
 *  the previous explanation, as its follows the one required by the
 *  "stdarg.h" library which will allow recognizing a variable amount of
 *  custom values.
 *
 * @param[in] arg_count How many arguments are in total.
 * @param[in] ... The arguments themselves.
 */
void d_errors_internal_show(
    int arg_count,
    ...
);


#endif //D_ERRORS
