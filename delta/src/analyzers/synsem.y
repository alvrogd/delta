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


    /* All possible semantic data types, for both terminal and non-terminal
       symbols */
    /* As of now, all numbers will be floats */
    #define YYSTYPE double
}


/* ══════════════════════════ Compilation Options ═════════════════════════ */



/* ═══════════════════════════ Bison declarations ═════════════════════════ */

/* **  Terminal symbols definitions ** */

/* The lexical analyzer must provide to this one all tokens that are found in
   the input. Therefore, they need to be on the same page about which ID
   represents each token. In order to do so, this analyzer will define all
   lexical components (tokens), and the lexical analyzer will rely on the
   header file that this one generates, and which contains all components. */

/* Almost all arithmetic operators will be left-associative, as it is how a
   calculator usually works. The only operator that is right-associative is
   the exponentiation, also as expected. */

/* The operator precedence is determined by the line in which a symbol is
   defined. A higher line number means a greater operator precedence. */

/* Whitespace */
%token D_LC_WHITESPACE_EOL

/* Numbers */
%token D_LC_LITERAL_INT
%token D_LC_LITERAL_FP

/* Arithmetic operatos */
%left       D_LC_OP_ARITHMETIC_PLUS D_LC_OP_ARITHMETIC_MINUS
%left       D_LC_OP_ARITHMETIC_TIMES D_LC_OP_ARITHMETIC_DIV
%precedence D_LC_OP_ARITHMETIC_NEG /* This precedence rule will allow negating expresions */
%right      D_LC_OP_ARITHMETIC_EXPONENT

/* Separators */
%token D_LC_SEPARATOR_L_PARENTHESIS
%token D_LC_SEPARATOR_R_PARENTHESIS


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
    |   expression          D_LC_WHITESPACE_EOL  { printf("-> %.10g\n", $1); }
    |   error              '\n'                  { yyerrok; yyclearin; }
    ;


/* Mathematical expressions */
expression:
        D_LC_LITERAL_FP                                                                         /*{ printf("un float: %f\n", $1); } */
    |   expression                   D_LC_OP_ARITHMETIC_PLUS     expression                     { $$ = $1 + $3; /*printf("bien: %f = %f + %f\n", $$, $1, $3);*/ }
    |   expression                   D_LC_OP_ARITHMETIC_MINUS    expression                     { $$ = $1 - $3; }
    |   expression                   D_LC_OP_ARITHMETIC_TIMES    expression                     { $$ = $1 * $3; }
    |   expression                   D_LC_OP_ARITHMETIC_DIV      expression
            {
                if($3 != 0) { $$ = $1 / $3; }
                else { $$ = 0.0; d_errors_parse_show(3, D_ERR_USER_INPUT_DIVISION_BY_ZERO, @3.last_line, @3.last_column); }
            }
    |   D_LC_OP_ARITHMETIC_MINUS     expression                  %prec D_LC_OP_ARITHMETIC_NEG   { $$ = -$2; }
    |   expression                   D_LC_OP_ARITHMETIC_EXPONENT expression                     { $$ = pow($1, $3); }
    |   D_LC_SEPARATOR_L_PARENTHESIS expression                  D_LC_SEPARATOR_R_PARENTHESIS   { $$ = $2; }
    |   D_LC_SEPARATOR_L_PARENTHESIS error                  D_LC_SEPARATOR_R_PARENTHESIS        { yyerrok; }
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
    fprintf(stderr, "%s\n", s);
}
