/**
 * @file lexical.h
 * @author Álvaro Goldar Dieste
 * @date Oct 2020
 *
 * @brief This is the lexical analyzer that the delta compiler will use.
 *
 * @details
 *  This is the definition of the lexical analyzer that the delta compiler use
 *  during through the compilation process.
 *
 *  Its duty is to parse the contents given by the I/O system, recognizing
 *  the lexical components that are present in it. These are defined in
 *  lexical_components.h
 *
 *  For some lexical components, it will be enough to just identify them
 *  through those lex. comp. definitions (i.e. an "if" has the same meaning
 *  wherever it may appear), whereas others, such as identifiers and literals,
 *  will be returned along with some additional attributes:
 *
 *    - Both identifiers and literals will carry the lexemes (characters) that
 *      represent them in order to not lose their specific values.
 *    
 *    - Furthermore, identifiers have many additional metadata, such as their
 *      data type, first appearance... That is why they will also point to
 *      their corresponding symbol table's entries, which will already contain
 *      all that metadata, instead of replicating it.
 *
 *  Thus, a recognized lexical component will be presented as a struct
 *  d_lexical_component, which contains:
 *
 *    - The integer that represents the lexical component's category.
 *
 *    - A void * pointer to its attributes.
 *        - If the component is a literal, it is a char * to its lexeme.
 *        - If the component is an identifier, it is a
 *          struct d_symbol_table_entry *, which already contains the lexeme
 *          (see common/symbol_table.h).
 */


#ifndef D_LEXICAL_ANALYZER
#define D_LEXICAL_ANALYZER


#include "io/io_system.h"
#include "common/symbol_table.h"



/**
 * @brief Represents a lexical analyzer.
 *
 * @details
 *  Opaque data type which represents a lexical analyzer.
 */
struct d_lexical_analyzer;


/**
 * @brief Represents a lexical component.
 *
 * @details
 *  Opaque data type which represents a lexical analyzer.
 */
struct d_lexical_component {
    /** Integer that represents the (sub)category to which the lexical
        component belongs. */
    int category;
    /** Any attributes that the lexical component may carry along:
          
          - char * to its lexeme if the component is a literal.
              (must be manually freed once de component is no longer needed)
              
          - struct d_symbol_table_entry * if the component is an identifier.
              (there is no need to free it as the symbol table is responsible
               of it)
    */
    void *attributes;
};


/**
 * @brief Initializes a lexical analyzer.
 *
 * @param[out] lexical_analyzer Reference to the pointer where the address of
 *                              the new lexical analyzer will be stored.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_lexical_analyzer_initialize(
    struct d_lexical_analyzer **lexical_analyzer
);


/**
 * @brief The lexical analyzer prepares to parse a certain source file.
 * 
 * @details
 *  The lexical analyzer is pointed which I/O system and symbol table to use
 *  in order to be able to parse a source file.
 *
 * @param[in,out] lexical_analyzer The lexical analyzer.
 * @param[in] io_system The I/O system.
 * @param[in] symbol_table The symbol table.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_lexical_analyzer_prepare_for_parsing(
    struct d_lexical_analyzer *lexical_analyzer,
    struct d_io_system *io_system,
    struct d_symbol_table *symbol_table
);


/**
 * @brief The lexical analyzer identifies the next lexical component in the
 *        current source file.
 *
 * @details
 *  The lexical analyzer continues parsing the current source file,
 *  recognizing and returning its next lexical component.
 *
 *  If the component turns out to be an identifier, the analyzer will also
 *  take care of initializing its corresponding entry in the symbol table.
 * 
 * @param[in,out] lexical_analyzer The lexical analyzer.
 * @param[out] lexical_analyzer Pointer to a struct d_lexical_analyzer to
 *                              which the lexical component's data will be
 *                              written.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_lexical_analyzer_get_next_lexical_comp(
    struct d_lexical_analyzer *lexical_analyzer,
    struct d_lexical_component *lexical_component
);


/**
 * @brief Destroys a lexical analyzer.
 *
 * @param[out] lexical_analyzer Reference to the pointer where the address of
 *                              the symbol table can be found.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_lexical_analyzer_destroy(
    struct d_lexical_analyzer **lexical_analyzer
);


#endif //D_LEXICAL_ANALYZER
