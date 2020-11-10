/**
 * @file syntactic.h
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief This is the syntantic analyzer that the delta compiler will use.
 *
 * @details
 *  As of now, the syntactic compiler's only duty is to command the lexical
 *  analyzer to parse the input file, reporting all lexical components that
 *  can be found.
 */


#ifndef D_SYNTACTIC_ANALYZER
#define D_SYNTACTIC_ANALYZER


#include "common/symbol_table.h"
#include "analyzers/lexical.h"


/**
 * @brief Represents a syntactic analyzer.
 *
 * @details
 *  Opaque data type which represents a syntactic analyzer.
 */
struct d_syntactic_analyzer;


/**
 * @brief Globally accessible syntactic analyzer that will be used.
 */
struct d_syntactic_analyzer *syntactic_analyzer;


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


#endif
