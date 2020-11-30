/**
 * @file commands.h
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief This file contains the utilities through which delta implements its
 *        built-in commands.
 *
 * @details
 *  This file contains the utilities through which delta implements its
 *  built-in commands.
 *
 *  A command is a prefixed function, which may take 0 or 1 arguments (more
 *  specifically, a string). The currently supported commands are:
 *
 *    - help(): shows generic help about delta.
 *    - dhelp(section): shows detailed help about the specified delta section.
 *
 *    - from(library_path): searches for a dynamic library and loads it, if it
 *                          is not yet, while also setting it as the last
 *                          opened dynamic library. 
 *    - import(function): loads a newly specified math function from the last
 *                        opened dynamic library; the function must follow the
 *                        "math_functions/d_dec_function" prototype.
 *
 *    - load(file): opens a file which contains statements written in the
 *                  delta's language; they are all executed as they are read.
 *
 *    - ws(): shows the current workspace; that is, shows the symbol table.
 *    - wcs(): clears all registered variables in the current workspace.
 *
 *    - quit(): exits delta.
 */


#ifndef D_COMMANDS
#define D_COMMANDS


/** Which numeric code the "quit" command will return in order to point out
    that the program must end its execution. This value must be understood by
    the lexical and semantic analyzer in order to stop parsing. */
#define D_COMMAND_QUIT_REQUEST 160


/** Templates for the functions that implement the built-in commands. */
typedef int (*comm_function_0) (void);
typedef int (*comm_function_1) (const char *);


/**
 * @brief Groups both templates as a single element. 
 * 
 * @details
 *  Groups the "comm_function_{0,1}" templates as a single element, so that
 *  the "d_commmand" structure may carry both types of pointers at the same
 *  time.
 */
typedef union {
    comm_function_0 argc_0;
    comm_function_1 argc_1;
} comm_function;


/**
 * @brief Represents the attributes of a delta command.
 *
 * @details
 *  Contains any attributes of a delta command that the symbol table needs to
 *  hold in order to invoke it.
 */
struct d_command {
    /** Pointer to where the command's implementation can be found. */
    comm_function implementation;

    /** How many arguments the command receives. As of now, a command may
        receive one argument at most. */
    int arg_count;
};


/** 0 args commands **/

/** Which names the user may specify to call the no arg built-in commands. */
extern const char *D_COMMANDS_NAMES_0[4];

/** Pointers to the implementations of the no arg built-in commands. */
extern const comm_function_0 D_COMMANDS_IMPLEMENTATIONS_0[4];


/** 1 args commands **/

/** Which names the user may specify to call the 1 arg built-in commands. */
extern const char *D_COMMANDS_NAMES_1[4];

/** Pointers to the implementations of the 1 arg built-in commands. */
extern const comm_function_1 D_COMMANDS_IMPLEMENTATIONS_1[4];


#endif //D_COMMANDS
