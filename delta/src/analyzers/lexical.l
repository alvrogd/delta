/**
 * @file lexical.l
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief Implementation of analyzers/lexical.h
 */


/* ══════════════════════════ Compilation Options ═════════════════════════ */

/** 1-byte chars. */
%option 8bit

/** EOFs will be manually managed in the rules section. */
%option noyywrap

/** The current lexeme will be a character pointer (=> no max. length). */
%option pointer


/* ═══════════════════════════════ Definitions ════════════════════════════ */

ALPHA   [a-zA-Z]

DIGIT   [0-9]

FLOAT_DECIMALS      {DIGIT}({DIGIT}|_)*
FLOAT_EXPONENT      (e|E)(\+|\-)?{DIGIT}({DIGIT}|_)*
/* Integer part of a floating point number */
FLOAT_INTEGER       {DIGIT}({DIGIT}|_)*

/* Printable characters */
PRINT   [ -²] 


/* ═════════════════ Includes & local functions/variables ═════════════════ */

%{
    /* ════════════════ */
    /* ═══ Includes ═══ */
    /* ════════════════ */

    #include "analyzers/lexical.h"

    #include "common/lexical_comp.h"
    #include "common/errors.h"
    #include "common/symbol_table.h"


    /* File access */
    #include <stdio.h>

    /* To store lexemes */
    #include <string.h>
    #include <stdlib.h>
    
    /* size_t */
    #include <stddef.h>


    /* ═══════════════ */
    /* ═══ Defines ═══ */
    /* ═══════════════ */

    /** Block size in bytes. */
    /* TODO As of now, the block size has been set to 4096 bytes as delta
            is intended to be used in Linux, in which the most popular
            fileystem is ext4, whose default block size is 4096 bytes.
            However, it would be nice to automatically detect the block size
            of whichever filesystem lies under.
    */
    #define YY_BUF_SIZE 4096


    /* ═════════════════════════ */
    /* ═══ Private functions ═══ */
    /* ═════════════════════════ */

    /* Declaration of some private fuctions that will be used in the rules
       section */

    /**
     * @brief Makes sure that the current identifier (a lexical component) is
     *        stored in the symbol table.
     *
     * @details
     *  Makes sure that the current identifier (a lexical component) is stored
     *  in the symbol table. That is, this function is meant to be called
     *  after succesfully recognizing an identifier.
     *
     *  Identifiers can be categorized into:
     * 
     *    - New identifiers (not present in the table yet). They always
     *      represent variables.
     *    - Already present identifiers. They may represent commands,
     *      functions, or previously declared variables.
     *
     *  If the identifier is not present yet, a new entry is added to the
     *  symbol table to effectively declare a new variable. New variables are
     *  initialized by default to the base 10 integer number "0". Otherwise,
     *  the already present entry will be retrieved.
     *
     *  In any case, the associated entry will be available in the "yylval"
     *  variable so that the syntactic and semantic analyzer may use it.
     *
     * @return D_LC_IDENTIFIER if the identifier turned out to be a new
     *         varible, the corresponding D_LC_XYZ if the identifier was
     *         already present in an entry in the symbol table, any other
     *         value otherwise.
     */
    int _d_lexical_analyzer_identifier_recognized(
        void
    );

    /**
    * @brief Stores the semantic value that is represented by the current
    *        literal (a lexical component).
    *
    * @details
    *  Interprets and stores the semantic value that is represented by the
    *  current literal (a lexical component), as its category is enough for
    *  the syntactic analyzer, but not for the semantic. Therefore, this
    *  function is meant to be called after successfully recognizing a
    *  literal.
    *
    *  Specifically:
    *    - If the literal is a number, its lexeme is coverted to its
    *      corresponding integer/floating value.
    *    - If the literal is a string, its lexeme is directly copied, skipping
    *      both the initial and ending double quotes.
    *
    *  The semantic value will be available in the "yylval" external variable.
    *
    * @param[in] lexical_component The lexical component category of the
    *                              literal.
    *
    * @return 0 if successful, any other value otherwise.
    */
    int _d_lexical_analyzer_literal_recognized(
        int lexical_component
    );


    /* ══════════════════════════ */
    /* ═══ Current line & col ═══ */
    /* ══════════════════════════ */

    /** How many '\n' have been seen in the input file that is being
        analyzed. */
    size_t _current_line;

    /** How many characters have been seen in the current line. */
    size_t _current_column;

    /** Each time that a rule is matched, the lex. analyzer updates its line
        and col counters, as it may use them for error reporting purposes. The
        syntactic & semantic analyzer may also use them for the same
        purpose. */
    #define YY_USER_ACTION \
        yylloc.first_line = _current_line; \
        yylloc.last_line = _current_line; \
        yylloc.first_column = _current_column; \
        _current_column += yyleng; yylloc.last_column = _current_column;


    /* ══════════════════════════ */
    /* ═══ Nested input files ═══ */
    /* ══════════════════════════ */

    /**
     * @brief This structure will allow the lexical analyzer to read nested
     *        input files.
     *
     * @details
     *  A stack will be simulated in order to read multiple files. The top of
     *  the stack contains the entry of the current file that is being read.
     *  Each time that a new "load" command is issued, a new entry for the new
     *  input file will be pushed into the stack, and popped out when reaching
     *  its EOF.
     *
     * @see flex & bison by John Levine, 1st edition, chapter 2.5
     */
    struct _d_lexical_analyzer_bufstack {
        /** Previous entry in the stack. */
        struct _d_lexical_analyzer_bufstack *previous_entry;

        /** Flex buffer of the file represented by the entry. */
        YY_BUFFER_STATE buffer_state;
        /** Line counter of the file represented by the entry. It only gets
            updated when pushing another entry into the stack. */
        size_t line;
        /** Col counter of the file represented by the entry. It only gets
            updated when pushing another entry into the stack. */
        size_t column;
        /** The file represented by the entry. */
        FILE *file;
        /** Name of the file represented by the entry. */
        const char *filename;
    };

    /** Last entry in the stack. */
    struct _d_lexical_analyzer_bufstack *current_bufstack = NULL;
%}


