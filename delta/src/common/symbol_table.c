/**
 * @file symbol_table.c
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief Implementation of common/symbol_table.h
 */

#include "common/symbol_table.h"

#include "common/errors.h"
#include "common/lexical_comp.h"

#include "common/commands.h"
#include "common/math_constants.h"
#include "common/math_functions.h"


#include <stdio.h>
#include <stdlib.h>

// strcpy, strlen
#include <string.h>

// To print int64_t data type
#include <inttypes.h>


/**
 * @brief Represents a symbol table.
 *
 * @details
 *  Data type which represents a symbol table, composed of multiple entries.
 */
struct d_symbol_table {
    /** The library requires the hash table to be a pointer to its entry
        type. */
    struct d_symbol_table_entry *table;
};


/**
 * @brief Globally accessible symbol table that will be used.
 */
struct d_symbol_table *symbol_table = NULL;


/**
 * @brief Implementation of symbol_table.h/d_symbol_table_initialize
 */
int d_symbol_table_initialize(
    void
)
{
    struct d_symbol_table_entry tmp_entry;

    size_t i = 0;

    size_t constant_count = sizeof(D_MATH_CONSTANTS_NAMES) /
                            sizeof(D_MATH_CONSTANTS_NAMES[0]);

    // All math functions take 1 parameter
    size_t function_count = sizeof(D_MATH_FUNCTIONS_NAMES) /
                            sizeof(D_MATH_FUNCTIONS_NAMES[0]);

    // There are built-in commands that take no parameters, and there those
    // that take 1 parameter
    size_t command_count = sizeof(D_COMMANDS_NAMES_0) /
                           sizeof(D_COMMANDS_NAMES_0[0]);
    size_t command_count_1 = sizeof(D_COMMANDS_NAMES_1) /
                             sizeof(D_COMMANDS_NAMES_1[0]);



    // The structure that represents the symbol table gets initialized, as
    // well as all its only member, which must point to NULL in order for the
    // library to properly work

    if((symbol_table = malloc(sizeof(struct d_symbol_table))) == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                               "symbol_table.c", "d_symbol_table_initialize",
                               "'malloc' for struct d_symbol_table");
        return -1;
    }

    symbol_table->table = NULL;


    // TODO It would be nice to not replicate all the code lines in the
    // following loops. A generic function could iterate a given array's
    // values, create the entries, store the lexemes, and set the specified
    // lex. component category. Meanwhile, a specialized function would be
    // called for each entry that spawns from the array, to fill up the
    // entry's attributes that are expected.

    for(i = 0; i < constant_count; ++i) {

        if((tmp_entry.lexeme = strdup(D_MATH_CONSTANTS_NAMES[i])) == NULL) {

            d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                                   "symbol_table.c",
                                   "d_symbol_table_initialize",
                                   "'strdup' on math constant's lexeme");
            return -1;
        }

        tmp_entry.lexical_component = D_LC_IDENTIFIER_CONSTANT;

        // They are all floating point numbers
        tmp_entry.attribute.dec_number.values.floating =
                                                   D_MATH_CONSTANTS_VALUES[i];
        tmp_entry.attribute.dec_number.is_floating = 1;

        d_symbol_table_add(&tmp_entry);

        #ifdef D_DEBUG
        printf("[symbol_table][initialize] Added constant: %s %f\n",
               tmp_entry.lexeme,
               tmp_entry.attribute.dec_number.values.floating);
        #endif
    }


    for(i = 0; i < function_count; ++i) {

        d_symbol_table_add_math_function(D_MATH_FUNCTIONS_NAMES[i],
                                         D_MATH_FUNCTIONS_IMPLEMENTATIONS[i]);

        #ifdef D_DEBUG
        printf("[symbol_table][initialize] Added function: %s %p\n",
               tmp_entry.lexeme, D_MATH_FUNCTIONS_IMPLEMENTATIONS[i]);
        #endif
    }


    for(i = 0; i < command_count; ++i) {

        if((tmp_entry.lexeme = strdup(D_COMMANDS_NAMES_0[i])) == NULL) {

            d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                                   "symbol_table.c",
                                   "d_symbol_table_initialize",
                                   "'strdup' on 0 arg command's lexeme");
            return -1;
        }

        tmp_entry.lexical_component = D_LC_IDENTIFIER_COMMAND;

        tmp_entry.attribute.command.implementation.argc_0 =
                                              D_COMMANDS_IMPLEMENTATIONS_0[i];
        tmp_entry.attribute.command.arg_count = 0;

        d_symbol_table_add(&tmp_entry);

        #ifdef D_DEBUG
        printf("[symbol_table][initialize] Added 0 arg command: %s %p %p\n",
               tmp_entry.lexeme, D_COMMANDS_IMPLEMENTATIONS_0[i],
               tmp_entry.attribute.command.implementation.argc_0);
        #endif
    }


    for(i = 0; i < command_count_1; ++i) {

        if((tmp_entry.lexeme = strdup(D_COMMANDS_NAMES_1[i])) == NULL) {

            d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                                   "symbol_table.c",
                                   "d_symbol_table_initialize",
                                   "'strdup' on 1 arg command's lexeme");
            return -1;
        }

        tmp_entry.lexical_component = D_LC_IDENTIFIER_COMMAND;

        tmp_entry.attribute.command.implementation.argc_1 =
                                              D_COMMANDS_IMPLEMENTATIONS_1[i];
        tmp_entry.attribute.command.arg_count = 1;

        d_symbol_table_add(&tmp_entry);

        #ifdef D_DEBUG
        printf("[symbol_table][initialize] Added 1 arg command: %s %p %p\n",
               tmp_entry.lexeme, D_COMMANDS_IMPLEMENTATIONS_1[i],
               tmp_entry.attribute.command.implementation.argc_1);
        #endif
    }


    return 0;
}


