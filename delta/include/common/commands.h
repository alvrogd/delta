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


/** Template for the functions that implement the commands. */
typedef int (*comm_function) (void);


extern const char *D_COMMANDS_NAMES[4];

extern const comm_function D_COMMANDS_IMPLEMENTATIONS[4];


#endif //D_COMMANDS