%%


 /* ════════════════════════════════ Rules ═══════════════════════════════ */

 /* ═════════════════════════ */
 /* ═══ 1-char components ═══ */
 /* ═════════════════════════ */

"+"   { return D_LC_OP_ARITHMETIC_PLUS; }
"-"   { return D_LC_OP_ARITHMETIC_MINUS; }
"*"   { return D_LC_OP_ARITHMETIC_TIMES; }
"/"   { return D_LC_OP_ARITHMETIC_DIV; }
"^"   { return D_LC_OP_ARITHMETIC_EXPONENT; }
"="   { return D_LC_OP_ASSIGNMENT_ASSIGN; }
";"   { return D_LC_SEPARATOR_SEMICOL; }
"("   { return D_LC_SEPARATOR_L_PARENTHESIS; }
")"   { return D_LC_SEPARATOR_R_PARENTHESIS; }


 /* ════════════════════════════════ */
 /* ═══ Multiple-char components ═══ */
 /* ════════════════════════════════ */

 /* ** Comments (single line) ** */

 /*"#".*\n    { ++_current_line; _current_column = 1; return D_LC_WHITESPACE_EOL; } TODO just ignore them*/


 /* ** Double quoted strings ** */

\"\"            { d_errors_parse_show(3, D_ERR_LEX_LITERALS_STRING_EMPTY,
                  _current_line, _current_column); }

\"{PRINT}+\"    { _d_lexical_analyzer_literal_recognized(D_LC_LITERAL_STR);
                  return D_LC_LITERAL_STR; }


 /* ** Identifiers ** */

({ALPHA}|_)({DIGIT}|{ALPHA}|_)* { return _d_lexical_analyzer_identifier_recognized(); }


 /* ** Numbers, binary integer ** */

 /*0[bB](_)*[01][01_]*                                  { _d_lexical_analyzer_literal_recognized(lexical_component);
                                                          lexical_component->category = D_LC_LITERAL_INT;
                                                          return 0; }

0[bB](_)*[^01]({DIGIT}|{ALPHA}|_)*                      { d_errors_parse_show(4,
                                                          D_ERR_LEX_LITERALS_INTEGERS_BINARY_BAD,
                                                          _current_line, _current_column,
                                                          "a binary number needs at least one "
                                                          "'0' -OR- '1' (i.e. 0b1)"); }

0[bB](_)*[01][01_]*([2-9]|{ALPHA})({DIGIT}|{ALPHA}|_)*  { d_errors_parse_show(4,
                                                          D_ERR_LEX_LITERALS_INTEGERS_BINARY_BAD,
                                                          _current_line, _current_column,
                                                          "a binary number may only have '0' "
                                                          "-AND- '1' -AND- '_'"); }*/


 /* ** Numbers, base 10 integer ** */

