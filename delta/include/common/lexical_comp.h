/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_INCLUDE_COMMON_LEXICAL_COMP_H_INCLUDED
# define YY_YY_INCLUDE_COMMON_LEXICAL_COMP_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 12 "src/analyzers/synsem.y"

    #include "analyzers/synsem.h"

    #include "common/errors.h"
    #include "common/symbol_table.h"

    #include <stdio.h>
    #include <stdlib.h>
    /* Functions that are expected to be available in a calculator */
    #include <math.h>

#line 60 "include/common/lexical_comp.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    D_LC_WHITESPACE_EOL = 258,
    D_LC_IDENTIFIER_VARIABLE = 259,
    D_LC_IDENTIFIER_FUNCTION = 260,
    D_LC_IDENTIFIER_COMMAND = 261,
    D_LC_IDENTIFIER_CONSTANT = 262,
    D_LC_LITERAL_INT = 263,
    D_LC_LITERAL_FP = 264,
    D_LC_OP_ASSIGNMENT_ASSIGN = 265,
    D_LC_OP_ARITHMETIC_PLUS = 266,
    D_LC_OP_ARITHMETIC_MINUS = 267,
    D_LC_OP_ARITHMETIC_TIMES = 268,
    D_LC_OP_ARITHMETIC_DIV = 269,
    D_LC_SEPARATOR_L_PARENTHESIS = 270,
    D_LC_SEPARATOR_R_PARENTHESIS = 271,
    D_LC_OP_ARITHMETIC_NEG = 272,
    D_LC_OP_ARITHMETIC_EXPONENT = 273
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 26 "src/analyzers/synsem.y"

    double                      dec_value; /* Decimal numbers */
    struct d_symbol_table_entry *st_entry; /* Identifiers */

#line 95 "include/common/lexical_comp.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_INCLUDE_COMMON_LEXICAL_COMP_H_INCLUDED  */
