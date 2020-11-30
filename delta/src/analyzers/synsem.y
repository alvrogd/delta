/**
 * @file synsem.y
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief Implementation of analyzers/synsem.h
 */


/* ═══════════════════════ Includes & redefinitions ═══════════════════════ */

%code requires {
    /* ════════════════ */
    /* ═══ Includes ═══ */
    /* ════════════════ */

    #include "analyzers/synsem.h"

    #include "common/errors.h"
    #include "common/symbol_table.h"


    #include <stdio.h>
    #include <stdlib.h>

    /* For some basic operations that are expected to be available in a
       calculator */
    #include <math.h>
    // To print int64_t data type
    #include <inttypes.h>


    /* ═══════════════ */
    /* ═══ Defines ═══ */
    /* ═══════════════ */

    /** Max length of string literals. Setting a static buffer for these
        literals will avoid having to constantly reserve and free memory. */
    #define D_LC_LITERAL_STR_MAX_LENGTH 256

    /** Which string is shown as prompt when delta is waiting for orders. */
    #define D_SYNSEM_PROMPT ">> "
}


/* ═══════════════════════════ */
/* ═══ Semantic data types ═══ */
/* ═══════════════════════════ */

/** Bison requires specifying all possible semantic data types, for both
    terminal and non-terminal symbols, through a single union. */
%union {
    /** Literal strings. */
    char string[D_LC_LITERAL_STR_MAX_LENGTH];
    /** Literal decimal numbers. */
    struct d_dec_number dec_number;
    /** Identifiers. */
    struct d_symbol_table_entry *st_entry;
}


/* ══════════════════════════ Compilation Options ═════════════════════════ */



/* ═══════════════════════════ Bison declarations ═════════════════════════ */

/* NOTE: any symbol that may have a semantic value needs to have explicitely
         set the value's data type */
          

/* ═══════════════════════════════════ */
/* ═══ Terminal symbol definitions ═══ */
/* ═══════════════════════════════════ */

/* Following up, there are some notes about how this section works... */

/* The lexical analyzer must provide to this one all tokens that are found in
   the input. Therefore, they need to be on the same page about which ID
   represents each token.
   
   In order to do so, Bison specifies that this analyzer defines all lexical
   components (tokens), and the lexical analyzer will need rely on the header
   header file that this one generates, through which all components are
   exported along with their IDs. */

/* Almost all arithmetic operators will be left-associative, as it is how a
   calculator usually works. The only arithmetic. operator that is right-
   -associative is the exponentiation, also as expected.
   
   The assignment operator has been chosen to be right-associative, as in
   C. */

/* The operator precedence is determined by the line in which a symbol is
   defined. A higher line number means a greater operator precedence. */

/* ** Whitespace ** */
%token D_LC_WHITESPACE_EOL

/* ** Strings ** */
%token <string> D_LC_LITERAL_STR

/* ** Identifiers ** */
%token <st_entry> D_LC_IDENTIFIER_COMMAND
%token <st_entry> D_LC_IDENTIFIER_CONSTANT
%token <st_entry> D_LC_IDENTIFIER_FUNCTION
%token <st_entry> D_LC_IDENTIFIER_VARIABLE

/* ** Numbers ** */
%token <dec_number> D_LC_LITERAL_INT
%token <dec_number> D_LC_LITERAL_FP

/* ** Assignment operators ** */
%right D_LC_OP_ASSIGNMENT_ASSIGN

/* ** Arithmetic operators ** */
%left       D_LC_OP_ARITHMETIC_PLUS D_LC_OP_ARITHMETIC_MINUS
%left       D_LC_OP_ARITHMETIC_TIMES D_LC_OP_ARITHMETIC_DIV

/* ** Separators ** */
%token D_LC_SEPARATOR_L_PARENTHESIS
%token D_LC_SEPARATOR_R_PARENTHESIS

/* ** More (even higher precedence) arithmetic operators ** */
%precedence D_LC_OP_ARITHMETIC_NEG /* This precedence rule will be used in a following section to negate expresions */
%right      D_LC_OP_ARITHMETIC_EXPONENT


/* ═══════════════════════════════════════ */
/* ═══ Non-terminal symbol definitions ═══ */
/* ═══════════════════════════════════════ */

/* Only those that may have semantic values need to be declared, in order to
   know their associated data types. */
%type <dec_number> expression;

/* The other non-terminal symbols do not need to be explicitely declared. */


/* ═══════════════════════ Local functions/variables ══════════════════════ */

%code {
    /** Declaration of the private fuction that Bison uses to report errors */
    // TODO update function
    void yyerror(char const *);

    /** This variable is used by the semantic procedures in order to be able
        to tell if the issued operation involves floating point arithmetic. */
    int is_floating_operation;
}


%%


/* ══════════════════════════════ Grammar rules ═══════════════════════════ */

/* 'input' is the initial symbol in the grammar. */
input:
        /* 'empty' != '\n'; this is just in case the user inputs CTRL+D. */
        %empty
        
    |   /* Read as many lines as possible. */
        input  line
        {
            /* Shown after each line that the user types in */
            printf(D_SYNSEM_PROMPT);
        }
    ;


