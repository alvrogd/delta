<SYMBOL_TABLE>
	<SYMBOL_TABLE_ENTRY, LC_KEYWORD, import>
	<SYMBOL_TABLE_ENTRY, LC_KEYWORD, double>
	<SYMBOL_TABLE_ENTRY, LC_KEYWORD, int>
	<SYMBOL_TABLE_ENTRY, LC_KEYWORD, while>
	<SYMBOL_TABLE_ENTRY, LC_KEYWORD, foreach>
	<SYMBOL_TABLE_ENTRY, LC_KEYWORD, return>
	<SYMBOL_TABLE_ENTRY, LC_KEYWORD, void>
	<SYMBOL_TABLE_ENTRY, LC_KEYWORD, cast>
	<SYMBOL_TABLE_ENTRY, LC_KEYWORD, string>

<LC_KEYWORD, import>
<LC_IDENTIFIER, std>
<LC_OP_ACCESS_DOT>
<LC_IDENTIFIER, stdio>
<LC_SEPARATOR_SEMICOL>
<LC_KEYWORD, import>
<LC_IDENTIFIER, std>
<LC_OP_ACCESS_DOT>
<LC_IDENTIFIER, algorithm>
<LC_OP_ACCESS_DOT>
<LC_IDENTIFIER, iteration>
<LC_SEPARATOR_SEMICOL>
<LC_KEYWORD, import>
<LC_IDENTIFIER, std>
<LC_OP_ACCESS_DOT>
<LC_IDENTIFIER, exception>
<LC_SEPARATOR_SEMICOL>
<LC_KEYWORD, double>
<LC_IDENTIFIER, beta1>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_KEYWORD, double>
<LC_OP_ACCESS_L_BRACKET>
<LC_OP_ACCESS_R_BRACKET>
<LC_IDENTIFIER, x>
<LC_SEPARATOR_COMMA>
<LC_KEYWORD, double>
<LC_OP_ACCESS_L_BRACKET>
<LC_OP_ACCESS_R_BRACKET>
<LC_IDENTIFIER, y>
<LC_SEPARATOR_COMMA>
<LC_KEYWORD, double>
<LC_IDENTIFIER, x_bar>
<LC_SEPARATOR_COMMA>
<LC_KEYWORD, double>
<LC_IDENTIFIER, y_bar>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_SEPARATOR_L_CURLY>
<LC_IDENTIFIER, enforce>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_IDENTIFIER, x>
<LC_OP_ACCESS_DOT>
<LC_IDENTIFIER, length>
<LC_OP_RELATIONAL_EQUALS>
<LC_IDENTIFIER, y>
<LC_OP_ACCESS_DOT>
<LC_IDENTIFIER, length>
<LC_SEPARATOR_COMMA>
<LC_LITERAL_STR, "x and y should be the same length!">
<LC_SEPARATOR_R_PARENTHESIS>
<LC_SEPARATOR_SEMICOL>
<LC_KEYWORD, double>
<LC_IDENTIFIER, num>
<LC_OP_ASSIGNMENT_ASSIGN>
<LC_LITERAL_INT, 0>
<LC_SEPARATOR_COMMA>
<LC_IDENTIFIER, denom>
<LC_OP_ASSIGNMENT_ASSIGN>
error[E2321]: malformed integer number, the only letters that may follow after an integer number are 'E' -OR- 'e' -OR- 'F' -OR- 'f' -OR- 'i' -OR- 'Li' for floats (i.e. 10E+15, 5f, 2i, 23Li) -AND- 'B' -OR- 'b' for binaries (i.e. 0b011) -AND- suffixes (i.e. 45u, 1000L, 1024LU)
 --> input file : ln 12 : col 29

