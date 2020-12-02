/**
 * @file help.h
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief This file contains the utilities on which delta relies to show its
 *        help.
 *
 * @details
 *  
 */


#ifndef D_HELP
#define D_HELP


/* Category: intro */
#define D_HELP_INTRO 00

/* Category: features */
#define D_HELP_FEATURES 10
#define D_HELP_FEATURES_DATA_TYPES 11
#define D_HELP_FEATURES_BASIC_OPERATIONS 12
#define D_HELP_FEATURES_BASIC_MATH_FUNCTIONS 13
#define D_HELP_FEATURES_VARIABLES_AND_CONSTANTS 14
#define D_HELP_FEATURES_WORKSPACE 15
#define D_HELP_FEATURES_LOAD_MATH_FUNCTIONS 16
#define D_HELP_FEATURES_NOTEBOOKS 17
#define D_HELP_FEATURES_ECHO 18
#define D_HELP_FEATURES_HELP 20
#define D_HELP_FEATURES_QUIT 19


/**
 * @brief Shows help about the specified topic.
 * 
 * @param topic The topic.
 */
void d_help_show_topic_help(
    int topic
);


#endif //D_HELP
