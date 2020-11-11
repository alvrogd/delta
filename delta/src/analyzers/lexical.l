/**
 * @file lexical.l
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief Implementation of analyzers/syntactic.h
 */


/* ══════════════════════════ Compilation Options ═════════════════════════ */

/** 1-byte chars. */
%option 8bit

/** We will just parse a single source file. */
%option noyywrap

/** The current lexeme will be a character pointer (= no max. length). */
%option pointer


/* ═════════════════════════ Exclusive start states ═══════════════════════ */

/** In order to easily parse comments. */
%x COMMENT_BLOCK
%x COMMENT_NESTED


/* ═══════════════════════════════ Definitions ════════════════════════════ */

ALPHA   [a-zA-Z]

DIGIT   [0-9]

FLOAT_INTEGER   {DIGIT}({DIGIT}|_)*
FLOAT_DECIMALS  {DIGIT}({DIGIT}|_)*
FLOAT_EXPONENT  (e|E)(\+|\-)?{DIGIT}({DIGIT}|_)*

PRINT   [ -²]


/* ═════════════════ Includes & local functions/variables ═════════════════ */

%{
    #include "analyzers/lexical.h"

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


    /* Declaration of some private fuctions that will be used in the rules
       section */
    int _d_lexical_analyzer_identifer_recognized(
        struct d_lexical_component *lexical_component
    );

    int _d_lexical_analyzer_literal_recognized(
        struct d_lexical_component *lexical_component
    );

    int _d_lexical_analyzer_literal_count_eol(
        struct d_lexical_component *lexical_component
    );


    /** How many '\n' have been seen in the input file that is being
        analyzed. */
    size_t current_line;

    /** How many characters have been seen in the current line */
    size_t current_column;

    /** Each time that a lexical component is recognized, its length will be
        added to the current col count. */
    #define YY_USER_ACTION { current_column += yyleng; }


    /** Because a standard finite automata will not be able to detect nested
        comments */
    size_t active_nested_comments;


    /** The default yylex() function is overriden as the lexical analyzer
        needs to be able to receive custom parameters. */
    #define YY_DECL int d_lexical_analyzer_get_next_lexical_comp(struct d_lexical_component *lexical_component)
%}


%%


 /* ════════════════════════════════ Rules ═══════════════════════════════ */

 /* ** 1-char components ** */

"-"   { lexical_component->category = D_LC_OP_ARITHMETIC_MINUS; return 0; }
"*"   { lexical_component->category = D_LC_OP_ARITHMETIC_TIMES; return 0; }
"/"   { lexical_component->category = D_LC_OP_ARITHMETIC_DIV; return 0; }
"<"   { lexical_component->category = D_LC_OP_RELATIONAL_LESS_THAN; return 0; }
"="   { lexical_component->category = D_LC_OP_ASSIGNMENT_ASSIGN; return 0; }
"."   { lexical_component->category = D_LC_OP_ACCESS_DOT; return 0; }
"["   { lexical_component->category = D_LC_OP_ACCESS_L_BRACKET; return 0; }
"]"   { lexical_component->category = D_LC_OP_ACCESS_R_BRACKET; return 0; }
","   { lexical_component->category = D_LC_SEPARATOR_COMMA; return 0; }
";"   { lexical_component->category = D_LC_SEPARATOR_SEMICOL; return 0; }
"("   { lexical_component->category = D_LC_SEPARATOR_L_PARENTHESIS; return 0; }
")"   { lexical_component->category = D_LC_SEPARATOR_R_PARENTHESIS; return 0; }
"{"   { lexical_component->category = D_LC_SEPARATOR_L_CURLY; return 0; }
"}"   { lexical_component->category = D_LC_SEPARATOR_R_CURLY; return 0; }


 /* ** 2-char components ** */

"++"    { lexical_component->category = D_LC_OP_ARITHMETIC_INCREMENT; return 0; }
"=="    { lexical_component->category = D_LC_OP_RELATIONAL_EQUALS; return 0; }
"+="    { lexical_component->category = D_LC_OP_ASSIGNMENT_PLUS_ASSIGN; return 0; }


 /* ** Multiple-char components ** */

 /* Comments, block */

"/*"                            { BEGIN(COMMENT_BLOCK); }

<COMMENT_BLOCK>[^*\n]           /* Ignore anything that is not '*' nor '\n' */

<COMMENT_BLOCK>"*"+[^*/\n]*     /* Ignore '*'s that are not followed by '/'s */

<COMMENT_BLOCK>\n               { ++current_line; current_column = 1; }

<COMMENT_BLOCK>"*"+"/"          { BEGIN(INITIAL); }

<COMMENT_BLOCK><<EOF>>          { d_errors_parse_show(3,
                                                      D_ERR_LEX_WHITESPACE_COMMENT_EOF,
                                                      current_line, current_column);
                                                      lexical_component->category = D_LC_EOF;
                                                      return 0; }


 /* Comments, block & nested */

