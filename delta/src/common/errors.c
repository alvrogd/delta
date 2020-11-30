/**
 * @file errors.c
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief Implementation of common/errors.h
 *
 * // TODO add color codes
 */


#include "common/errors.h"

#include <stdio.h>
#include <stddef.h>
#include <string.h>


/** Global buffer that should be plenty for any metadata. It avoids allocating
    new memory each time that a message must be shown. */
char char_buffer[128];


/**
 * @brief Implementation of errors.h/d_errors_get_template 
 */
const char *d_errors_get_template(
    int error_code
)
{
    switch (error_code) {
    
        /* Category: internal errors */

        case D_ERR_INTERN_ARGUMENT_INVALID:
            return "invalid argument, [..]";
        
        case D_ERR_INTERN_ARGUMENT_NULL:
            return "NULL argument, [..]";
        
        case D_ERR_INTERN_SYSCALL_FAILED:
            return "failed syscall, [..]";

        case D_ERR_INTERN_LOGIC:
            return "logical error, [..]";


        /* Category: lexical analysis errors */

        // Subcategory: literals errors
        case D_ERR_LEX_LITERALS_FLOATS_BAD:
            return "malformed floating number, [..]";

        case D_ERR_LEX_LITERALS_INTEGERS_BAD:
            return "malformed integer number, [..]";

        case D_ERR_LEX_LITERALS_INTEGERS_BINARY_BAD:
            return "malformed binary integer number, [..]";

        case D_ERR_LEX_LITERALS_INTEGERS_BASE10_BAD:
            return "malformed base 10 integer number, [..]";

        case D_ERR_LEX_LITERALS_STRING_EMPTY:
            return "a string cannot be empty";

        case D_ERR_LEX_LITERALS_STRING_UNSUPPORTED_ESCAPE:
            return "as of now, the only supported escape sequences are ";

        case D_ERR_LEX_LITERALS_STRING_EOF:
            return "unclosed string, reached EOF";

        // Subcategory: operators errors
        case D_ERR_LEX_OPERATORS_UNSUPPORTED:
            return "as of now, the only two supported operators starting by "
                   "+ are [..]";

        // Subcategory: whitespace errors
        case D_ERR_LEX_WHITESPACE_COMMENT_EOF:
            return "unclosed comment, reached EOF";


        /* Category: user errors */

        case D_ERR_USER_INPUT_DIVISION_BY_ZERO:
            return "catched a division by 0";

        case D_ERR_USER_INPUT_FILE_INACCESSIBLE:
            return "the input file is inaccessible; check if the path is "
                   "right, and if its permissions allow reading it";

        case D_ERR_USER_INPUT_WRITE_CONSTANT:
            return "mathematical constants are inmutable";

        case D_ERR_USER_INPUT_INCORRECT_ARG_COUNT:
            return "incorrect amount of arguments, expected [..] args";

        case D_ERR_USER_INPUT_INCORRECT_ARG_TYPE:
            return "incorrect argument type, expected a [..]";

        case D_ERR_USER_INPUT_NO_DYN_LIBRARY_SELECTED:
            return "no dynamic library has been selected; check if the last "
                   "specified library has been successfully opened";

        case D_ERR_USER_INPUT_FUNCTION_INACCESSIBLE:
            return "no function by the given name could be found; check if "
                   "the name is right, and if it should be in the currently "
                   "selected library";

        case D_ERR_USER_INPUT_FUNCTION_NAME_TAKEN:
            return "a mathematical function that goes by the same name is "
                   "already loaded";

        default:
            printf("error: invalid argument, 'error_code' not recognized\n"
                   " --> internal file : errors.c : "
                   "d_errors_get_template()\n");

            return "no template defined for the specified error code";
    }
}


/**
 * @brief Outputs the specified error message.
 * 
 * @param[in] error_code The error code which identifies the error type.
 * @param[in] metadata String that contains any additional info about the
 *                     error that must be printed before the message template.
 * @param[in] custom_values_count How many custom values have been specified.
 * @param[in] custom_values List which contains any given custom values that
 *                          are to be inserted into the message template.
 */
