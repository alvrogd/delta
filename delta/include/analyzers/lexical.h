/**
 * @file lexical.h
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief This is the lexical analyzer that delta will use.
 *
 * @details
 *  This is the definition of the lexical analyzer that delta will use 
 *  through the interpretation process. This analyzer relies on flex, a tool
 *  for generating programs that perform pattern-matching on text, which
 *  already includes its own I/O system.
 *
 *  Its duty is to read the contents given by the user, recognizing the
 *  lexical components that are present in it. The standard input source will
 *  be stdin, but the user can order the lexical analyzer to read a certain
 *  file at anytime.
 *
 *  As the lexical & semantic is implemented using Bison, the flex-bison
 *  interaction means that those lexical components will be defined in another
 *  bison-generated file, "lexical_components.h".
 *
 *  For some lexical components, it will be enough to just identify them
 *  through those lex. comp. definitions (i.e. a "==" has the same meaning
 *  wherever it may appear), whereas others, such as identifiers and literals,
 *  will be returned along with some additional attributes, as each specific
 *  lexeme in this categories has a different meaning:
 *
 *    - Identifiers will carry pointers to their entries in the symbol table.
 *      Identifiers may correspond to variables, math functions/constants, and
 *      built-in commands.
 *    
 *    - Base 10 numbers will carry a "struct d_dec_number" that represent
 *      them.
 *
 *    - Strings will carry a string that represent their quote-limited
 *      characters.
 *
 * @see https://github.com/westes/flex
 */


#ifndef D_LEXICAL_ANALYZER
#define D_LEXICAL_ANALYZER


/* size_t */
#include <stddef.h>


/**
 * @brief Path to the file that is currently being read.
 */
extern const char *d_lex_current_file;

/**
 * @brief If the lexical analyzer is currently reading input from stdin or
 *        not.
 */
extern int d_lex_is_stdin;


/**
 * @brief How many '\n' have been seen in the input file that is being
 *        analyzed.
 */
extern size_t d_lex_current_line;

/**
 * @brief How many characters have been seen in the current line.
 */
extern size_t d_lex_current_column;


/**
 * @brief Represents a lexical component.
 *
 * @details
 *  Opaque data type which represents a lexical component.
 */
struct d_lexical_component {
    /** Integer that represents the (sub)category to which the lexical
        component belongs. */
    int category;
    /** Any attributes that the lexical component may carry along. */
    const void *attributes;
};


/**
 * @brief Initializes the lexical analyzer.
 *
 * @details
 *  Initializes the lexical analyzer, setting stdin as the default input
 *  source.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_lexical_analyzer_initialize(
    void
);


/**
 * @brief Prepares to read new input file.
 *
 * @details
 *  Prepares the lexical analyzer to read a new input file. Therefore, the
 *  current input file, as well as all of its metadata, must be stored in the
 *  stack for later usage.
 *
 * @param[in] filename Absolute or relative path to the new input file.
 * 
 * @return 0 if successful, any other value otherwise.
 */
int d_lexical_analyzer_new_file(
    const char *filename
);


/**
 * @brief Closes the current input file.
 *
 * @details
 *  Closes the current input file, and restores the file represented by
 *  the currently accessible entry in the stack. Note that a "pop" may
 *  only be issued if a corresponding "new_file" has been called
 *  previously. Otherwise, you may end up trying to close stdin.
 * 
 * @return 0 if successful, any other value otherwise.
 */
int d_lexical_analyzer_pop_file(
    void
);


/**
 * @brief The lexical analyzer identifies the next lexical component in the
 *        current source file.
 *
 * @details
 *  If there is no source file specified, the analyzer will just take data
 *  from stdin by default.
 *
 * @return 0 if successful, any other value otherwise.
 */
int yylex(
    void
);


/**
 * @brief Destroys the lexical analyzer.
 *
 * @details
 *  Destroys the lexical analyzer, while also closing any still opened files.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_lexical_analyzer_destroy(
    void
);


#endif //D_LEXICAL_ANALYZER