<LC_SEPARATOR_SEMICOL>
<LC_KEYWORD, int>
<LC_IDENTIFIER, whilectr>
<LC_OP_ASSIGNMENT_ASSIGN>
<LC_LITERAL_INT, 0>
<LC_SEPARATOR_SEMICOL>
<LC_KEYWORD, while>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_IDENTIFIER, whilectr>
<LC_OP_RELATIONAL_LESS_THAN>
<LC_IDENTIFIER, x>
<LC_OP_ACCESS_DOT>
<LC_IDENTIFIER, length>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_SEPARATOR_L_CURLY>
<LC_IDENTIFIER, num>
<LC_OP_ASSIGNMENT_PLUS_ASSIGN>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_IDENTIFIER, x>
<LC_OP_ACCESS_L_BRACKET>
<LC_IDENTIFIER, whilectr>
<LC_OP_ACCESS_R_BRACKET>
<LC_OP_ARITHMETIC_MINUS>
<LC_IDENTIFIER, x_bar>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_OP_ARITHMETIC_TIMES>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_IDENTIFIER, y>
<LC_OP_ACCESS_L_BRACKET>
<LC_IDENTIFIER, whilectr>
<LC_OP_ACCESS_R_BRACKET>
<LC_OP_ARITHMETIC_MINUS>
<LC_IDENTIFIER, y_bar>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_SEPARATOR_SEMICOL>
<LC_IDENTIFIER, whilectr>
<LC_OP_ARITHMETIC_INCREMENT>
<LC_SEPARATOR_SEMICOL>
<LC_SEPARATOR_R_CURLY>
<LC_KEYWORD, foreach>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_IDENTIFIER, xval>
<LC_SEPARATOR_SEMICOL>
<LC_IDENTIFIER, x>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_IDENTIFIER, denom>
<LC_OP_ASSIGNMENT_PLUS_ASSIGN>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_IDENTIFIER, xval>
<LC_OP_ARITHMETIC_MINUS>
<LC_IDENTIFIER, x_bar>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_OP_ARITHMETIC_TIMES>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_IDENTIFIER, xval>
<LC_OP_ARITHMETIC_MINUS>
<LC_IDENTIFIER, x_bar>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_SEPARATOR_SEMICOL>
<LC_KEYWORD, return>
<LC_IDENTIFIER, num>
<LC_OP_ARITHMETIC_DIV>
<LC_IDENTIFIER, denom>
<LC_SEPARATOR_SEMICOL>
<LC_SEPARATOR_R_CURLY>
<LC_KEYWORD, void>
<LC_IDENTIFIER, main>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_SEPARATOR_L_CURLY>
<LC_KEYWORD, double>
<LC_OP_ACCESS_L_BRACKET>
<LC_OP_ACCESS_R_BRACKET>
<LC_IDENTIFIER, x>
<LC_OP_ASSIGNMENT_ASSIGN>
<LC_OP_ACCESS_L_BRACKET>
error[E2322]: malformed binary integer number, a binary number may only have '0' -AND- '1' -AND- '_'
 --> input file : ln 32 : col 26