/**
 * @brief Implementation of symbol_table.h/d_symbol_table_search
 */
struct d_symbol_table_entry *d_symbol_table_search(
    const char* key
)
{
    struct d_symbol_table_entry *entry = NULL;


    if(symbol_table == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "symbol_table.c", "d_symbol_table_search",
                               "'symbol_table'");
        return NULL;
    }

    if(key == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "symbol_table.c", "d_symbol_table_search",
                               "'key'");
        return NULL;
    }


    // Returns NULL if no corresponding entry is found
    HASH_FIND_STR(symbol_table->table, (const char *)key, entry);


    return entry;
}


/**
 * @brief Implementation of symbol_table.h/d_symbol_table_add
 */
int d_symbol_table_add(
    struct d_symbol_table_entry *entry
)
{
    struct d_symbol_table_entry *internal_entry;


    if(symbol_table == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "symbol_table.c", "d_symbol_table_add",
                               "'symbol_table'");
        return -1;
    }

    if(entry == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "symbol_table.c", "d_symbol_table_add",
                               "'entry'");
        return -1;
    }


    #ifdef D_DEBUG
    // Should already be checked from the caller
    // No entry with the same key can be already present in the table
    if(d_symbol_table_search(entry->lexeme) != NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_LOGIC, "symbol_table.c",
                               "d_symbol_table_add", "key already present");
        return 1;
    }
    #endif

    // With each new entry, a new internally-managed structure is allocated
    if((internal_entry = malloc(sizeof(struct d_symbol_table_entry))) ==
       NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                               "symbol_table.c", "d_symbol_table_add",
                                "'malloc' for struct d_symbol_table_entry");
        return -1;
    }

    // The new struture just copies value by value the given struct's members
    internal_entry->lexeme = entry->lexeme;
    internal_entry->lexical_component = entry->lexical_component;
    internal_entry->attribute = entry->attribute;
    // There is no need to copy the library's handle, it just needs to be
    // present

    // As previously said, the "lexeme" member is used as the key
    HASH_ADD_KEYPTR(hh, symbol_table->table, internal_entry->lexeme,
                    strlen((const char *)internal_entry->lexeme),
                    internal_entry);


    return 0;
}


/**
 * @brief Implementation of symbol_table.h/d_symbol_table_add_math_function
 */
