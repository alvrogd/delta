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


#include <stdio.h>
// For dynamic library loading
#include <dlfcn.h>


/**
 * @brief This structure will allow delta to load multiple dynamic libraries.
 *
 * @details
 *  This structure will allow delta to load multiple dynamic libraries, which
 *  will be saved using a stack.
 */
struct _d_commands_bufstack {
    /** Previous entry. */
    struct _d_lexical_analyzer_bufstack *previous;
    /** Opened library. */
    void *library;
};

/** The stack is empty at first. */
struct _d_commands_bufstack *current_d_commands_bufstack = NULL;
void *last_library = NULL;


/**
 * @brief Clears the current workspace.
 *
 * @details
 *  Clears the current workspace; that is, all registered variables are
 *  deleted.
 * 
 * @return 0 if successful, any other value otherwise.
 */
int _d_commands_clear_workspace()
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
 * @brief Loads a newly specified math function from the last opened dynamic
 *        library.
 *
 * @details
 *  Loads a newly specified math function from the last opened dynamic
 *  library. The function must follow the "math_functions/dec_function"
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
    struct d_symbol_table_entry tmp_entry;
    unsigned char *tmp_entry_lexeme = NULL;

    void *loaded_object = NULL;

    printf("3 %s\n", dlerror());

    // TODO use same function as in symbol table
    if((tmp_entry_lexeme =
        malloc(strlen(function) + 1)) == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                                "commands.c",
                                "d_commands_load_function",
                                "'malloc' for math function's lexeme");
        return -1;
    }

    strcpy((char *)tmp_entry_lexeme, function);
    tmp_entry.lexeme = tmp_entry_lexeme;

    tmp_entry.lexical_component = D_LC_IDENTIFIER_FUNCTION;

    tmp_entry.attribute.function = (dec_function)
                                    dlsym(last_library, function);
    printf("F %d\n", last_library == NULL);
    d_symbol_table_add(&tmp_entry);

    //#ifdef DEBUG
    printf("[symbol_table][initialize] Added function: %s %p\n",
            tmp_entry.lexeme, tmp_entry.attribute.function);
    //#endif
    printf("4 %s\n", dlerror());


    return 0;
}


/**
 * @brief Searches for a dynamic library and loads it.
 *
 * @details
 *  Searches for a dynamic library and loads it, if it is not yet, while also
 *  setting it as the last opened dynamic library. 
 * 
 * @param[in] function Relative or absolute path to the library.
 *
 * @return 0 if sucessful, any other value otherwise.
 */
int _d_commands_load_library(
    const char *filename
)
{
        printf("1 %s\n", dlerror());

    /* TODO add error handling */
    void *library = dlopen(filename, RTLD_LAZY);
    struct _d_commands_bufstack *bs = malloc(sizeof(struct _d_commands_bufstack));

    bs->previous = current_d_commands_bufstack;
    bs->library = last_library;

    last_library = library;
    current_d_commands_bufstack = bs;
    printf("2 %s\n", dlerror());


    return 0;
} /* TODO close all when exiting */


/**
 * @brief Shows generic help about delta.
 * 
 * @return 0 if successful, any other value otherwise.
 */
int _d_commands_show_help()
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
int _d_commands_show_workspace()
{
    return d_symbol_table_show();
}


/**
 * @brief Returns the "quit" numeric code so that the lexical and semantic
 *        analyzer stops parsing.
 * 
 * @return The "quit" numeric code.
 */
int _d_commands_quit()
{
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
