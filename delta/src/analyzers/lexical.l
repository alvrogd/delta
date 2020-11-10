/**
 * @file lexical.l
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
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
 *  As you may see, this analyzer relies on flex, a tool for generating
 *  programs that perform pattern-matching on text.
 *
 * @see https://github.com/westes/flex
 */


/* ══════════════════════════ Compilation Options ═════════════════════════ */

/** Custom flex-generated files. */
%option outfile="lexical.c"

/** 1-byte chars. */
%option 8bit

/** The current lexeme will be a character pointer (= no max. length). */
%option pointer

/** We will just parse a single source file. */
%option noyywrap


/* ═════════════════════════ Exclusive start states ═══════════════════════ */

/** In order to easily parse comments. */
%x COMMENT_BLOCK
%x COMMENT_NESTED


/* ═══════════════════════════════ Definitions ════════════════════════════ */

PRINT   [ - ] 

DIGIT    [0-9]
NON_ZERO [1-9]

FLOAT_INTEGER  {DIGIT}({DIGIT}|_)*
FLOAT_DECIMALS {DIGIT}({DIGIT}|_)*
FLOAT_EXPONENT (e|E)(\+|\-)?{DIGIT}({DIGIT}|_)*

ALPHA [a-zA-z]


/* ═══════════════════════ Includes & local variables ═════════════════════ */

%{
    #include "common/errors.h"
    #include "common/lexical_components.h"
    #include "common/symbol_table.h"

    /* File access */
    #include <stdio.h>
    /* To store lexemes */
    #include <string.h>
    /* size_t */
    #include <stddef.h>


    /** Block size in bytes. */
    /* TODO As of now, the block size has been set to 4096 bytes as the
            compiler is intended to be used in Linux, in which the most
            popular filesystem is ext4, whose default block size is 4096
            bytes. However, it would be nice to automatically detect the block
            size of whichever filesystem lies under.
    */
    #define YY_BUF_SIZE 4096


    /** How many '\n' have been seen in the input file that is being
        analyzed. */
    size_t current_line;

    /** How many characters have been seen in the current line */
    size_t current_column;

    /** Because a standard finite automata will not be able to detect nested
        comments */
    size_t active_nested_comments;

    /** To store any possible attributes that a lexical component may have. */
    const void *lex_component_attributes;
%}


%%


 /* ════════════════════════════════ Rules ═══════════════════════════════ */

 /* ** 1-char components ** */

"-"   { return D_LC_OP_ARITHMETIC_MINUS; }
"*"   { return D_LC_OP_ARITHMETIC_TIMES; }
"/"   { return D_LC_OP_ARITHMETIC_DIV; }
"<"   { return D_LC_OP_RELATIONAL_LESS_THAN; }
"="   { return D_LC_OP_ASSIGNMENT_ASSIGN; }
"."   { return D_LC_OP_ACCESS_DOT; }
"["   { return D_LC_OP_ACCESS_L_BRACKET; }
"]"   { return D_LC_OP_ACCESS_R_BRACKET; }
","   { return D_LC_SEPARATOR_COMMA; }
";"   { return D_LC_SEPARATOR_SEMICOL; }
"("   { return D_LC_SEPARATOR_L_PARENTHESIS; }
")"   { return D_LC_SEPARATOR_R_PARENTHESIS; }
"{"   { return D_LC_SEPARATOR_L_CURLY; }
"}"   { return D_LC_SEPARATOR_R_CURLY; }


 /* ** 2-char components ** */

"++"    { return D_LC_OP_ARITHMETIC_INCREMENT; }
"=="    { return D_LC_OP_RELATIONAL_EQUALS; }
"+="    { return D_LC_OP_ASSIGNMENT_PLUS_ASSIGN; }


 /* ** Multiple-char components ** */

 /* Comments, block */

"/*"                      { BEGIN(COMMENT_BLOCK); }

<COMMENT_BLOCK>"*/"       { BEGIN(INITIAL); }

<COMMENT_BLOCK>[^*\n]+    /* Ignore anything that is not "*" nor '\n' */

<COMMENT_BLOCK>"*"        /* Ignore '*' */

<COMMENT_BLOCK>\n         { ++current_line; current_column = 1; }

<COMMENT_BLOCK><<EOF>>    { d_errors_parse_show(3,
                                                D_ERR_LEX_WHITESPACE_COMMENT_EOF,
                                                current_line, current_column);
                            return D_LC_EOF; }


 /* Comments, block & nested */

"/+"                    { ++active_nested_comments; 
                          BEGIN(COMMENT_NESTED); }

