/**
 * @file synsem.y
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief Implementation of analyzers/synsem.h
 */


/* ═══════════════════════ Includes & redefinitions ═══════════════════════ */

%code requires {
    #include "analyzers/synsem.h"

    #include "common/errors.h"
    #include "common/symbol_table.h"

    #include <stdio.h>
    #include <stdlib.h>
    /* Functions that are expected to be available in a calculator */
    #include <math.h>


    /* Max length of string literals */
    #define D_LC_LITERAL_STR_MAX_LENGTH 256
}

/* All possible semantic data types, for both terminal and non-terminal
    symbols */
%union {
    char string[D_LC_LITERAL_STR_MAX_LENGTH];    /* Literal strings */
    double                      dec_value; /* Decimal numbers */
    struct d_symbol_table_entry *st_entry; /* Identifiers */
}


/* ══════════════════════════ Compilation Options ═════════════════════════ */



/* ═══════════════════════════ Bison declarations ═════════════════════════ */

/* NOTE: any symbol that may have a semantic value needs to have explicitely
         set its data type */
          

/* **  Terminal symbols definitions ** */

/* The lexical analyzer must provide to this one all tokens that are found in
   the input. Therefore, they need to be on the same page about which ID
   represents each token. In order to do so, this analyzer will define all
   lexical components (tokens), and the lexical analyzer will rely on the
   header file that this one generates, and which contains all components. */

/* Almost all arithmetic operators will be left-associative, as it is how a
   calculator usually works; the only arith. operator that is right-
   -associative is the exponentiation, also as expected.
   
   The assignment operator is also right-associative, as in C. */

/* The operator precedence is determined by the line in which a symbol is
   defined. A higher line number means a greater operator precedence. */

/* Whitespace */
%token D_LC_WHITESPACE_EOL

/* Strings */
%token <string> D_LC_LITERAL_STR

/* Identifiers */
%token <st_entry> D_LC_IDENTIFIER_VARIABLE
%token <st_entry> D_LC_IDENTIFIER_FUNCTION
%token <st_entry> D_LC_IDENTIFIER_COMMAND
%token <st_entry> D_LC_IDENTIFIER_CONSTANT

/* Numbers */
%token <dec_value> D_LC_LITERAL_INT
%token <dec_value> D_LC_LITERAL_FP

/* Assignment operators */
%right D_LC_OP_ASSIGNMENT_ASSIGN

/* Arithmetic operators */
%left       D_LC_OP_ARITHMETIC_PLUS D_LC_OP_ARITHMETIC_MINUS
%left       D_LC_OP_ARITHMETIC_TIMES D_LC_OP_ARITHMETIC_DIV

/* Separators */
%token D_LC_SEPARATOR_L_PARENTHESIS
%token D_LC_SEPARATOR_R_PARENTHESIS

/* More arithmetic operators */
%precedence D_LC_OP_ARITHMETIC_NEG /* This precedence rule will allow negating expresions */
%right      D_LC_OP_ARITHMETIC_EXPONENT


/* **  Non-terminal symbols definitions ** */

/* Only those that may have semantic values need to be declared, in order to
   know their required data type */
%type <dec_value> expression;


/* ═══════════════════════ Local functions/variables ══════════════════════ */

%code {
    /* Declaration of the private fuction that Bison uses to report errors */
    void yyerror(char const *);
}


%%


/* ══════════════════════════════ Grammar rules ═══════════════════════════ */

/* 'input' is the initial symbol in the grammar */
input:
        %empty /* 'empty' != '\n' */
    |   input  line /* In order to recognize one or more lines */
    ;


/* Each line may contain a mathematical expression; a '\n' is always present
   at the end. The third entry allows basic recovery from syntax errors. */
