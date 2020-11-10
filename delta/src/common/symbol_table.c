/**
 * @file symbol_table.c
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief Implementation of common/symbol_table.h
 */


#include "common/symbol_table.h"
#include "common/keywords.h"
#include "common/lexical_components.h"
#include "common/errors.h"


#include <stdio.h>
#include <stdlib.h>

// strcpy, strlen
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
 * @brief Globally accessible symbol table that will be used.
 */
struct d_symbol_table *symbol_table = NULL;


/**
 * @brief Implementation of symbol_table.h/d_symbol_table_initialize
 */
int d_symbol_table_initialize()
{
    size_t i = 0;
    size_t keyword_count = sizeof(D_LANG_KEYWORDS) /
                           sizeof(D_LANG_KEYWORDS[0]);

    struct d_symbol_table_entry tmp_entry;
    unsigned char *tmp_entry_lexeme = NULL;


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

    // And the symbol table initialization will be completed once all of the
    // registered keywords are put into it
    for(i = 0; i < keyword_count; ++i) {

        // The lexeme is the keyword itself; a '\0' char must be present at
        // the end
        if((tmp_entry_lexeme = malloc(strlen(D_LANG_KEYWORDS[i]) + 1)) ==
            NULL) {

            d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                                   "symbol_table.c",
                                   "d_symbol_table_initialize",
                                   "'malloc' for keyword's lexeme");
            return -1;
        }

        strcpy((char *)tmp_entry_lexeme, D_LANG_KEYWORDS[i]);
        tmp_entry.lexeme = tmp_entry_lexeme;

        tmp_entry.lexical_component = D_LC_KEYWORD;

        d_symbol_table_add(&tmp_entry);

        #ifdef DEBUG
        printf("[symbol_table][initialize] Added kwd.: %s\n",
               tmp_entry.lexeme);
        #endif
    }
    

    #ifdef DEBUG
    printf("[symbol_table][initialize] Added kwd.: %s\n",
            tmp_entry.lexeme);
    printf("[symbol_table][initialize] Kwd. \"int\" present: %d\n",
           d_symbol_table_search(symbol_table, "int") != NULL);
    printf("[symbol_table][initialize] Kwd. \"potato\" present: %d\n",
           d_symbol_table_search(symbol_table, "potato") != NULL);
    printf("[symbol_table][initialize] Kwd. \"string\" present: %d\n",
           d_symbol_table_search(symbol_table, "string") != NULL);
    #endif


    return 0;
}


/**
 * @brief Implementation of symbol_table.h/d_symbol_table_search
 */
struct d_symbol_table_entry *d_symbol_table_search(
    const unsigned char* key
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


    // No entry with the same key can be already present in the table
    if(d_symbol_table_search(entry->lexeme) != NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_LOGIC, "symbol_table.c",
                               "d_symbol_table_add", "key already present");
        return 1;
    }

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
    // There is no need to copy the library's handle, it just needs to be
    // present

    // As previously said, the "lexeme" member is used as the key
    HASH_ADD_KEYPTR(hh, symbol_table->table, internal_entry->lexeme,
                    strlen((const char *)internal_entry->lexeme), internal_entry);


    return 0;
}


/**
 * @brief Implementation of symbol_table.h/d_symbol_table_show
 */
int d_symbol_table_show()
{
    struct d_symbol_table_entry *current_entry;
    struct d_symbol_table_entry *tmp;


    if(symbol_table == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "symbol_table.c", "d_symbol_table_show",
                               "'symbol_table'");
        return -1;
    }


    printf("<SYMBOL_TABLE>\n");

    // This iteration procedure is directly taken from the library's
    // reference
    HASH_ITER(hh, symbol_table->table, current_entry, tmp) {   

        printf("\t<SYMBOL_TABLE_ENTRY, %s, %s>\n",
               d_lc_to_string(current_entry->lexical_component),
               current_entry->lexeme);
    }

    printf("\n");


    return 0;
}


/**
 * @brief Implementation of symbol_table.h/d_symbol_table_destroy
 */
int d_symbol_table_destroy()
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

        // Deletes the first entry and proceeds to the next one
        HASH_DEL(symbol_table->table, current_entry);

        // Each internally-managed entry must be properly freed
        free((unsigned char *) current_entry->lexeme);
        free(current_entry);
    }

    // The symbol table structure itself must also be freed
    free(symbol_table);


    return 0;
}