/* All lines will be '\n'-terminated, and they may also contain:

     - A mathematical expression.
     - Or a built-in command issue.

   The last rule in this subsection allows generic error recovery from syntax
   errors.
*/
line:
        /* Just a '\n'. */
        D_LC_WHITESPACE_EOL


    |   /* A mathematical expression + '\n'. */
        expression  D_LC_WHITESPACE_EOL
            {
                /* Always shows the value of the expression */

                if($1.is_floating) {
                    printf("   %.15g\n", $1.values.floating);
                }
                else {
                    printf("   %" PRId64 "\n", $1.values.integer);
                }
            }


    |   /* A 0-arg command + '\n'. */
        D_LC_IDENTIFIER_COMMAND  D_LC_WHITESPACE_EOL
            {
                /* Checking if the specified command is indeed a 0-arg one */
                if($1->attribute.command.arg_count == 0) {

                    /* If so, the command gets executed; the return value is 
                       checked just to be able to tell if the user has
                       requested that delta exits */
                    if($1->attribute.command.implementation.argc_0()
                       == D_COMMAND_QUIT_REQUEST) {

                        return 0;
                    }
                }

                else {
                    d_errors_parse_show(4,
                                        D_ERR_USER_INPUT_INCORRECT_ARG_COUNT,
                                        @1.last_line, @1.last_column, "0");
                }
            }

    |   /* A 0-arg command + () + '\n'. */
        D_LC_IDENTIFIER_COMMAND  D_LC_SEPARATOR_L_PARENTHESIS  D_LC_SEPARATOR_R_PARENTHESIS  D_LC_WHITESPACE_EOL
            {
                /* Checking if the specified command is indeed a 0-arg one */
                if($1->attribute.command.arg_count == 0) {

                    /* If so, the command gets executed; the return value is checked just to be able to tell if
                       the user has requested that delta exits */
                    if($1->attribute.command.implementation.argc_0()
                       == D_COMMAND_QUIT_REQUEST) {

                        return 0;
                    }
                }

                else {
                    d_errors_parse_show(4, D_ERR_USER_INPUT_INCORRECT_ARG_COUNT, @1.last_line, @1.last_column,
                                        "0");
                }
            }

    |   /* A 1-arg command + ( + math_expression + ) + '\n'.
           It will always be an error, as commands may only take one string as argument at most. */
        D_LC_IDENTIFIER_COMMAND  D_LC_SEPARATOR_L_PARENTHESIS  expression  D_LC_SEPARATOR_R_PARENTHESIS  D_LC_WHITESPACE_EOL
            {
                /* If the command is not even supposed to take an argument */
                if($1->attribute.command.arg_count == 0) {
                    d_errors_parse_show(4, D_ERR_USER_INPUT_INCORRECT_ARG_COUNT, @3.last_line, @3.last_column, "0");
                }

                /* Command that receives 1 argument, but a string-type one, not a mathematical expression */
                else {
                    d_errors_parse_show(4, D_ERR_USER_INPUT_INCORRECT_ARG_TYPE, @3.last_line, @3.last_column, "string");
                }
            }

    |   /* A 1-arg command + ( + string + ) + '\n'. */
        D_LC_IDENTIFIER_COMMAND  D_LC_SEPARATOR_L_PARENTHESIS  D_LC_LITERAL_STR  D_LC_SEPARATOR_R_PARENTHESIS  D_LC_WHITESPACE_EOL
            {
                /* Checking if the specified command is indeed a 1-arg one */
                if($1->attribute.command.arg_count == 1) {
                    /* The command gets executed */
                    $1->attribute.command.implementation.argc_1($3);
                }

                else {
                    d_errors_parse_show(4, D_ERR_USER_INPUT_INCORRECT_ARG_COUNT, @3.last_line, @3.last_column, "1");
                }
            }


    |   /* An error + '\n'. */
        error  D_LC_WHITESPACE_EOL
            /* Tells bison that the error has been catched and managed */
            { yyerrok; yyclearin; }
    ;


