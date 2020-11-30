/**
 * @file symbol_table.h
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief This is the symbol table that delta will use.
 *
 * @details
 *  This is the definition of the symbol table that delta will use through all
 *  of its interpretation phases. It will allow delta to keep track, at each
 *  time point, of the currently active identifiers, math functions and built-
 *  in commands, as well as all of their attributes.
 *
 *  The symbol table is internally represented by a single hash table, as
 *  there is no scoping at the moment. Specifically, the "uthash" library by
 *  Troy D. Hanson is the one who provides the whole hash table functionality.
 *
 *  It is also worth noting that, once the symbol table is created, its
 *  initial contents are:
 *
 *    - The built-in commands in common/commands.h
 *    - The math constants in common/math_constants.h
 *    - The math functions in common/math_functions.h
 *
 *  This will allow telling those elements apart from variables' identifiers,
 *  as they are all alphanumeric strings.
 */


#ifndef D_SYMBOL_TABLE
#define D_SYMBOL_TABLE


#include "common/commands.h"
#include "common/dec_numbers.h"
#include "common/math_functions.h"

// External library 
#include "lib/uthash.h"


/**
 * @brief Represents a symbol table.
 *
 * @details
 *  Opaque data type which represents a symbol table, composed of multiple
 *  entries.
 */
struct d_symbol_table;


/**
 * @brief Represents an entry in the symbol table.
 *
 * @details
 *  Data type which represents an entry in the symbol table. Each entry is
 *  identified by a char * member.
 */
struct d_symbol_table_entry {

    /** '\0' terminated string which represents the lexeme that originated the
        entry, as well as serving as its key. It will be automatically freed
        once the entry is deleted. */
    const unsigned char *lexeme;

    /** Lexical component which corresponds to the entry's lexeme. */
    int lexical_component;

    /** Union that holds any attribute of any kind that an entry may
        require. */
    union {
        /** For decimal variables and constants. */
        struct d_dec_number dec_number;
        /** For mathematical functions for decimal values. */
        dec_function function;
        /** For built-in commands. */
        struct d_command command; 
    } attribute;

    /** Makes this structure hashable by the library. */
    UT_hash_handle hh;
};


/**
 * @brief Initializes the symbol table.
 *
 * @details
 *  Initializes the symbol table, filling it with all the math constants and
 *  functions, and built-in commands.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_symbol_table_initialize();


/**
 * @brief Searches in the symbol table for the specified entry.
 *
 * @details
 *  Searches in the symbol table an entry identified by the given key, and
 *  returning it if found.
 *
 * @param[in] key '\0' terminated string which represents the key of the
 *                entry.
 *
 * @return Pointer to the requested entry, or NULL if not found.
 */
struct d_symbol_table_entry *d_symbol_table_search(
    const unsigned char* key
);


/**
 * @brief Adds a new entry to the symbol table.
 *
 * @details
 *  Adds the specified entry to the symbol table. The entry's key must NOT be
 *  present yet.
 *
 *  Every member in the given entry will be copied to a new entry internally
 *  managed by the symbol table.
 *
 *  WARNING: if any member points to a certain memory region, just the pointer
 *           itself will be copied, whereas the contents of the region will
 *           not.
 *
 * @param[in] entry The entry.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_symbol_table_add(
    struct d_symbol_table_entry *entry
);


/**
 * @brief Adds a new entry in the symbol table for the specified math
 *        function.
 *
 * @details
 *  Adds a new entry in the symbol table for the specified math function. The
 *  entry's key must NOT be present yet; that is, a function that goes by the
 *  same name.
 *
 * @param[in] funcion_name Name by which the function will be identified.
 * @param[in] function_implementation Where the function's implementation can
 *            be found.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_symbol_table_add_math_function(
    const char *function_name,
    dec_function function_implementation
);


/**
 * @brief Shows the symbol table.
 *
 * @details
 *  Prints all the runtime-dependant entries in the symbol table:
 *
 *    - Math constants.
 *    - Math loaded functions.
 *    - Variables.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_symbol_table_show();


/**
 * @brief Deletes all entries that belong to the specified lexical component.
 *
 * @param[in] lexical_component The lexical component category.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_symbol_table_delete(
    int lexical_component
);


/**
 * @brief Destroys the symbol table.
 *
 * @details
 *  Destroys the symbol table, as well as all of its entries.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_symbol_table_destroy();


#endif //D_SYMBOL_TABLE
