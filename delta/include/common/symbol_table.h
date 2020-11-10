/**
 * @file symbol_table.h
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief This is the symbol table that the delta compiler will use.
 *
 * @details
 *  This is the definition of the symbol table that the delta compiler will
 *  use through all of its compilation phases. It will allow the compiler to
 *  keep track, at each time point, of the currently active identifiers, as
 *  well as all of their attributes.
 *
 *  As of now, it does not support scoping, as it assumes that all source code
 *  can be found in the same block.
 *
 *  The symbol table is internally represented by a single hash table, as
 *  there is no scoping at the moment. Specifically, the "uthash" library by
 *  Troy D. Hanson is the one who provides the whole hash table functionality.
 *
 *  It is also worth noting that, once the symbol table is created, its
 *  initial contents are the keywords in common/keywords.h, which need to be
 *  present in order to tell them apart from the source code's identifiers.
 */


#ifndef D_SYMBOL_TABLE
#define D_SYMBOL_TABLE


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
    /** Makes this structure hashable by the library. */
    UT_hash_handle hh;
};


/**
 * @brief Globally accessible symbol table that will be used.
 */
struct d_symbol_table *symbol_table;


/**
 * @brief Initializes the symbol table.
 *
 * @details
 *  Initializes the symbol table, filling it with all the reserved keywords in 
 *  keywords.h
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
 * @param[in] '\0' terminated string which represents the key of the entry.
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
 *  already present.
 *
 *  Every member in the given entry will be copied to a new entry internally
 *  managed by the symbol table.
 *
 *  WARNING: if any member points to a certain memory region, just the pointer
 *           itself will be copied, whereas the contents of the region will
 *           not.
 *
 * @param[in,out] symbol_table The symbol table.
 * @param[in] The entry.
 *
 * @return 0 if successful, 1 if the key is duped, any other value otherwise.
 */
int d_symbol_table_add(
    struct d_symbol_table_entry *entry
);


/**
 * @brief Shows the symbol table.
 *
 * @details
 *  Shows the symbol table. That is, this function prints all of its entries
 *  telling apart identifiers from keywords.
 * *
 * @return 0 if successful, any other value otherwise.
 */
int d_symbol_table_show();


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