void _d_errors_show(
    int error_code,
    const char *metadata,
    int remaining_custom_values,
    va_list *custom_values
)
{
    const char *message_template = NULL;
    const char *message_template_end = NULL;
    size_t message_template_length = 0;

    const char *current_template_section_start = NULL;
    const char *current_template_section_end = NULL;
    size_t current_template_section_length = 0;


    /* 1. Header */
    //
    // error[E{ERROR_CODE}]

    printf("error[E%d]", error_code);


    /* 2. The error message itself */
    //
    // Uses the message template, in which any present definitions of custom
    // values will be replaced by the given custom values

    printf(": ");

    message_template = d_errors_get_template(error_code);
    message_template_length = strlen(message_template);
    message_template_end = message_template + message_template_length;

    current_template_section_start = message_template;


    do {
        // Searching how many characters can be printed until the next custom
        // value
        current_template_section_end = strstr(current_template_section_start,
                                            "[..]");

        if(current_template_section_end != NULL) {
            current_template_section_length = current_template_section_end -
                                              current_template_section_start;
        }

        else {
            // No custom value flags left in the template, so just print
            // everything
            current_template_section_length = message_template_end -
                                              current_template_section_start;
        }

        printf("%.*s", (int) current_template_section_length,
                        current_template_section_start);

        current_template_section_start += current_template_section_length;
        if(current_template_section_end != NULL) {
            // In order to also skip the custom value flag that has been
            // replaced
            current_template_section_start += 4;
        }

        // A custom value gap must be filled if found
        if(current_template_section_end != NULL) {

            // There may not be enough custom values to fill all gaps
            if(remaining_custom_values > 0) {

                printf("%s", va_arg(*custom_values, const char *));
                --remaining_custom_values;
            }

            else {
                printf("[..]");
            }
        }

    // While not all the template has been used
    } while(current_template_section_start < message_template_end);

    printf("\n");


    /* 3. Any metadata */

    if(metadata != NULL) {
        printf(" --> %s", metadata);
    }

    printf("\n\n");
}


/**
 * @brief Implementation of errors.h/d_errors_parse_show 
 */
void d_errors_parse_show(
    int arg_count,
    ...
)
{
    // List that contains all variable arguments
    va_list valist;
    
    int error_code = 0;
    size_t line;
    size_t col;

    
    if(arg_count < 3) {
        perror("error: invalid argument, 'arg_count' must be >= 3\n"
               " --> internal file : errors.c : d_errors_parse_show()");
    }


    // Parsing arguments...
    va_start(valist, arg_count);
    error_code = va_arg(valist, int);

    // Generating metadata about the error...
    line = va_arg(valist, size_t);
    col = va_arg(valist, size_t);
    snprintf(char_buffer, sizeof(char_buffer), "input file : ln %zu : col "
             "%zu", line, col);

    // Showing the whole error...
    _d_errors_show(error_code, char_buffer, arg_count - 3, &valist);

    // Cleaning up...
    va_end(valist);
}


/**
 * @brief Implementation of errors.h/d_errors_internal_show 
 */
void d_errors_internal_show(
    int arg_count,
    ...
)
{
    // List that contains all variable arguments
    va_list valist;
    
    int error_code = 0;
    char file[32];
    char function[64];


    if(arg_count < 3) {
        perror("error: invalid argument, 'arg_count' must be >= 3\n"
               " --> internal file : errors.c : d_errors_internal_show()");
    }


    // Parsing arguments...
    va_start(valist, arg_count);
    error_code = va_arg(valist, int);

    // Generating metadata about the error...
    strncpy(file, va_arg(valist, const char *), sizeof(file) - 1);
    strncpy(function, va_arg(valist, const char *), sizeof(function) - 1);
    snprintf(char_buffer, sizeof(char_buffer), "internal file : %s : %s()",
             file, function);

    // Showing the whole error...
    _d_errors_show(error_code, char_buffer, arg_count - 3, &valist);

    // Cleaning up...
    va_end(valist);
}
