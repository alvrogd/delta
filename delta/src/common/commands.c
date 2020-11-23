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


#include <dlfcn.h>
#include <stdio.h>
// TODO '_' in function names

/**
 * @brief This structure will allow the lexical analyzer to load multiple
 *        dynamic libraries.
 *
 * @details
 *  A stack will be simulated.
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


int d_commands_clear_workspace()
{
    return d_symbol_table_delete(D_LC_IDENTIFIER_VARIABLE);
}

int d_commands_load_file(
    const char *filename
)
{
    printf("d_commands_load_file %s\n", filename);
    return d_lexical_analyzer_new_file(filename);
}

int d_commands_load_function(
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

int d_commands_load_library(
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

int d_commands_show_help()
{
    printf("[!] Help TODO"); // TODO just like README.md

    return 0;
}

int d_commands_show_detailed_help(
    const char *topic
)
{
    // TODO strcmp
    printf("[!] Help %s TODO", topic); // TODO just like README.md

    return 0;
}

int d_commands_show_workspace()
{
    return d_symbol_table_show();
}

int d_commands_quit()
{
    return D_COMMAND_QUIT_REQUEST;
}


const char *D_COMMANDS_NAMES_0[] = {
    "help",
    "quit",
    "ws",
    "wsc"
};

const comm_function_0 D_COMMANDS_IMPLEMENTATIONS_0[] = {
    &d_commands_show_help,
    &d_commands_quit,
    &d_commands_show_workspace,
    &d_commands_clear_workspace,
};

const char *D_COMMANDS_NAMES_1[] = {
    "dhelp",
    "import",
    "from",
    "load",
};

const comm_function_1 D_COMMANDS_IMPLEMENTATIONS_1[] = {
    &d_commands_show_detailed_help,
    &d_commands_load_function,
    &d_commands_load_library,
    &d_commands_load_file,
};
