/**
 * @file lexical.h
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief This is the lexical analyzer that delta will use.
 *
 * @details
 *  This is the definition of the lexical analyzer that delta will use 
 *  through the compilation process.
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
 *  This analyzer relies on flex, a tool for generating programs that perform
 *  pattern-matching on text, which already includes its own I/O system.
 *
 * @see https://github.com/westes/flex
 */

#ifndef D_LEXICAL_ANALYZER
#define D_LEXICAL_ANALYZER


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
              (must be manually freed once de component is no longer needed,
               using the d_lexical_analyzer_destroy_lexical_comp function)
              
          - struct d_symbol_table_entry * if the component is an identifier.
              (there is no need to free it as the symbol table is responsible
               of it)
    */
    const void *attributes;
};


/**
 * @brief Initializes the lexical analyzer.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_lexical_analyzer_initialize();


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
 * @return 0 if successful, any other value otherwise.
 */
int yylex();


/**
 * @brief The lexical analyzer destroys a lexical component that has
 *        previously created.
 *
 * @details
 *  The lexical analyzer frees any resources that may have allocated
 *  previously for a given lexical component, and for which it must take
 *  responsability, as some of them may carry attributes.
 *
 *  Specifically, according to the different types of lexical components that
 *  have attributes:
 *
 *    - Those that have an entry in the symbol table are already memory-
 *      -managed by it. That is:
 *
 *        - If actual scopes were supported, closing one would imply deleting
 *          all of its identifiers. They must not be deleted earlier, as an
 *          identifier can be referenced multiple times in the same scope. As
 *          of now, the "only scope" would be terminated when destroying the
 *          symbol table. In such event, all allocated memory to each present
 *          identifier will be freed.
 *
 *        - Keywords cannot be removed from the table until the input file
 *          parsing has ended. Therefore, they will also be destroyed by the
 *          symbol table in the previous circumstances.
 *
 *    - Regarding the other components that also have attributes, but no
 *      entry, these attributes have been reserved on purpose when recognizing
 *      the lex. comp. from the input file, so they will not be freed unless
 *      the consumer requests it.
 *
 * @param[in,out] lexical_component Pointer to the lexical component that will
 *                                  be freed.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_lexical_analyzer_destroy_lexical_com(
    struct d_lexical_component *lexical_component
);


/**
 * @brief Destroys the lexical analyzer.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_lexical_analyzer_destroy();


#endif // D_LEXICAL_ANALYZER