int d_symbol_table_add_math_function(
    const char *function_name,
    d_dec_function function_implementation
)
{
    struct d_symbol_table_entry internal_entry;


    if(symbol_table == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "symbol_table.c",
                               "d_symbol_table_add_math_function",
                               "'symbol_table'");
        return -1;
    }

    if(function_name == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "symbol_table.c",
                               "d_symbol_table_add_math_function",
                               "'function_name'");
        return -1;
    }


    // The function's name is its lexeme
    if((internal_entry.lexeme = strdup(function_name)) == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                                "symbol_table.c",
                                "d_symbol_table_add_math_function",
                                "'strdup' on 1 math function's lexeme");
        return -1;
    }

    internal_entry.lexical_component = D_LC_IDENTIFIER_FUNCTION;

    internal_entry.attribute.function = function_implementation;


    return d_symbol_table_add(&internal_entry);
}


/**
 * @brief Implementation of symbol_table.h/d_symbol_table_show
 */
int d_symbol_table_show(
    void
)
{
    struct d_symbol_table_entry *current_entry;
    struct d_symbol_table_entry *tmp;


    if(symbol_table == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "symbol_table.c", "d_symbol_table_show",
                               "'symbol_table'");
        return -1;
    }


    // TODO This is not the most efficient way of showing the symbol table, as
    // it needs to iterate it as many times as categories will be shown. It
    // has been done this way just to quickly avoid mixing-up the categories'
    // entries, but it would need to be rewritten in order to be efficient for
    // larger use-cases

    printf("   1. Mathematical constants:\n");

    // This iteration procedure is directly taken from the library's
    // reference
    HASH_ITER(hh, symbol_table->table, current_entry, tmp) {   

        if(current_entry->lexical_component == D_LC_IDENTIFIER_CONSTANT) {

            printf("      %s => %f\n", current_entry->lexeme,
                   current_entry->attribute.dec_number.values.floating);
        }
    }


    printf("   2. Loaded mathematical functions:\n");

    HASH_ITER(hh, symbol_table->table, current_entry, tmp) {   

        if(current_entry->lexical_component == D_LC_IDENTIFIER_FUNCTION) {

            printf("      %s\n", current_entry->lexeme);
        }
    }


    printf("   3. Your variables:\n");

    HASH_ITER(hh, symbol_table->table, current_entry, tmp) {   

        if(current_entry->lexical_component == D_LC_IDENTIFIER_VARIABLE) {

            if(current_entry->attribute.dec_number.is_floating) {
                printf("      [%s] %s => %.10g\n", "floating",
                       current_entry->lexeme,
                       current_entry->attribute.dec_number.values.floating);
            }

            else {
                printf("      [%s] %s => %" PRId64 "\n", "integer",
                       current_entry->lexeme,
                       current_entry->attribute.dec_number.values.integer);
            }
        }
    }


    return 0;
}


/**
 * @brief Implementation of symbol_table.h/d_symbol_table_delete
 */
int d_symbol_table_delete(
    int lexical_component
)
{
    struct d_symbol_table_entry *current_entry;
    struct d_symbol_table_entry *tmp;


    if(symbol_table == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "symbol_table.c", "d_symbol_table_delete",
                               "'symbol_table'");
        return -1;
    }

    
    // This iteration procedure is directly taken from the library's reference
    HASH_ITER(hh, symbol_table->table, current_entry, tmp) {

        // Deletes the entry, if applicable
        if(current_entry->lexical_component == lexical_component) {

            HASH_DEL(symbol_table->table, current_entry);

            // Each internally-managed entry must be properly freed
            free((char *) current_entry->lexeme);
            free(current_entry);
        }
    }


    return 0;
}


/**
 * @brief Implementation of symbol_table.h/d_symbol_table_destroy
 */
int d_symbol_table_destroy(
    void
)
{
    struct d_symbol_table_entry *current_entry;
    struct d_symbol_table_entry *tmp;


    if(symbol_table == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "symbol_table.c", "d_symbol_table_destroy",
                               "'symbol_table'");
        return -1;
    }

    
    // This "all deletion" procedure is directly taken from the library's
    // reference
    HASH_ITER(hh, symbol_table->table, current_entry, tmp) {

        // Deletes the entry before proceeding to the next one
        HASH_DEL(symbol_table->table, current_entry);

        // Each internally-managed entry must be properly freed
        free((char *) current_entry->lexeme);
        free(current_entry);
    }

    // The symbol table structure itself must also be freed
    free(symbol_table);


    return 0;
}