<COMMENT_NESTED>"+/"    { --active_nested_comments;
                          if(active_nested_comments == 0) BEGIN(INITIAL); }

<COMMENT_NESTED>[^+\n]+ /* Ignore anything that is not "+\/" nor '\n' */

<COMMENT_NESTED>"+"      /* Ignore '+' */

<COMMENT_NESTED>\n       { ++current_line; current_column = 1; }

<COMMENT_NESTED><<EOF>> { d_errors_parse_show(3,
                                              D_ERR_LEX_WHITESPACE_COMMENT_EOF,
                                              current_line, current_column);
                          return D_LC_EOF; }


 /* Comments (single line) */

"//".*\n    { ++current_line; current_column = 1; }


 /* Double quoted strings */

\"\"    { d_errors_parse_show(3, D_ERR_LEX_LITERALS_STRING_EMPTY, current_line,
                              current_column); }

\"({PRINT}|\\[abfnrtvx])+\"  { _d_lexical_analyzer_literal_recognized();
                              return D_LC_LITERAL_STR; }


 /* Identifiers */

({ALPHA}|_)({DIGIT}|{ALPHA}|_)*   { return _d_lexical_analyzer_identifer_recognized(); }


 /* Numbers, binary integer */

0[bB](_)*[01][01_]*                                     { _d_lexical_analyzer_literal_recognized();
                                                          return D_LC_LITERAL_INT; }

0[bB](_)*[^01]({DIGIT}|{ALPHA}|_)*                      { d_errors_parse_show(4,
                                                          D_ERR_LEX_LITERALS_INTEGERS_BINARY_BAD,
                                                          current_line, current_column,
                                                          "a binary number needs at least one "
                                                          "'0' -OR- '1' (i.e. 0b1)"); }

0[bB](_)*[01][01_]*([2-9]|{ALPHA})({DIGIT}|{ALPHA}|_)*  { d_errors_parse_show(4,
                                                          D_ERR_LEX_LITERALS_INTEGERS_BINARY_BAD,
                                                          current_line, current_column,
                                                          "a binary number may only have '0' "
                                                          "-AND- '1' -AND- '_'"); }


 /* Numbers, decimal integer */

{NON_ZERO}({DIGIT}|_)*    { _d_lexical_analyzer_literal_recognized();
                                  return D_LC_LITERAL_INT; }

{NON_ZERO}({DIGIT}|_)*({ALPHA}--[bBeE])({DIGIT}|{ALPHA}|_)*    { d_errors_parse_show(4,
                                                                  D_ERR_LEX_LITERALS_INTEGERS_BAD,
                                                                  current_line, current_column,
                                                                  "the only letters that may "
                                                                  "follow after an integer "
                                                                  "number are 'E' -OR- 'e' for "
                                                                  "floats (i.e. 10E+15) -AND- "
                                                                  "'B' -OR- 'b'  for binaries "
                                                                  "(i.e. 0b011)"); }


 /* Numbers, decimal float */

{FLOAT_INTEGER}\.({FLOAT_DECIMALS}({FLOAT_EXPONENT})?)?                                                                 { _d_lexical_analyzer_literal_recognized(); 
                                                                                                                          return D_LC_LITERAL_FP; }

\.{FLOAT_DECIMALS}({FLOAT_EXPONENT})?                                                                                   { _d_lexical_analyzer_literal_recognized(); 
                                                                                                                          return D_LC_LITERAL_FP; }

{FLOAT_INTEGER}{FLOAT_EXPONENT}                                                                                         { _d_lexical_analyzer_literal_recognized(); 
                                                                                                                          return D_LC_LITERAL_FP; }

{FLOAT_INTEGER}\.{ALPHA}({DIGIT}|{ALPHA}|_)*                                                                            { d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                                                                                                          current_line, current_column,
                                                                                                                          "the only letter that may follow after a float's "
                                                                                                                          "decimal dot is 'E' -OR- 'e', providing that there "
                                                                                                                          "is at least a digit beforehand (i.e. 10.3E+15)"); }

({FLOAT_INTEGER})?\.{FLOAT_DECIMALS}({ALPHA}--[eE])({DIGIT}|{ALPHA}|_)*                                                { d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                                                                                                          current_line, current_column,
                                                                                                                          "the only letter that may be in a float's decimal "
                                                                                                                          "part is 'E' -OR- 'e' for its exponent (i.e. "
                                                                                                                          "10.3E+15)"); }

