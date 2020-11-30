/**
 * @file commands.c
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief Implementation of common/commands.h
 */


#include "common/commands.h"

#include "common/lexical_comp.h"
#include "common/symbol_table.h"

#include "lib/uthash.h"


#include <stdio.h>
// For dynamic library loading
#include <dlfcn.h>



/**
 * @brief This structure will allow delta to load multiple dynamic libraries.
 *
 * @details
 *  Delta will rely on a hash table to keep track of each dynamic library that
 *  has been loaded. In order to do so, this structure represents an entry in
 *  that table.
 */
struct _d_commands_table_entry {

    /** '\0' terminated string which is the given path through which the
        library has been found and loaded. */
    const char *path;
    /** The opened library to which the entry corresponds. */
    void *library;
    /** Makes this structure hashable by the library. */
    UT_hash_handle hh;
};

/** The library requires the hash table to be a pointer to its entry
    type, initialized to NULL. */
struct _d_commands_table_entry *_d_commands_table = NULL;

/**
 * The last selected library.
 */
struct _d_commands_table_entry *_last_library = NULL;


/**
 * @brief Clears the current workspace.
 *
 * @details
 *  Clears the current workspace; that is, all registered variables are
 *  deleted.
 * 
 * @return 0 if successful, any other value otherwise.
 */
int _d_commands_clear_workspace(
    void
)
{
    return d_symbol_table_delete(D_LC_IDENTIFIER_VARIABLE);
}


/**
 * @brief Opens a file which contains statements written in the delta's
 *        language.
 * 
 *
 * @details
 *  Opens a file which contains statements written in the delta's language.
 *  They are all executed as they are read.
 *
 * @param[in] filename Relative or absolute path to the file.
 *
 * @return 0 if sucessful, any other value otherwise.
 */
int _d_commands_load_file(
    const char *filename
)
{
    if(filename == NULL) {
        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "commands.c", "_d_commands_load_file",
                               "'filename'");
        return -1;
    }


    #ifdef D_DEBUG
    printf("[commands][load_file] Loading: %s\n", filename);
    #endif

    return d_lexical_analyzer_new_file(filename);
}


/**
 * @brief Loads a newly specified math function from the currently selected
 *        dynamic library.
 *
 * @details
 *  Loads a newly specified math function from the currently selected dynamic
 *  library. The function must follow the "math_functions/d_dec_function"
 *  prototype.
 * 
 * @param[in] function Name by which the function can be identified. A
 *                     function that goes by the same name must not be present
 *                     yet in the symbol table.
 *
 * @return 0 if sucessful, any other value otherwise.
 */
int _d_commands_load_function(
    const char *function
)
{
    void *loaded_object = NULL;


    if(_d_commands_table == NULL) {

        d_errors_internal_show(3, D_ERR_USER_IO_NO_DYN_LIBRARY_SELECTED,
                               "commands.c", "_d_commands_load_function");
        return -1;
    }

    if(function == NULL) {
        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "commands.c", "_d_commands_load_function",
                               "'function'");
        return -1;
    }


    #ifdef D_DEBUG    
    printf("[commands][load function] Clearing previous dl errors\n");
    dlerror();
    #endif

    #ifdef D_DEBUG
    printf("[commands][load function] Specified function: %s\n", function);
    #endif


    // If the function has not been loaded yet
    if(d_symbol_table_search(function) == NULL) {

        // If the library is not already loaded, there we go
        if((loaded_object = dlsym(_last_library->library, function))
           == NULL) {

            d_errors_internal_show(3, D_ERR_USER_IO_FUNCTION_INACCESSIBLE,
                                   "commands.c", "_d_commands_load_function");
            #ifdef D_DEBUG
            printf("[commands][load function] dl error: %s\n", dlerror());
            #endif

            return -1;
        }

        // Now the library can be added to the symbol table
        d_symbol_table_add_math_function(function,
                                         (d_dec_function)loaded_object);

        #ifdef D_DEBUG
        printf("[commands][load function] Dynamically loaded function: %s "
               "%p\n", function, loaded_object);
        #endif
    }

    else {
        d_errors_internal_show(3, D_ERR_USER_IO_FUNCTION_NAME_TAKEN,
                               "commands.c", "_d_commands_load_function");

        return -1;
    }
    

    return 0;
}


/**
 * @brief Searches for a dynamic library and loads it.
 *
 * @details
 *  Searches for a dynamic library and loads it, if it is not yet, while also
 *  setting it as the selected opened dynamic library. 
 * 
 * @param[in] path Relative or absolute path to the library.
 *
 * @return 0 if sucessful, any other value otherwise.
 */