<LC_SEPARATOR_COMMA>
<LC_LITERAL_INT, 2>
<LC_SEPARATOR_COMMA>
<LC_LITERAL_INT, 3>
<LC_SEPARATOR_COMMA>
<LC_LITERAL_INT, 4>
<LC_SEPARATOR_COMMA>
<LC_LITERAL_INT, 5>
<LC_SEPARATOR_COMMA>
<LC_LITERAL_FP, 6.5>
<LC_SEPARATOR_COMMA>
<LC_LITERAL_INT, 7>
<LC_SEPARATOR_COMMA>
<LC_LITERAL_FP, 7.5>
<LC_SEPARATOR_COMMA>
<LC_LITERAL_INT, 8_000>
<LC_OP_ARITHMETIC_DIV>
<LC_LITERAL_FP, 1.0e+03>
<LC_SEPARATOR_COMMA>
<LC_LITERAL_FP, 8.5>
<LC_OP_ACCESS_R_BRACKET>
<LC_SEPARATOR_SEMICOL>
<LC_KEYWORD, double>
<LC_OP_ACCESS_L_BRACKET>
<LC_OP_ACCESS_R_BRACKET>
<LC_IDENTIFIER, y>
<LC_OP_ASSIGNMENT_ASSIGN>
<LC_OP_ACCESS_L_BRACKET>
<LC_LITERAL_INT, 1>
<LC_SEPARATOR_COMMA>
<LC_LITERAL_INT, 2>
<LC_SEPARATOR_COMMA>
<LC_LITERAL_INT, 3>
<LC_SEPARATOR_COMMA>
<LC_LITERAL_FP, 3.2>
<LC_SEPARATOR_COMMA>
<LC_LITERAL_FP, 3.4>
<LC_SEPARATOR_COMMA>
<LC_LITERAL_FP, 40e-1>
<LC_SEPARATOR_COMMA>
<LC_LITERAL_FP, 4.2>
<LC_SEPARATOR_COMMA>
<LC_LITERAL_FP, 4.3>
<LC_SEPARATOR_COMMA>
<LC_LITERAL_FP, 4.35>
<LC_SEPARATOR_COMMA>
<LC_LITERAL_FP, 4.37>
<LC_OP_ACCESS_R_BRACKET>
<LC_SEPARATOR_SEMICOL>
<LC_KEYWORD, double>
<LC_IDENTIFIER, x_bar>
<LC_SEPARATOR_COMMA>
<LC_IDENTIFIER, y_bar>
<LC_SEPARATOR_COMMA>
<LC_IDENTIFIER, b1>
<LC_SEPARATOR_COMMA>
<LC_IDENTIFIER, _0>
<LC_SEPARATOR_SEMICOL>
<LC_IDENTIFIER, x_bar>
<LC_OP_ASSIGNMENT_ASSIGN>
<LC_IDENTIFIER, sum>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_IDENTIFIER, x>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_OP_ARITHMETIC_DIV>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_KEYWORD, cast>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_KEYWORD, double>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_IDENTIFIER, x>
<LC_OP_ACCESS_DOT>
<LC_IDENTIFIER, length>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_SEPARATOR_SEMICOL>
<LC_IDENTIFIER, y_bar>
<LC_OP_ASSIGNMENT_ASSIGN>
<LC_IDENTIFIER, sum>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_IDENTIFIER, y>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_OP_ARITHMETIC_DIV>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_KEYWORD, cast>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_KEYWORD, double>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_IDENTIFIER, y>
<LC_OP_ACCESS_DOT>
<LC_IDENTIFIER, length>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_SEPARATOR_SEMICOL>
<LC_IDENTIFIER, b1>
<LC_OP_ASSIGNMENT_ASSIGN>
<LC_IDENTIFIER, beta1>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_IDENTIFIER, x>
<LC_SEPARATOR_COMMA>
<LC_IDENTIFIER, y>
<LC_SEPARATOR_COMMA>
<LC_IDENTIFIER, x_bar>
<LC_SEPARATOR_COMMA>
<LC_IDENTIFIER, y_bar>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_SEPARATOR_SEMICOL>
<LC_IDENTIFIER, _0>
<LC_OP_ASSIGNMENT_ASSIGN>
<LC_IDENTIFIER, y_bar>
<LC_OP_ARITHMETIC_MINUS>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_IDENTIFIER, b1>
<LC_OP_ARITHMETIC_TIMES>
<LC_IDENTIFIER, x_bar>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_SEPARATOR_SEMICOL>
<LC_IDENTIFIER, writefln>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_LITERAL_STR, "\"Slope\": %f">
<LC_SEPARATOR_COMMA>
<LC_IDENTIFIER, b1>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_SEPARATOR_SEMICOL>
<LC_IDENTIFIER, writefln>
<LC_SEPARATOR_L_PARENTHESIS>
<LC_LITERAL_STR, "\"Intercept\": %f">
<LC_SEPARATOR_COMMA>
<LC_IDENTIFIER, _0>
<LC_SEPARATOR_R_PARENTHESIS>
<LC_SEPARATOR_SEMICOL>
<LC_IDENTIFIER, a>
<LC_OP_ASSIGNMENT_ASSIGN>
<LC_LITERAL_INT, 1>
<LC_SEPARATOR_SEMICOL>
<LC_IDENTIFIER, a>
<LC_OP_ASSIGNMENT_ASSIGN>
<LC_LITERAL_STR, " +/ 1">
<LC_SEPARATOR_SEMICOL>
<LC_IDENTIFIER, a>
<LC_OP_ASSIGNMENT_ASSIGN>
<LC_OP_ARITHMETIC_TIMES>
<LC_LITERAL_INT, 2>
<LC_OP_ARITHMETIC_DIV>
<LC_LITERAL_INT, 3>
<LC_SEPARATOR_SEMICOL>
<LC_SEPARATOR_R_CURLY>

<SYMBOL_TABLE>
	<SYMBOL_TABLE_ENTRY, LC_KEYWORD, import>
	<SYMBOL_TABLE_ENTRY, LC_KEYWORD, double>
	<SYMBOL_TABLE_ENTRY, LC_KEYWORD, int>
	<SYMBOL_TABLE_ENTRY, LC_KEYWORD, while>
	<SYMBOL_TABLE_ENTRY, LC_KEYWORD, foreach>
	<SYMBOL_TABLE_ENTRY, LC_KEYWORD, return>
	<SYMBOL_TABLE_ENTRY, LC_KEYWORD, void>
	<SYMBOL_TABLE_ENTRY, LC_KEYWORD, cast>
	<SYMBOL_TABLE_ENTRY, LC_KEYWORD, string>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, std>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, stdio>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, algorithm>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, iteration>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, exception>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, beta1>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, x>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, y>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, x_bar>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, y_bar>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, enforce>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, length>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, num>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, denom>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, whilectr>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, xval>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, main>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, b1>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, _0>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, sum>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, writefln>
	<SYMBOL_TABLE_ENTRY, LC_IDENTIFIER, a>