((({FLOAT_INTEGER})?\.{FLOAT_DECIMALS})|{FLOAT_INTEGER})[eE][^0-9+-]({DIGIT}|{ALPHA}|_)*                              { d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                                                                                                          current_line, current_column,
                                                                                                                          "the only characters that may follow after a "
                                                                                                                          "float's 'E' -OR- 'e', are '+' -OR- '-' -OR- 'a "
                                                                                                                          "digit' (i.e. 10.3E+15, 10.3E10)"); }

((({FLOAT_INTEGER})?\.{FLOAT_DECIMALS})|{FLOAT_INTEGER})(e|E)(\+|\-)?[^0-9]({DIGIT}|{ALPHA}|_)*                       { d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                                                                                                          current_line, current_column,
                                                                                                                            "a float's decimal number may only have 'digits' "
                                                                                                                            "-AND- '_'"); }

((({FLOAT_INTEGER})?\.{FLOAT_DECIMALS})|{FLOAT_INTEGER})(e|E)(\+|\-)?{DIGIT}({DIGIT}|_)*[^0-9_]({DIGIT}|{ALPHA}|_)*   { d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                                                                                                          current_line, current_column,
                                                                                                                          "a float's decimal number may only have 'digits' "
                                                                                                                          "-AND- '_'"); }


 /* ** Whitespace ** */

[ \t]+  /* Ignore */
\n      { ++current_line; current_column = 1; }


 /* ** EOF ** */

<<EOF>>     { return D_LC_EOF; }


%%


/* ════════════════════════════════ Functions ═════════════════════════════ */

/**
 * @brief Initializes the lexical analyzer.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_lexical_analyzer_initialize()
{
    // No initializations are needed as of now

    return 0;
}


/**
 * @brief The lexical analyzer prepares to parse a certain source file.
 * 
 * @details
 *  The lexical analyzer is pointed which source file to use while parsing.
 *
 * @param[in] file_path Path (relative or absolute) to the desired file.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_lexical_analyzer_prepare_for_parsing(
    const char *file_path
)
{
    if(file_path == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "lexical.l",
                               "d_lexical_analyzer_prepare_for_parsing",
                               "'file_path'");
        return -1;
    }


    if ((yyin = fopen(file_path, "r")) == NULL) {

        d_errors_internal_show(3, D_ERR_USER_INPUT_FILE_INACCESSIBLE,
                               "lexical.l",
                               "d_lexical_analyzer_prepare_for_parsing");
        return -1;
    }


    // The counters need to be initialized
    current_line = 1;
    current_column = 1;
    active_nested_comments = 0;

    lex_component_attributes = NULL;


    return 0;
}


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
 * @param[out] lexical_component Pointer to a struct d_lexical_analyzer to
 *                               which the lexical component's data will be
 *                               written.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_lexical_analyzer_get_next_lexical_comp(
    struct d_lexical_component *lexical_component
)
{
    if(lexical_component == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "lexical.l",
                               "d_lexical_analyzer_get_next_lexical_comp",
                               "'lexical_component'");
        return -1;
    }


    // Flex takes care of recognizing the lexical component
    lexical_component->category = yylex();

    // Any associated attributes will have been stored while parsing
    if(lex_component_attributes != NULL) {

        lexical_component->attributes = lex_component_attributes;
        lex_component_attributes = NULL;
    }


    return 0;
}


/**
 * @brief Stores the lexeme that represents the current lexical component.
 *
 * @details
 *  Stores the lexeme (value) that represents the current lexical component.
 *  Specifically, it gets copied to a new string, which will be available in
 *  the "lex_component_attributes" pointer.
 *
 * @return 0 if successful, any other value otherwise.
 */
int _d_lexical_analyzer_literal_recognized()
{
    const unsigned char *lexeme = NULL;
    
    
    if((lexeme = (const unsigned char *) strdup(yytext)) == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                               "lexical.l",
                               "_d_lexical_analyzer_literal_recognized",
                               "'strdup' of current lexeme");
        return -1;
    }

    #ifdef D_DEBUG
    printf("[lexical_analyzer][literal recognized] Saved lexeme: %s\n",
           lexeme);
    #endif

    lex_component_attributes = lexeme;

    
    return 0;
}


/**
 * @brief Stores the current lexical component in the symbol table.
 *
 * @details
 *  Generates a new entry in the symbol table for the current lexical
 *  component. Specifically, this entry will have the component's lexeme, and
 *  it will be available in the "lex_component_attributes" pointer.
 *
 *  It is worth noting that the identifier may actually be a keyword. If it
 *  were the case, its already present entry in the table will be retrieved,
 *  instead of creating a new one, and it will be also available in the
 *  "lex_component_attributes" pointer.
 *
 * @return D_LC_IDENTIFIER if successful, D_LC_IDENTIFIER if the component
 *         turned out to be a keyword, any other value otherwise.
 */