{DIGIT}({DIGIT}|_)*                                     { _d_lexical_analyzer_literal_recognized(D_LC_LITERAL_INT);
                                                          return D_LC_LITERAL_INT; }

{DIGIT}({DIGIT}|_)*[aAcCdDf-zF-Z]({DIGIT}|{ALPHA}|_)*   { d_errors_parse_show(4, D_ERR_LEX_LITERALS_INTEGERS_BAD,
                                                          _current_line, _current_column, "the only letters that may "
                                                          "follow after an integer number are 'E' -OR- 'e' for "
                                                          "floats (i.e. 10E+15) -AND- 'B' -OR- 'b' for binaries "
                                                          "(i.e. 0b011)"); }


 /* ** Numbers, base 10 float ** */

{FLOAT_INTEGER}\.(({FLOAT_DECIMALS})({FLOAT_EXPONENT})?)?                                                               { _d_lexical_analyzer_literal_recognized(D_LC_LITERAL_FP); 
                                                                                                                          return D_LC_LITERAL_FP; }

\.({FLOAT_DECIMALS})({FLOAT_EXPONENT})?                                                                                 { _d_lexical_analyzer_literal_recognized(D_LC_LITERAL_FP); 
                                                                                                                          return D_LC_LITERAL_FP; }

{FLOAT_INTEGER}{FLOAT_EXPONENT}                                                                                         { _d_lexical_analyzer_literal_recognized(D_LC_LITERAL_FP); 
                                                                                                                          return D_LC_LITERAL_FP; }

{FLOAT_INTEGER}\.{ALPHA}({DIGIT}|{ALPHA}|_)*                                                                            { d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                                                                                                          _current_line, _current_column,
                                                                                                                          "the only letter that may follow after a float's "
                                                                                                                          "decimal dot is 'E' -OR- 'e', providing that there "
                                                                                                                          "is at least a digit beforehand (i.e. 10.3E+15)"); }

({FLOAT_INTEGER})?\.({FLOAT_DECIMALS})([a-dA-Df-zF-Z])({DIGIT}|{ALPHA}|_)*                                              { d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                                                                                                          _current_line, _current_column,
                                                                                                                          "the only letter that may be in a float's decimal "
                                                                                                                          "part is 'E' -OR- 'e' for its exponent (i.e. "
                                                                                                                          "10.3E+15)"); }


((({FLOAT_INTEGER})?\.({FLOAT_DECIMALS})|({FLOAT_INTEGER})))[eE][^0-9+-]({DIGIT}|{ALPHA}|_)*                            { d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                                                                                                          _current_line, _current_column,
                                                                                                                          "the only characters that may follow after a "
                                                                                                                          "float's 'E' -OR- 'e', are '+' -OR- '-' -OR- 'a "
                                                                                                                          "digit' (i.e. 10.3E+15, 10.3E10)"); }

((({FLOAT_INTEGER})?\.({FLOAT_DECIMALS})|({FLOAT_INTEGER})))[eE](\+|\-)?[^0-9]({DIGIT}|{ALPHA}|_)*                      { d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                                                                                                          _current_line, _current_column,
                                                                                                                          "the only characters that may follow after a "
                                                                                                                          "float's exponent and sign declaration are digits "
                                                                                                                          "(i.e. 10.3E+15, 10.3E10_5)"); }

((({FLOAT_INTEGER})?\.({FLOAT_DECIMALS})|({FLOAT_INTEGER})))[eE](\+|\-)?{DIGIT}({DIGIT}|_)*{ALPHA}({DIGIT}|{ALPHA}|_)*  { d_errors_parse_show(4, D_ERR_LEX_LITERALS_FLOATS_BAD,
                                                                                                                          _current_line, _current_column,
                                                                                                                          "a float's decimal number may only have 'digits' "
                                                                                                                          "-AND- '_'"); }


 /* ══════════════════ */
 /* ═══ Whitespace ═══ */
 /* ══════════════════ */

[ \t]+  /* Ignore */
\n      { ++_current_line; _current_column = 1;
          return D_LC_WHITESPACE_EOL; }


 /* ═══════════ */
 /* ═══ EOF ═══ */
 /* ═══════════ */

<<EOF>>     { d_lexical_analyzer_pop_file(); }


 /* ════════════════════ */
 /* ═══ Default rule ═══ */
 /* ════════════════════ */

 /* Catches non-supported characters */
.   { d_errors_parse_show(3, D_ERR_LEX_CHARACTER_NOT_SUPPORTED, _current_line,
                          _current_column); }

