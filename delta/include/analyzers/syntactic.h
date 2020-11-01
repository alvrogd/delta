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


#include "io/io_system.h"
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
 * @brief Initializes a syntactic analyzer.
 *
 * @param[out] syntactic_analyzer Reference to the pointer where the address
 *                                of the new syntactic analyzer will be
 *                                stored.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_syntactic_analyzer_initialize(
    struct d_syntactic_analyzer **syntactic_analyzer
);


/**
 * @brief The syntactic analyzer prepares to parse a certain source file.
 * 
 * @details
 *  The syntactic analyzer is pointed which I/O system, symbol table and
 *  lexical analyzer to use in order to be able to parse a source file.
 *
 * @param[in,out] syntactic_analyzer The syntactic analyzer.
 * @param[in] io_system The I/O system.
 * @param[in] symbol_table The symbol table.
 * @param[in] lexical_analyzer The lexical analyzer.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_syntactic_analyzer_prepare_for_parsing(
    struct d_syntactic_analyzer *syntactic_analyzer,
    struct d_io_system *io_system,
    struct d_symbol_table *symbol_table,
    struct d_lexical_analyzer *lexical_analyzer
);


/**
 * @brief The syntactic analyzer parses a certain source file.
 * 
 * @details
 *  The syntactic analyzer requests to the lexical analyzer to report all
 *  lexical components that are present in the input file, while showing them
 *  through the standard output.
 *
 * @param[in,out] syntactic_analyzer The syntactic analyzer.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_syntactic_analyzer_parse(
    struct d_syntactic_analyzer *syntactic_analyzer
);


/**
 * @brief Destroys a syntactic analyzer.
 *
 * @param[out] syntactic_analyzer Reference to the pointer where the address
 *                                of the syntactic analyzer can be found.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_syntactic_analyzer_destroy(
    struct d_syntactic_analyzer **syntactic_analyzer
);


#endif
