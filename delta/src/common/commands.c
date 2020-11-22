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


int d_commands_clear_workspace()
{
    return d_symbol_table_delete(D_LC_IDENTIFIER_VARIABLE);
}


int d_commands_show_help()
{
    printf("[!] Help TODO"); // TODO just like README.md

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
    "load",
};

const comm_function_1 D_COMMANDS_IMPLEMENTATIONS_1[] = {
    NULL,
};