%%


/* ════════════════════════════════ Functions ═════════════════════════════ */

/**
 * @brief Implementation of lexical.h/d_lexical_analyzer_initialize
 */
int d_lexical_analyzer_initialize(
    void
)
{
    // By default, all input will be given by the user through stdin...
    yyin = stdin;
    
    // The error-reporting counters need to be initialized
    _current_line = 1;
    _current_column = 1;


    // ... Therefore, stdin gets registered as the first entry in the stack
    if((current_bufstack =
        malloc(sizeof(struct _d_lexical_analyzer_bufstack))) == NULL) {
        
        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                               "lexical.l", "d_lexical_analyzer_initialize",
                               "'malloc' for first entry in bufstack");
        return -1;
    }

    // No input file opened yet
    current_bufstack->previous_entry = NULL;

    // Creates a new flex state that is dedicated to stdin
    current_bufstack->buffer_state = yy_create_buffer(stdin, YY_BUF_SIZE);

    current_bufstack->file = stdin;
    if((current_bufstack->filename = strdup("stdin")) == NULL) {
        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                               "lexical.l", "d_lexical_analyzer_initialize",
                               "'strdup' on filename");
    }

    // And sets the state as the current one
    yy_switch_to_buffer(current_bufstack->buffer_state);


    return 0;
}


/**
 * @brief Implementation of lexical.h/d_lexical_analyzer_new_file
 */
int d_lexical_analyzer_new_file(
    const char *filename
)
{
    FILE *new_file = NULL;
    struct _d_lexical_analyzer_bufstack *new_entry = NULL;


    if(current_bufstack == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "lexical.l", "d_lexical_analyzer_new_file",
                               "'current_bufstack'");
        return -1;
    }

    if(filename == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "lexical.l", "d_lexical_analyzer_new_file",
                               "'filename'");
        return -1;
    }


    if((new_file = fopen(filename, "r")) == NULL) {

        d_errors_internal_show(3, D_ERR_USER_IO_FILE_INACCESSIBLE,
                               "lexical.l", "d_lexical_analyzer_new_file");
        return -1;
    }

    if((new_entry = malloc(sizeof(struct _d_lexical_analyzer_bufstack)))
       == NULL) {
        
        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                                "lexical.l", "d_lexical_analyzer_new_file",
                                "'malloc' for new_entry");
        return -1;
    }


    // There will always be a stacked entry, as stdio shoud never be removed,
    // so there are no NULL deferencing concerns

    // First of all, the current error-reporting counters in the last stack
    // entry must be updated
    current_bufstack->line = _current_line;
    current_bufstack->column = _current_column;


    // Now, the new entry can be set-up
    new_entry->previous_entry = current_bufstack;
    current_bufstack = new_entry;

    // Creates a new flex state that is dedicated to the new file
    new_entry->buffer_state = yy_create_buffer(new_file, YY_BUF_SIZE);

    new_entry->file = new_file;
    if((new_entry->filename = strdup(filename)) == NULL) {
        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                               "lexical.l", "d_lexical_analyzer_initialize",
                               "'strdup' on filename");
    }

    // And sets the state as the current one
    yy_switch_to_buffer(new_entry->buffer_state);

    // Also, the error-reporting counters must be reset
    _current_line = 1;
    _current_column = 1;


    return 0;
}


/**
 * @brief Implementation of symbol_table.h/d_lexical_analyzer_pop_file
 */
int d_lexical_analyzer_pop_file(
    void
)
{
    struct _d_lexical_analyzer_bufstack *last_entry = NULL;
    struct _d_lexical_analyzer_bufstack *previous_entry = NULL;


    if(current_bufstack == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_NULL,
                               "lexical.l", "d_lexical_analyzer_pop_file",
                               "'current_bufstack'");
        return -1;
    }

    if(current_bufstack->file == stdin) {

        d_errors_internal_show(4, D_ERR_INTERN_LOGIC, "lexical.l",
                               "d_lexical_analyzer_pop_file", "tried to "
                               "close stdin");
        return 1;
    }


    last_entry = current_bufstack;
    previous_entry = last_entry->previous_entry;


    // The last opened file gets completely closed
    if(fclose(last_entry->file) == EOF) {

        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                               "lexical.l", "d_lexical_analyzer_pop_file",
                               "'fclose' on last entry's file");
        return -1;
    }

    free((void *) last_entry->filename);
    yy_delete_buffer(last_entry->buffer_state);
    
    // And its entry gets erased
    free(last_entry);


    // Now, the flex state of the previous file is loaded
    yy_switch_to_buffer(previous_entry->buffer_state);

    // Its error-reporting counters are reset
    _current_line = previous_entry->line;
    _current_column = previous_entry->column;

    // And gets set as the last entry of the stack
    current_bufstack = previous_entry;


    return 0;
}


