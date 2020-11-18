/**
 * @file synsem.h
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief This is the syntantic and semantic analyzer that delta will use.
 *
 * @details
 *  /* TODO
 */


#ifndef D_SYNTACTIC_SEMANTIC_ANALYZER
#define D_SYNTACTIC_SEMANTIC_ANALYZER


#include "common/symbol_table.h"
#include "analyzers/lexical.h"


/**
 * @brief Initializes the syntactic analyzer.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_syntactic_analyzer_initialize();


/**
 * @brief The syntactic analyzer parses a certain source file.
 * 
 * @details
 *  The syntactic analyzer requests to the lexical analyzer to report all
 *  lexical components that are present in the input file, while showing them
 *  through the standard output.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_syntactic_analyzer_parse();


/**
 * @brief Destroys the syntactic analyzer.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_syntactic_analyzer_destroy();


#endif// D_SYNTACTIC_SEMANTIC_ANALYZER