<INITIAL,COMMENT_NESTED>"/+"    { ++active_nested_comments; 
                                  BEGIN(COMMENT_NESTED); }

<COMMENT_NESTED>[^+\n]          /* Ignore anything that is not '+' nor '\n' */

<COMMENT_NESTED>"+"+[^+/\n]*    /* Ignore '+'s that are not followed by '/'s */

<COMMENT_NESTED>\n              { ++current_line; current_column = 1; }

<COMMENT_NESTED>"+"+"/"         { --active_nested_comments;
                                  if(active_nested_comments == 0) BEGIN(INITIAL); }

<COMMENT_NESTED><<EOF>>         { d_errors_parse_show(3,
                                                      D_ERR_LEX_WHITESPACE_COMMENT_EOF,
                                                      current_line, current_column);
                                                      lexical_component->category = D_LC_EOF;
                                                      return 0; }


 /* Comments (single line) */

"//".*\n    { ++current_line; current_column = 1; }


 /* Double quoted strings */

\"\"                                { d_errors_parse_show(3,
                                                          D_ERR_LEX_LITERALS_STRING_EMPTY,
                                                          current_line, current_column); }

\"({PRINT}|\\([abfnrtuUvx])?)+\"    { _d_lexical_analyzer_literal_recognized(lexical_component);
                                      _d_lexical_analyzer_literal_count_eol(lexical_component);
                                      lexical_component->category = D_LC_LITERAL_STR;
                                      return 0; }


 /* Identifiers */

({ALPHA}|_)({DIGIT}|{ALPHA}|_)*     { lexical_component->category = _d_lexical_analyzer_identifer_recognized(lexical_component);
                                      return 0; }



 /* Numbers, binary integer */

0[bB](_)*[01][01_]*                                     { _d_lexical_analyzer_literal_recognized(lexical_component);
                                                          lexical_component->category = D_LC_LITERAL_INT;
                                                          return 0; }

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

{DIGIT}({DIGIT}|_)*                                     { _d_lexical_analyzer_literal_recognized(lexical_component);
                                                          lexical_component->category = D_LC_LITERAL_INT;
                                                          return 0; }

{DIGIT}({DIGIT}|_)*[aAcCdDf-zF-Z]({DIGIT}|{ALPHA}|_)*   { d_errors_parse_show(4,
                                                          D_ERR_LEX_LITERALS_INTEGERS_BAD,
                                                          current_line, current_column,
                                                          "the only letters that may "
                                                          "follow after an integer "
                                                          "number are 'E' -OR- 'e' for "
                                                          "floats (i.e. 10E+15) -AND- "
                                                          "'B' -OR- 'b'  for binaries "
                                                          "(i.e. 0b011)"); }


 /* Numbers, decimal float */

{FLOAT_INTEGER}\.(({FLOAT_DECIMALS})({FLOAT_EXPONENT})?)?                                                               { _d_lexical_analyzer_literal_recognized(lexical_component); 
                                                                                                                          lexical_component->category = D_LC_LITERAL_FP;
                                                                                                                          return 0; }

\.({FLOAT_DECIMALS})({FLOAT_EXPONENT})?                                                                                 { _d_lexical_analyzer_literal_recognized(lexical_component); 
                                                                                                                          lexical_component->category = D_LC_LITERAL_FP;
                                                                                                                          return 0; }

{FLOAT_INTEGER}{FLOAT_EXPONENT}                                                                                         { _d_lexical_analyzer_literal_recognized(lexical_component); 
                                                                                                                          lexical_component->category = D_LC_LITERAL_FP;
                                                                                                                          return 0; }

{FLOAT_INTEGER}\.{ALPHA}({DIGIT}|{ALPHA}|_)*                                                                            { d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                                                                                                          current_line, current_column,
                                                                                                                          "the only letter that may follow after a float's "
                                                                                                                          "decimal dot is 'E' -OR- 'e', providing that there "
                                                                                                                          "is at least a digit beforehand (i.e. 10.3E+15)"); }

({FLOAT_INTEGER})?\.({FLOAT_DECIMALS})([a-dA-Df-zF-Z])({DIGIT}|{ALPHA}|_)*                                              { d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                                                                                                          current_line, current_column,
                                                                                                                          "the only letter that may be in a float's decimal "
                                                                                                                          "part is 'E' -OR- 'e' for its exponent (i.e. "
                                                                                                                          "10.3E+15)"); }


((({FLOAT_INTEGER})?\.({FLOAT_DECIMALS})|({FLOAT_INTEGER})))[eE][^0-9+-]({DIGIT}|{ALPHA}|_)*                            { d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                                                                                                          current_line, current_column,
                                                                                                                          "the only characters that may follow after a "
                                                                                                                          "float's 'E' -OR- 'e', are '+' -OR- '-' -OR- 'a "
                                                                                                                          "digit' (i.e. 10.3E+15, 10.3E10)"); }