line:
        D_LC_WHITESPACE_EOL
    |   expression  D_LC_WHITESPACE_EOL
            { printf("   %.10g\n>> ", $1); }
    |   D_LC_IDENTIFIER_COMMAND  D_LC_WHITESPACE_EOL
            {
                if($1->attribute.command.arg_count == 0) {
                    if($1->attribute.command.implementation.argc_0() == D_COMMAND_QUIT_REQUEST) return 0; // Quit requested
                    printf("\n>> ");
                }
                else {
                    d_errors_parse_show(4, D_ERR_USER_INPUT_INCORRECT_ARG_COUNT, @1.last_line, @1.last_column, "1");
                }
            }
    |   D_LC_IDENTIFIER_COMMAND  D_LC_SEPARATOR_L_PARENTHESIS  D_LC_SEPARATOR_R_PARENTHESIS  D_LC_WHITESPACE_EOL
            {
                if($1->attribute.command.arg_count == 0) {
                    if($1->attribute.command.implementation.argc_0() == D_COMMAND_QUIT_REQUEST) return 0; // Quit requested
                    printf("\n>> ");
                }
                else {
                    d_errors_parse_show(4, D_ERR_USER_INPUT_INCORRECT_ARG_COUNT, @3.last_line, @3.last_column, "1");
                }
            }
    |   D_LC_IDENTIFIER_COMMAND  D_LC_SEPARATOR_L_PARENTHESIS  expression  D_LC_SEPARATOR_R_PARENTHESIS  D_LC_WHITESPACE_EOL
            {
                if($1->attribute.command.arg_count == 0) {
                    d_errors_parse_show(4, D_ERR_USER_INPUT_INCORRECT_ARG_COUNT, @3.last_line, @3.last_column, "0");

                }
                else { // Command that receives 1 arg
                    d_errors_parse_show(4, D_ERR_USER_INPUT_INCORRECT_ARG_TYPE, @3.last_line, @3.last_column, "string");
                }
            }
    |   D_LC_IDENTIFIER_COMMAND  D_LC_SEPARATOR_L_PARENTHESIS  D_LC_LITERAL_STR  D_LC_SEPARATOR_R_PARENTHESIS  D_LC_WHITESPACE_EOL
            {
                if($1->attribute.command.arg_count == 1) {
                    printf("typed %s\n", $3);
                    $1->attribute.command.implementation.argc_1($3);
                }
                else {
                    d_errors_parse_show(4, D_ERR_USER_INPUT_INCORRECT_ARG_COUNT, @3.last_line, @3.last_column, "0");
                }
            }
    |   error  D_LC_WHITESPACE_EOL
            { yyerrok; yyclearin; printf("\n>> "); }
    ;


/* Mathematical expressions */
expression:
        D_LC_LITERAL_INT
    |   D_LC_LITERAL_FP
    |   D_LC_IDENTIFIER_VARIABLE
            { $$ = $1->attribute.dec_value; }
    |   D_LC_IDENTIFIER_CONSTANT
            { $$ = $1->attribute.dec_value; }
    |   D_LC_IDENTIFIER_VARIABLE  D_LC_OP_ASSIGNMENT_ASSIGN  expression
            { $$ = $3; $1->attribute.dec_value = $3; }
    |   D_LC_IDENTIFIER_CONSTANT  D_LC_OP_ASSIGNMENT_ASSIGN  expression
            { d_errors_parse_show(3, D_ERR_USER_INPUT_WRITE_CONSTANT, @3.last_line, @3.last_column); }
    |   D_LC_IDENTIFIER_FUNCTION  D_LC_SEPARATOR_L_PARENTHESIS  expression  D_LC_SEPARATOR_R_PARENTHESIS
            { $$ = $1->attribute.function($3); }
    |   expression  D_LC_OP_ARITHMETIC_PLUS  expression
            { $$ = $1 + $3; /*printf("bien: %f = %f + %f\n", $$, $1, $3);*/ }
    |   expression  D_LC_OP_ARITHMETIC_MINUS  expression
            { $$ = $1 - $3; }
    |   expression  D_LC_OP_ARITHMETIC_TIMES  expression
            { $$ = $1 * $3; }
    |   expression  D_LC_OP_ARITHMETIC_DIV  expression
            {
                if($3 != 0) { $$ = $1 / $3; }
                else { d_errors_parse_show(3, D_ERR_USER_INPUT_DIVISION_BY_ZERO, @3.last_line, @3.last_column); }
            }
    |   D_LC_OP_ARITHMETIC_MINUS  expression  %prec  D_LC_OP_ARITHMETIC_NEG
            { $$ = -$2; }
    |   expression  D_LC_OP_ARITHMETIC_EXPONENT  expression
            { $$ = pow($1, $3); }
    |   D_LC_SEPARATOR_L_PARENTHESIS  expression  D_LC_SEPARATOR_R_PARENTHESIS
            { $$ = $2; }
    |   D_LC_SEPARATOR_L_PARENTHESIS  error  D_LC_SEPARATOR_R_PARENTHESIS
            { yyerrok; }
    ;


%%


/* ════════════════════════════════ Functions ═════════════════════════════ */

/**
 * @brief Implementation of synsem.h/d_synsem_analyzer_initialize
 */
int d_synsem_analyzer_initialize()
{
    return 0;
}


/**
 * @brief Implementation of synsem.h/d_synsem_analyzer_parse
 */
int d_synsem_analyzer_parse()
{
    printf(">> ");

    return yyparse();
}


/**
 * @brief Implementation of synsem.h/d_synsem_analyzer_destroy
 */
int d_synsem_analyzer_destroy()
{
    return 0;
}

void yyerror(char const *s)
{
    fprintf(stderr, "%s", s);
}
