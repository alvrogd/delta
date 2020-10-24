/**
 * @file symbol_table.c
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief Implementation of common/symbol_table.h
 */


#include "common/symbol_table.h"


#include <stdio.h>
#include <stdlib.h>

// strlen
#include <string.h>


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
 * @brief Implementation of symbol_table.h/d_symbol_table_initialize
 */
int d_symbol_table_initialize(
    struct d_symbol_table **symbol_table
)
{
    if(symbol_table == NULL) {

        perror("ERROR::SYMBOL_TABLE::Reference to struct d_symbol_table is "
               "NULL");
        return -1;
    }

    // The structure that represents the symbol table gets initialized, as
    // well as all its only member, which must point to NULL in order for the
    // library to properly work

    if((*symbol_table = malloc(sizeof(struct d_symbol_table))) == NULL) {

        perror("ERROR::SYMBOL_TABLE::Could not allocate a struct "
               "d_symbol_table");
        return -1;
    }

    (*symbol_table)->table = NULL;



    // TODO fill w/ all keywords

    return 0;
}


/**
 * @brief Searches in the symbol table for the specified entry.
 *
 * @details
 *  Searches in the symbol table an entry identified by the given key, and
 *  returning it if found.
 *
 * @param[in,out] symbol_table The symbol table.
 * @param[in] '\0' terminated string which represents the key of the entry.
 *
 * @return Pointer to the requested entry, or NULL if not found.
 */
struct d_symbol_table_entry *d_symbol_table_search(
    struct d_symbol_table *symbol_table,
    const char* key
)
{
    struct d_symbol_table_entry *entry = NULL;


    if(symbol_table == NULL) {

        perror("ERROR::SYMBOL_TABLE::Reference to struct d_symbol_table is "
               "NULL");
        return NULL;
    }

    if(key == NULL) {

        perror("ERROR::SYMBOL_TABLE::Reference to key is NULL");
        return NULL;
    }


    // Returns NULL if no corresponding entry is found
    HASH_FIND_KEYPTR(symbol_table->table, key, entry);


    return entry;
}


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
 * @return 0 if successful, any other value otherwise.
 */
int d_symbol_table_add(
    struct d_symbol_table *symbol_table,
    struct d_symbol_table_entry *entry
)
{
    struct d_symbol_table_entry *internal_entry;


    if(symbol_table == NULL) {

        perror("ERROR::SYMBOL_TABLE::Reference to struct d_symbol_table is "
               "NULL");
        return -1;
    }

    if(entry == NULL) {

        perror("ERROR::SYMBOL_TABLE::Reference to entry is NULL");
        return -1;
    }


    // No entry with the same key can be already present in the table
    if(d_symbol_table_search(symbol_table, entry->key) != NULL) {

        perror("ERROR::SYMBOL_TABLE::Duped key");
        return -1;
    }

    // With each new entry, a new internally-managed structure is allocated
    if((*internal_entry = malloc(sizeof(struct d_symbol_table_entry))) ==
       NULL) {

        perror("ERROR::SYMBOL_TABLE::Could not allocate a struct "
               "d_symbol_table_entry");
        return -1;
    }

    // The new struture just copies value by value the given struct's members
    internal_entry->lexeme = entry->lexeme;
    internal_entry->lexical_component = entry->lexical_component;
    // There is no need to copy the library's handle, it just needs to be
    // present

    // As previously said, the "lexeme" member is used as the key
    HASH_ADD_KEYPTR(hh, symbol_table->table, internal_entry->lexeme,
                    strlen(internal_entry->lexeme), internal_entry);


    return 0;
}


/**
 * @brief Destroys a symbol table.
 *
 * @details
 *  Destroys the specified symbol table, as well as all of its entries.
 *
 * @param[out] symbol_table Reference to the pointer where the address of the
 *                          symbol table can be found.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_symbol_table_destroy(
    struct d_symbol_table **symbol_table
)
{
    struct d_symbol_table_entry *current_entry;
    struct d_symbol_table_entry *tmp;


    if(symbol_table == NULL) {

        perror("ERROR::SYMBOL_TABLE::Reference to struct d_symbol_table is "
               "NULL");
        return -1;
    }

    
    // This "all deletion" procedure is directly taken from the library's
    // reference
    HASH_ITER(hh, (*symbol_table)->table, current_entry, tmp) {

        // Deletes the first entry and proceeds to the next one
        HASH_DEL((*symbol_table)->table, current_entry);

        // Each internally-managed entry must be properly freed
        free(current_entry);
    }

    // The symbol table structure itself must also be freed
    free(*symbol_table);


    return 0;
}