/* Mathematical expressions. They are always represented by a decimal number.
   
   The last rule in this subsection allows generic error recovery from syntax
   errors.
*/
expression:
        /* A integer decimal. */
        D_LC_LITERAL_INT

    |   /* A floating point decimal. */
        D_LC_LITERAL_FP


    |   /* A predefined constant. */
        D_LC_IDENTIFIER_CONSTANT
            /* Its value gets directly set as the one of the recognized
               expression */
            { $$ = $1->attribute.dec_number; }

    |   /* A decimal variable. */
        D_LC_IDENTIFIER_VARIABLE
            /* Its value gets directly set as the one of the recognized
               expression */
            { $$ = $1->attribute.dec_number; }

    
    |   /* Assigning a expression to a variable. */
        D_LC_IDENTIFIER_VARIABLE  D_LC_OP_ASSIGNMENT_ASSIGN  expression
            {
                /* The expression's value is set as the variable's one */
                $1->attribute.dec_number = $3;
                /* And it is also set as the value of the recognized
                   expression */
                $$ = $3;
            }

    |   /* Assigning a expression to a constant.
           This is not allowed. */
        D_LC_IDENTIFIER_CONSTANT  D_LC_OP_ASSIGNMENT_ASSIGN  expression
            { d_errors_parse_show(3, D_ERR_USER_INPUT_WRITE_CONSTANT,
                                  @3.last_line, @3.last_column);
            }


    |   /* Calling a mathematical function with no argument.
           This is an error as math functions take exactly one argument. */
        D_LC_IDENTIFIER_FUNCTION  D_LC_SEPARATOR_L_PARENTHESIS  D_LC_SEPARATOR_R_PARENTHESIS
            {
                d_errors_parse_show(4, D_ERR_USER_INPUT_INCORRECT_ARG_COUNT, @2.last_line,
                                    @2.last_column, "1");
            }

    |   /* Calling a mathematical function with a string as argument.
           This is an error as math functions take exactly a "double" argument. */
        D_LC_IDENTIFIER_FUNCTION  D_LC_SEPARATOR_L_PARENTHESIS  D_LC_LITERAL_STR  D_LC_SEPARATOR_R_PARENTHESIS
            {
                d_errors_parse_show(4, D_ERR_USER_INPUT_INCORRECT_ARG_TYPE, @3.last_line, @3.last_column,
                                    "double");
            }

    |   /* Calling a mathematical function with one expression as argument. */
        D_LC_IDENTIFIER_FUNCTION  D_LC_SEPARATOR_L_PARENTHESIS  expression  D_LC_SEPARATOR_R_PARENTHESIS
            {
                /* The function gets executed and its result is set as the recognized expression's
                   one; math functions always return "double" (floating) values */
                $$.values.floating = $1->attribute.function(d_dec_numbers_get_floating_value(&($3)));
                $$.is_floating = 1;
            }



    |   /* Adding two expressions. */
        expression  D_LC_OP_ARITHMETIC_PLUS  expression
            /* The value of the recognized expression is computed through an
               auxiliar function */
            { $$ =  d_dec_numbers_compute_operation('+', &($1), &($3)); }

    |   /* Subtracting two expressions. */
        expression  D_LC_OP_ARITHMETIC_MINUS  expression
            /* The value of the recognized expression is computed through an
               auxiliar function */
            { $$ =  d_dec_numbers_compute_operation('-', &($1), &($3)); }

    |   /* Multiplying two expressions. */
        expression  D_LC_OP_ARITHMETIC_TIMES  expression
            /* The value of the recognized expression is computed through an
               auxiliar function */
            { $$ =  d_dec_numbers_compute_operation('*', &($1), &($3)); }

    |   /* Dividing two expressions. */
        expression  D_LC_OP_ARITHMETIC_DIV  expression
            {
                if(d_dec_numbers_get_floating_value(&($3)) != 0.0) {

                    /* The value of the recognized expression is computed
                       through an auxiliar function */
                    $$ =  d_dec_numbers_compute_operation('/', &($1), &($3));
                }

                else {
                    d_errors_parse_show(3, D_ERR_USER_INPUT_DIVISION_BY_ZERO,
                                        @3.last_line, @3.last_column);
                }   
            }

    |   /* Exponentiation using two expressions. */
        expression  D_LC_OP_ARITHMETIC_EXPONENT  expression
            /* The value of the recognized expression is computed through an
               auxiliar function */
            { $$ =  d_dec_numbers_compute_operation('^', &($1), &($3)); }


    |   /* Negating a expression. */
        D_LC_OP_ARITHMETIC_MINUS  expression  %prec  D_LC_OP_ARITHMETIC_NEG
            /* The "%prec" attribute specifies a special rule precedence */
            { $$ = d_dec_numbers_get_negated_value(&($2)); }


    |   /* A expression between parentheses. */
        D_LC_SEPARATOR_L_PARENTHESIS  expression  D_LC_SEPARATOR_R_PARENTHESIS
            /* Its value gets directly set as the one of the recognized
               expression */
            { $$ = $2; }


    |   /* An error that happens to be between parentheses. */
        D_LC_SEPARATOR_L_PARENTHESIS  error  D_LC_SEPARATOR_R_PARENTHESIS
            /* Tells bison that the error has been catched and managed */
            { yyerrok; yyclearin; }
    ;


%%


/* ════════════════════════════════ Functions ═════════════════════════════ */

/**
 * @brief Implementation of synsem.h/d_synsem_analyzer_initialize
 */
int d_synsem_analyzer_initialize()
{
    // No initializacions are needed as of now


    return 0;
}


/**
 * @brief Implementation of synsem.h/d_synsem_analyzer_parse
 */
int d_synsem_analyzer_parse()
{
    // Shows the promtp and starts the parsing process
    printf(D_SYNSEM_PROMPT);


    return yyparse();
}


/**
 * @brief Implementation of synsem.h/d_synsem_analyzer_destroy
 */
int d_synsem_analyzer_destroy()
{
    // No clean-up is needed as of now


    return 0;
}


void yyerror(char const *s)
{
    fprintf(stderr, "%s", s);
}