int _d_lexical_analyzer_identifer_recognized()
{
    const unsigned char *lexeme = NULL;
    struct d_symbol_table_entry entry;
    struct d_symbol_table_entry *entry_in_table = NULL;
    
    
    // Saving the lexeme
    if((lexeme = (const unsigned char *) strdup(yytext)) == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                               "lexical.l",
                               "_d_lexical_analyzer_literal_recognized",
                               "'strdup' of current lexeme");
        return -1;
    }

    #ifdef D_DEBUG
    printf("[lexical_analyzer][identifier recognized] Saved lexeme: %s\n",
           lexeme);
    #endif


    // The symbol table requires the entry to be initialized before adding it

    #ifdef D_DEBUG
    printf("[lexical_analyzer][identifier recognized] Symbol table entry "
           "key: %s\tKey length: %zu\n", lexeme,
           strlen((const char *)lexeme));
    #endif

    // In order to do so, let's check first if a corresponding entry is
    // already present
    entry_in_table = d_symbol_table_search(lexeme);

    if(entry_in_table == NULL) {

        #ifdef D_DEBUG
        printf("[lexical_analyzer][identifier recognized] Key not present "
               "yet\n");
        #endif

        // If it is not already present, it definitely cannot be a
        // keyword
        entry.lexeme = lexeme;
        entry.lexical_component = D_LC_IDENTIFIER;

        d_symbol_table_add(&entry);
        entry_in_table = d_symbol_table_search(lexeme);
    }

    // Anyways, if the component deserves an entry in the symbol
    // table, its attribute will always be a reference to that entry
    lex_component_attributes = entry_in_table;

    
    return entry_in_table->lexical_component;
}


/**
 * @brief Shows the specified lexical component.
 *
 * @details
 *  Prints out which category the given lexical component belongs to, as well
 *  as its lexeme, if any (for keywords and identifiers).
 * 
 * @param[in] lexical_component The lexical component.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_lexical_analyzer_show_lexical_comp(
    struct d_lexical_component *lexical_component
)
{
    if(lexical_component == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL, "lexical.l",
                               "d_lexical_analyzer_show_lexical_comp",
                               "'lexical_component'");
        return -1;
    }


    printf("<%s", d_lc_to_string(lexical_component->category));


    // The lexeme will be stored as its attribute, if any
    if(lexical_component->attributes != NULL) {

        // If its attribute is its lexeme
        if(lexical_component->category / D_LC_DISTANCE_CATEGORY ==
           D_LC_LITERAL / D_LC_DISTANCE_CATEGORY) {

            printf(", %s", (const char *) lexical_component->attributes);
        }

        // If its attribute is a pointer to its entry in the symbol table
        else if(lexical_component->category / D_LC_DISTANCE_CATEGORY ==
                    D_LC_KEYWORD / D_LC_DISTANCE_CATEGORY ||
                
                lexical_component->category / D_LC_DISTANCE_CATEGORY ==
                    D_LC_IDENTIFIER / D_LC_DISTANCE_CATEGORY) {

            printf(", %s",
                ((struct d_symbol_table_entry *)
                    lexical_component->attributes)->lexeme);
        }
    }


    printf(">\n");


    return 0;
}


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
)
{
    if(lexical_component == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL, "lexical.l",
                               "d_lexical_analyzer_destroy_lexical_com",
                               "'lexical_component'");
        return -1;
    }


    if(lexical_component->attributes != NULL) {

        // If the component is a literal, its attribute is its lexeme
        //   -> must be freed by the lex. analyzer
        if(lexical_component->category / D_LC_DISTANCE_CATEGORY ==
           D_LC_LITERAL / D_LC_DISTANCE_CATEGORY) {

            free((void *) lexical_component->attributes);
        }

        // Otherwise, its attribute must be a pointer to its entry in the
        // symbol table
        //   -> the symbol table will take care of it
    }


    return 0;
}


/**
 * @brief Destroys the lexical analyzer.
 *
 * @return 0 if successful, any other value otherwise.
 */
int d_lexical_analyzer_destroy()
{
    if (fclose(yyin) != 0) {

        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                               "lexical.l", "d_lexical_analyzer_destroy",
                               "'fclose' on input source file");
        return -1;
    }
    
    
    return 0;
}
