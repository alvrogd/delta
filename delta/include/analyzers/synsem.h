/**
 * @file synsem.h
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief This is the syntantic and semantic analyzer that delta will use.
 *
 * @details
 *  This is de definition of the syntactic & semantic analyzer that delta will
 *  use through the interpretation process. The analyzer relies on bison, a
 *  tool that generates parsers that convert an annotated context-free grammar
 *  into a deterministic LR or generalized LR (GLR) parser employing LALR(1)
 *  parser tables.
 *
 *  Its duty is to parse the lexical components that are recognized by the
 *  lexical analyzer, in order to recognize valid sentences of delta's
 *  language that may be executed. As the lexical analyzer is implemented
 *  using flex, the integration between these analyzers will follow standard
 *  procedures covered in many online resources (there is an example below).
 *
 *  TODO which sentences are valid?
 *
 * @see https://www.gnu.org/software/bison/
 * @see flex & bison by John Levine, 1st edition, chapter 2.5
 */


#ifndef D_SYNTACTIC_SEMANTIC_ANALYZER
#define D_SYNTACTIC_SEMANTIC_ANALYZER


#include "analyzers/lexical.h"
#include "common/symbol_table.h"


/**
 * @brief Flag that tells the syntactic & semantic analyzer if it must request
 *        the lexical analyzer to read another file.
 *
 * @details
 *  Flag that tells the syntactic & semantic analyzer if it must request the
 *  lexical analyzer to read another file. Depending on its value:
 *
 *    - 0: no other file needs to be read.
 *    - Otherwise: after reading the whole current line, the request will be
 *                 issued to the lexical analyzer and the flag's value will be
 *                 reset.
 */
extern int d_synsem_load_file;

/**
 * @brief Path of the other file that the lexical analyzer must read, if any.
 */
extern const char *d_synsem_load_file_path;


/**
 * @brief Initializes the syntactic analyzer.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_synsem_analyzer_initialize(
    void
);


/**
 * @brief The syntactic analyzer starts the parsing process.
 * 
 * @details
 *  The syntactic analyzer requests to the lexical analyzer to report all
 *  lexical components that it can find in the user-given input. This process
 *  will just continue as long as no "quit" request is issued by the user.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_synsem_analyzer_parse(
    void
);


/**
 * @brief Destroys the syntactic analyzer.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_synsem_analyzer_destroy(
    void
);


#endif //D_SYNTACTIC_SEMANTIC_ANALYZER