int _d_commands_load_library(
    const char *path
)
{
    struct _d_commands_table_entry entry;
    struct _d_commands_table_entry *entry_in_table = NULL;


    if(path == NULL) {
        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "commands.c", "_d_commands_load_library",
                               "'path'");
        return -1;
    }


    #ifdef D_DEBUG    
    printf("[commands][load library] Clearing previous dl errors\n");
    dlerror();
    #endif

    #ifdef D_DEBUG
    printf("[commands][load library] Given path: %s\n", lexeme);
    #endif


    // Now the entry of the new library should be initialized in order to add
    // it

    // In order to do so, let's check first if a corresponding entry is
    // already present
    HASH_FIND_STR(_d_commands_table, (const char *)path, entry_in_table);

    if(entry_in_table == NULL) {

        #ifdef D_DEBUG
        printf("[commands][load library] Library not present yet\n");
        #endif

        // If the library is not already loaded, there we go
        if((entry.library = dlopen(path, RTLD_LAZY)) == NULL) {

            d_errors_internal_show(3, D_ERR_USER_IO_FILE_INACCESSIBLE,
                                   "commands.c", "_d_commands_load_library");
            #ifdef D_DEBUG
            printf("[commands][load library] dl error: %s\n", dlerror());
            #endif

            return -1;
        }

        // Saving the path through which the library has been accessed
        if((entry.path = (const char *) strdup(path)) == NULL) {

            d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                                   "commands.c", "_d_commands_load_library",
                                   "'strdup' of given path");
            return -1;
        }

        // Now the library can be added to the table
        // With each new entry, a new internally-managed structure is
        // allocated
        if((entry_in_table = malloc(sizeof(struct _d_commands_table_entry)))
           == NULL) {

            d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                                   "commands.c", "_d_commands_load_library",
                                   "'malloc' for struct "
                                   "_d_commands_table_entry");
            return -1;
        }

        entry_in_table->library = entry.library;
        entry_in_table->path = entry.path;
        // There is no need to copy the library's handle, it just needs to be
        // present

        HASH_ADD_KEYPTR(hh, _d_commands_table, entry_in_table->path,
                        strlen((const char *)entry_in_table->path),
                        entry_in_table);
    }


    // Anyways, the resulting entry, be it new or not, is set as the last
    // selected library
    _last_library = entry_in_table;


    return 0;
} /* TODO close all when exiting */


/**
 * @brief Shows generic help about delta.
 * 
 * @return 0 if successful, any other value otherwise.
 */
int _d_commands_show_help(
    void
)
{
    printf("[!] Help TODO"); // TODO just like README.md

    return 0;
}


/**
 * @brief Shows detailed help about the specified delta section.
 * 
 * @param[in] topic The section.
 *
 * @return 0 if successful, any other value otherwise.
 */
int _d_commands_show_detailed_help(
    const char *topic
)
{
    if(topic == NULL) {
        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "commands.c", "_d_commands_load_file",
                               "'topic'");
        return -1;
    }


    // TODO strcmp
    printf("[!] Help %s TODO", topic); // TODO just like README.md

    return 0;
}


/**
 * @brief Shows the current workspace.
 *
 * @details
 *  Shows the current workspace; that is, shows the symbol table.
 * 
 * @return 0 if successful, any other value otherwise.
 */
int _d_commands_show_workspace(
    void
)
{
    return d_symbol_table_show();
}


/**
 * @brief Returns the "quit" numeric code so that the lexical and semantic
 *        analyzer stops parsing.
 *
 * @details
 *  Returns the "quit" numeric code so that the lexical and semantic analyzer
 *  stops parsing. Naturally, as delta is supposed to quit, all dynamically
 *  loaded libraries while parsing are closed.
 * 
 * @return The "quit" numeric code, -1 if failed.
 */
int _d_commands_quit(
    void
)
{
    struct _d_commands_table_entry *current_entry;
    struct _d_commands_table_entry *tmp;


    if(_d_commands_table != NULL) {

        HASH_ITER(hh, _d_commands_table, current_entry, tmp) {

            // Closes the library and deletes its entry before proceeding to
            // the next one
            HASH_DEL(_d_commands_table, current_entry);

            // Closing the library implies automatically closing all loaded
            // functions that are present in it
            if(dlclose(current_entry->library) != 0) {

                d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                                       "commands.c", "_d_commands_quit",
                                       "'dlclose' on a dynamically loaded "
                                       "library");
                return -1;
            }

            free((char *) current_entry->path);
            free(current_entry);
        }
    }

    _d_commands_table = NULL;
    _last_library = NULL;


    return D_COMMAND_QUIT_REQUEST;
}


/** 0 args commands **/

/** Which names the user may specify to call the no arg built-in commands. */
const char *D_COMMANDS_NAMES_0[] = {
    "help",
    "quit",
    "ws",
    "wsc"
};

/** Pointers to the implementations of the no arg built-in commands. */
const comm_function_0 D_COMMANDS_IMPLEMENTATIONS_0[] = {
    &_d_commands_show_help,
    &_d_commands_quit,
    &_d_commands_show_workspace,
    &_d_commands_clear_workspace,
};


/** 1 args commands **/

/** Which names the user may specify to call the 1 arg built-in commands. */
const char *D_COMMANDS_NAMES_1[] = {
    "dhelp",
    "import",
    "from",
    "load",
};

/** Pointers to the implementations of the 1 arg built-in commands. */
const comm_function_1 D_COMMANDS_IMPLEMENTATIONS_1[] = {
    &_d_commands_show_detailed_help,
    &_d_commands_load_function,
    &_d_commands_load_library,
    &_d_commands_load_file,
};
