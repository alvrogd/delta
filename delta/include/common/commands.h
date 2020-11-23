/**
 * @file commands.h
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief This file lists a subset of the keywords in the D language.
 *
 * @details
 *  This file lists all the keywords in the D language that can be found in
 *  regression.d  
 */

/* TODO update */

#ifndef D_COMMANDS
#define D_COMMANDS


/** Which numeric code the "quit" command will return in order to point out
    that the program must end its execution .*/
#define D_COMMAND_QUIT_REQUEST 160


/** Templates for the functions that implement the commands. */
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
    /** Pointer to which the command's implementation resides. */
    comm_function implementation;

    /** How many arguments the command receives. As of now, a command may
        receive one argument at most; more specifically, a string. */
    int arg_count;
};


/* 0 args commands */
extern const char *D_COMMANDS_NAMES_0[4];
extern const comm_function_0 D_COMMANDS_IMPLEMENTATIONS_0[4];

/* 1 args commands */
extern const char *D_COMMANDS_NAMES_1[4];
extern const comm_function_1 D_COMMANDS_IMPLEMENTATIONS_1[4];


#endif //D_COMMANDS