((({FLOAT_INTEGER})?\.({FLOAT_DECIMALS})|({FLOAT_INTEGER})))[eE](\+|\-)?[^0-9]({DIGIT}|{ALPHA}|_)*                      { d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                                                                                                          current_line, current_column,
                                                                                                                          "the only characters that may follow after a "
                                                                                                                          "float's exponent and sign declaration are digits "
                                                                                                                          "(i.e. 10.3E+15, 10.3E10_5)"); }

((({FLOAT_INTEGER})?\.({FLOAT_DECIMALS})|({FLOAT_INTEGER})))[eE](\+|\-)?{DIGIT}({DIGIT}|_)*{ALPHA}({DIGIT}|{ALPHA}|_)*  { d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                                                                                                          current_line, current_column,
                                                                                                                          "a float's decimal number may only have 'digits' "
                                                                                                                          "-AND- '_'"); }


 /* ** Whitespace ** */

[ \t]+  /* Ignore */
\n      { ++current_line; current_column = 1; }


 /* ** EOF ** */

<<EOF>>     { lexical_component->category = D_LC_EOF;
              return 0; }


%%


/* ════════════════════════════════ Functions ═════════════════════════════ */

/**
 * @brief Implementation of lexical.h/d_lexical_analyzer_initialize
 */
int d_lexical_analyzer_initialize()
{
    // No initializations are needed as of now

    return 0;
}


/**
 * @brief Implementation of lexical.h/d_lexical_analyzer_prepare_for_parsing
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


    return 0;
}


/**
 * @brief Stores the lexeme that represents the current literal (a lexical
 *        component).
 *
 * @details
 *  Stores the lexeme that represents the current literal (a lexical
 *  component), as its full meaning cannot be represented using only its
 *  category.
 *
 *  Specifically, the lexeme gets copied to a new string, which will be
 *  available in the "lexical_component->attributes" pointer.
 *
 * @param[out] lexical_component Pointer to a struct d_lexical_analyzer to
 *                               which the lexical component's data will be
 *                               written.
 *
 * @return 0 if successful, any other value otherwise.
 */
int _d_lexical_analyzer_literal_recognized(
    struct d_lexical_component *lexical_component
)
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

    lexical_component->attributes = lexeme;

    
    return 0;
}


/**
 * @brief Checks if any '\n's are present in the last stored lexeme.
 *
 * @details
 *  Checks if any '\n's are present in the last stored lexeme. This action is
 *  required when recognizing double quoted strings, as they may include
 *  newlines which must be manually counted in order to properly update the
 *  current line and col counters.
 *
 * @param[out] lexical_component Pointer to a struct d_lexical_analyzer to
 *                               which the lexical component's data will be
 *                               written.
 *
 * @return 0 if successful, any other value otherwise.
 */
int _d_lexical_analyzer_literal_count_eol(
    struct d_lexical_component *lexical_component
)
{
    size_t eol_count = 0;
    
    // Starts right from the beginning of the lexeme
    const char *last_eol = (const char *)lexical_component->attributes;
    const char *tmp = NULL;


    while((tmp = strchr(last_eol, '\n')) != NULL) {
        ++eol_count;
        last_eol = tmp;
    }

    
    current_line += eol_count;

    if(eol_count > 0) {
        current_column = 1;
    }

    // The current line will have has many characters as there are from the
    // last eol to the end of the lexeme
    current_column += yyleng - (last_eol -
                                 (const char *)lexical_component->attributes);


    return 0;
}


/**
 * @brief Stores the current identifier (a lexical component) in the symbol
 *        table.
 *
 * @details
 *  Generates a new entry in the symbol table for the current identifier (a
 *  lexical component). All identifiers need an entry in the symbol table.
 *
 *  Specifically, this entry will have the component's lexeme, and
 *  it will be available in the "lexical_component->attributes" pointer.
 *
 *  It is worth noting that the identifier may actually be a keyword. If it
 *  were the case, its already present entry in the table will be retrieved,
 *  instead of creating a new one, and it will be also available in the
 *  "lexical_component->attributes" pointer.
 *
 * @param[out] lexical_component Pointer to a struct d_lexical_analyzer to
 *                               which the lexical component's data will be
 *                               written.
 *
 * @return D_LC_IDENTIFIER if successful, D_LC_IDENTIFIER if the component
 *         turned out to be a keyword, any other value otherwise.
 */
int _d_lexical_analyzer_identifer_recognized(
    struct d_lexical_component *lexical_component
)
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
    lexical_component->attributes = entry_in_table;

    
    return entry_in_table->lexical_component;
}


/**
 * @brief Implementation of lexical.h/d_lexical_analyzer_show_lexical_comp
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
 * @brief Implementation of lexical.h/d_lexical_analyzer_destroy_lexical_com
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


    lexical_component->category = -1; // Gets invalidated


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


        lexical_component->attributes = NULL;  // Gets invalidated
    }


    return 0;
}


/**
 * @brief Implementation of lexical.h/d_lexical_analyzer_destroy
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