/**
 * @brief Implementation of "lexical.l/_d_lexical_analyzer_literal_recognized"
 */
int _d_lexical_analyzer_literal_recognized(
    int lexical_component
)
{
    switch(lexical_component) {

        case D_LC_LITERAL_INT:

            yylval.dec_number.values.integer =
                                      strtoll(yytext, NULL, 10); // To int64_t
            yylval.dec_number.is_floating = 0;
            
            #ifdef D_DEBUG
            printf("[lexical_analyzer][literal recognized] Saved base 10 "
                   "integer number: %d\n", yylval.dec_number.values.integer);
            #endif

            break;


        case D_LC_LITERAL_FP:

            yylval.dec_number.values.floating = atof(yytext);
            yylval.dec_number.is_floating = 1;
            
            #ifdef D_DEBUG
            printf("[lexical_analyzer][literal recognized] Saved base 10 "
                   "floating point number: %f\n",
                   yylval.dec_number.values.floating);
            #endif

            break;


        case D_LC_LITERAL_STR:

            // +1 to ignore the first quote
            strncpy(yylval.string, yytext + 1,
                    D_LC_LITERAL_STR_MAX_LENGTH - 1);

            // If yytext were to be larger than the max. allowed size, a '\0'
            // needs to be manually added at the end of the resulting string
            // as strncpy will not find it
            //
            // Otherwise, the last character of the resulting string is set to
            // '\0' to effectively remove the second quote
            yylval.string[
                yyleng - 2 < D_LC_LITERAL_STR_MAX_LENGTH - 1 ?
                    yyleng - 2 :
                    D_LC_LITERAL_STR_MAX_LENGTH - 1
            ] = '\0';
            
            #ifdef D_DEBUG
            printf("[lexical_analyzer][literal recognized] Saved string: "
                   "%s\n", yylval.string);
            #endif

            break;


        default:
            d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_INVALID,
                                   "lexical.l",
                                   "_d_lexical_analyzer_literal_recognized",
                                   "the specified type of literal lexical "
                                   "component is not supported");
            return -1;
    }


    return 0;
}


/**
 * @brief Implementation of
 *        "lexical.l/_d_lexical_analyzer_identifier_recognized"
 */
int _d_lexical_analyzer_identifier_recognized(
    void
)
{
    const char *lexeme = NULL;

    struct d_symbol_table_entry entry;
    struct d_symbol_table_entry *entry_in_table = NULL;
    

    // Saving the lexeme
    if((lexeme = (const char *) strdup(yytext)) == NULL) {

        d_errors_internal_show(4, D_ERR_INTERN_SYSCALL_FAILED,
                               "lexical.l",
                               "_d_lexical_analyzer_identifier_recognized",
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

        // If it is not already present, it will be a new variable
        entry.lexeme = lexeme;
        entry.lexical_component = D_LC_IDENTIFIER_VARIABLE;

        // '0' base 10 integer by default
        entry.attribute.dec_number.is_floating = 0;
        entry.attribute.dec_number.values.integer = 0;

        d_symbol_table_add(&entry);
        entry_in_table = d_symbol_table_search(lexeme);
    }

    else {
        // If it is already present, the lexeme (key) must be freed as it is
        // already present in the retrieved entry
        free((void *) lexeme);
    }


    // Anyways, if the current component deserves an entry in the symbol
    // table, its attribute will always be a reference to that entry
    yylval.st_entry = entry_in_table;

    
    return entry_in_table->lexical_component;
}


/**
 * @brief Implementation of lexical.h/d_lexical_analyzer_destroy
 */
int d_lexical_analyzer_destroy(
    void
)
{   
    // All input files that have not been closed yet
    while(current_bufstack->file != stdin) {
        d_lexical_analyzer_pop_file();
    }

    // stdin cannot be closed, but its flex state should be freed before
    // exiting
    yy_delete_buffer(current_bufstack->buffer_state);
    // As well as its other attributes in the stack entry
    free((void *) current_bufstack->filename);
    free((void *) current_bufstack);


    return 0;
}
