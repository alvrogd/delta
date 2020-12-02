/**
 * @file help.c
 * @author Álvaro Goldar Dieste
 * @date Nov 2020
 *
 * @brief Implementation of common/help.h
 */


#include "common/help.h"

#include "common/errors.h"


#include <stdio.h>


/**
 * @brief Implementation of help.h/d_help_show_topic_help
 */
void d_help_show_topic_help(
    int topic
)
{
    switch (topic) {
    
        /* Category: intro */

        case D_HELP_INTRO:

            printf(
                "# Delta\n"
                "\n"
                "Delta is a **mathematical expression interpreter** written in C.\n"
                "\n"
                "Type `help` or `help()` for information on how to use Delta.\n"
                "\n"
            );

            break;



        /* Category: features */

        case D_HELP_FEATURES:

            printf(
                "## Features\n"
                "\n"
                "Delta is a basic command line tool that **recognizes mathematical\n"
                "expressions and computes them**. This project was made at the same time\n"
                "that the author was learning about compiler and interpreter\n"
                "technologies, so that it would help him in the process.\n"
                "\n"
            );

            break;


        case D_HELP_FEATURES_DATA_TYPES:

            printf(
                "### Data types\n"
                "\n"
                "As of now, only **base-10 numbers are supported**.\n"
                "\n"
                "-   **Integer numbers** contain one or more digits. They are represented\n"
                "    with the `int64_t` data type.\n"
                "\n"
                "    >> 45\n"
                "       45\n"
                "    >> 45a\n"
                "    error[E2321]: malformed integer number, the only letters that may \n"
                "    follow after an integer number are 'E' -OR- 'e' for floats (i.e. \n"
                "    10E+15)\n"
                "     --> stdin : ln 1 : col 4\n"
                "\n"
                "-   **Floating point numbers** can be written in multiple ways, while\n"
                "    also supporting scientific notation. They are represented with the\n"
                "    `double` data type.\n"
                "\n"
                "    >> 1.15\n"
                "       1.15\n"
                "    >> 2.43E-4\n"
                "       0.000243\n"
                "    >> 0.14\n"
                "       0.14\n"
                "    >> .3\n"
                "       0.3\n"
                "    >> .8E-3\n"
                "       0.0008\n"
                "    >> 10E5\n"
                "       1000000\n"
                "    >> 10E5bcd\n"
                "    error[E2311]: malformed floating number, a float's decimal number may\n"
                "    only have 'digits' -AND- '_'\n"
                "     --> stdin : ln 2 : col 8\n"
                "\n"
            );

            break;


        case D_HELP_FEATURES_BASIC_OPERATIONS:

            printf(
                "### Basic operations\n"
                "\n"
                "Delta supports the following **basic mathematical operations**:\n"
                "\n"
                "-   Addition.\n"
                "-   Subtraction.\n"
                "-   Multiplication.\n"
                "-   Division.\n"
                "-   Modulo.\n"
                "-   Exponentiation.\n"
                "\n"
                "These operations can be issued just as you would usually do in any\n"
                "programming language:\n"
                "\n"
                "    >> 1+2\n"
                "       3\n"
                "    >> 5^3\n"
                "       125\n"
                "\n"
                "Both **integer and floating point arithmetic is supported**. Delta will\n"
                "dynamically check the data types of the values that you provide to those\n"
                "operations, and it will use:\n"
                "\n"
                "-   Integer arithmetic when the two operands are integer numbers.\n"
                "-   Floating point arithmetic otherwise.\n"
                "\n"
                "    >> 1 - 2.5\n"
                "       -1.5\n"
                "    >> 4 * 3\n"
                "       12\n"
                "    >> 2^0.5\n"
                "       1.4142135623731\n"
                "\n"
                "Beware that if the result of a floating point operation has no decimal\n"
                "digits, they will not be shown:\n"
                "\n"
                "    >> 0.5 + 0.5\n"
                "       1\n"
                "\n"
                "It is true that integer numbers could also be represented using the\n"
                "floating point data type. However, integer arithmetic is used whenever\n"
                "it is possible as floating point arithmetic is well known to have\n"
                "accuracy problems.\n"
                "\n"
                "Moreover, you must be careful when issuing division operations. Just as\n"
                "in the C language, a division between two integer numbers will not\n"
                "report any decimal digits:\n"
                "\n"
                "    >> 5 / 2\n"
                "       2\n"
                "    >> 5.0 / 2\n"
                "       2.5\n"
                "\n"
                "The modulo operations supports both times of arithmetic:\n"
                "\n"
                "    >> 5 %% 2\n"
                "       1\n"
                "    >> 5.5 %% 2\n"
                "       1.5\n"
                "\n"
                "Divisions by zero will also be caught and reported:\n"
                "\n"
                "    >> 5 / 2\n"
                "       2\n"
                "    >> 5.0 / 2\n"
                "       2.5\n"
                "    >> 5 / (1 - 1)\n"
                "    error[E4001]: caught a division by 0\n"
                "     --> stdin : ln 57 : col 12\n"
                "\n"
                "Naturally, you may also group expressions using parentheses:\n"
                "\n"
                "    >> 1 + 2 * 3\n"
                "       7\n"
                "    >> (1 + 2) * 3\n"
                "       9\n"
                "\n"
                "And dangling parentheses will be reported as errors:\n"
                "\n"
                "    >> (1 + 2))\n"
                "       3\n"
                "    error[E3002]: caught a dangling parenthesis\n"
                "     --> stdin : ln 19 : col 9\n"
                "\n"
            );

            break;


        case D_HELP_FEATURES_BASIC_MATH_FUNCTIONS:

            printf(
                "### Basic mathematical functions\n"
                "\n"
                "Delta also preloads some **basic mathematical functions**:\n"
                "\n"
                "-   **Generic functions**:\n"
                "    -   `fabs()`: absolute value.\n"
                "-   **Exponential functions**:\n"
                "    -   `exp()`: e raised to the given power.\n"
                "    -   `exp2()`: 2 raised to the given power.\n"
                "    -   `log()`: natural logarithm (base *e*).\n"
                "    -   `log2()`: binary logarithm (base *2*).\n"
                "    -   `log10()`: common logarithm (base *10*).\n"
                "-   **Power functions**:\n"
                "    -   `cbrt()`: cubic root.\n"
                "    -   `sqrt()`: square root.\n"
                "-   **Trigonometric functions**:\n"
                "    -   `acos()`: arc cosine.\n"
                "    -   `asin()`: arc sine.\n"
                "    -   `atan()`: arc tangent.\n"
                "    -   `cos()`: cosine.\n"
                "    -   `sin()`: sine.\n"
                "    -   `tan()`: tangent.\n"
                "-   **Hyperbolic functions**:\n"
                "    -   `acosh()`: hyperbolic arc cosine.\n"
                "    -   `asinh()`: hyperbolic arc sine.\n"
                "    -   `atanh()`: hyperbolic arc tangent.\n"
                "    -   `cosh()`: hyperbolic cosine.\n"
                "    -   `sinh()`: hyperbolic sine.\n"
                "    -   `tanh()`: hyperbolic tangent.\n"
                "\n"
                "These functions take exactly 1 argument, and return 1 floating point\n"
                "value:\n"
                "\n"
                "    >> sqrt(2)\n"
                "       1.4142135623731\n"
                "    >> log(5.2)\n"
                "       1.64865862558738\n"
                "    >> exp()\n"
                "    error[E4002]: incorrect amount of arguments, expected 1 args\n"
                "     --> stdin : ln 23 : col 6\n"
                "\n"
                "Beware that trigonometric functions expect that the input value is given\n"
                "in radians:\n"
                "\n"
                "    >> sin(3.14)\n"
                "       0.00159265291648683\n"
                "\n"
            );

            break;


        case D_HELP_FEATURES_VARIABLES_AND_CONSTANTS:

            printf(
                "### Variables and constants\n"
                "\n"
                "Delta can **store the values of computed expressions in variables**. In\n"
                "order to declare a variable, you just need to give it a name, as Delta\n"
                "uses dynamic typing. These names must start with a letter, and contain\n"
                "zero or more letters/digits/underscores:\n"
                "\n"
                "    >> a = 12\n"
                "       12\n"
                "    >> b123_a = 23\n"
                "       23\n"
                "\n"
                "By default, variables are initialized to the integer value *0*:\n"
                "\n"
                "    >> b\n"
                "       0\n"
                "    >> b + 1\n"
                "       1\n"
                "\n"
                "Which allows some exotic expressions such as the following one, even\n"
                "though they are discouraged due to how obscure they are:\n"
                "\n"
                "    >> c = d + 1\n"
                "       1\n"
                "\n"
                "In the previous snippet, both `c` and `d` are declared as new variables.\n"
                "`d` is given the default value, which results in `c = 0 + 1`.\n"
                "\n"
                "Just as dynamic checking is used in mathematical operations, Delta also\n"
                "determines which data type each variable needs to use depending on the\n"
                "assigned value. For instance, if we have the two following variables:\n"
                "\n"
                "    >> a_floating_one = 23.56\n"
                "       23.56\n"
                "    >> an_integer_one = 45\n"
                "       45\n"
                "\n"
                "The **`ws` (*Workspace Show*) built-in command can be used to list the\n"
                "registered variables, as well as their types**.\n"
                "\n"
                "    >> ws\n"
                "       1. Mathematical constants:\n"
                "          e => 2.718282\n"
                "          pi => 3.141593\n"
                "       2. Loaded mathematical functions:\n"
                "          fabs\n"
                "          exp\n"
                "          exp2\n"
                "          log\n"
                "          log2\n"
                "          log10\n"
                "          cbrt\n"
                "          sqrt\n"
                "          acos\n"
                "          asin\n"
                "          atan\n"
                "          cos\n"
                "          sin\n"
                "          tan\n"
                "          acosh\n"
                "          asinh\n"
                "          atanh\n"
                "          cosh\n"
                "          sinh\n"
                "          tanh\n"
                "       3. Your variables:\n"
                "          [floating] a_floating_one => 23.56\n"
                "          [integer] an_integer_one => 45\n"
                "\n"
                "Furthermore, as you may see in the previous snippet, Delta also\n"
                "**preloads some mathematical constants**:\n"
                "\n"
                "-   *e*.\n"
                "-   *π*.\n"
                "\n"
                "You can use their values just as you would with any standard variable:\n"
                "\n"
                "    >> a = 2\n"
                "       2\n"
                "    >> a * pi\n"
                "       6.28318530717959\n"
                "\n"
                "But trying to overwrite them will result in an error:\n"
                "\n"
                "    >> e = 1.2\n"
                "    error[E3001]: mathematical constants are immutable\n"
                "     --> stdin : ln 41 : col 2\n"
                "\n"
            );

            break;


        case D_HELP_FEATURES_WORKSPACE:

            printf(
                "### The WorkSpace\n"
                "\n"
                "The **Delta WorkSpace, which can be shown using the `ws` built-in\n"
                "command**, as previously seen, **contains a handful of information**\n"
                "about the current session:\n"
                "\n"
                "1.  Which mathematical constants have been loaded.\n"
                "2.  Which mathematical functions have been loaded.\n"
                "3.  Which variables have been defined.\n"
                "\n"
                "All defined **variables can be cleared using the `wsc` (*WorkSpace\n"
                "Clear*) built-in command**:\n"
                "\n"
                "    >> pie = 3.14\n"
                "       3.14\n"
                "    >> ws\n"
                "       1. Mathematical constants:\n"
                "          e => 2.718282\n"
                "          pi => 3.141593\n"
                "       2. Loaded mathematical functions:\n"
                "          fabs\n"
                "          <..>\n"
                "          tanh\n"
                "       3. Your variables:\n"
                "          [floating] pie => 3.14\n"
                "    >> wsc\n"
                "       Workspace successfully cleared\n"
                "    >> ws()\n"
                "       1. Mathematical constants:\n"
                "          e => 2.718282\n"
                "          pi => 3.141593\n"
                "       2. Loaded mathematical functions:\n"
                "          fabs\n"
                "          <..>\n"
                "          tanh\n"
                "       3. Your variables:\n"
                "\n"
                "It is worth noting that commands that take no arguments can be called\n"
                "with or without `()`.\n"
                "\n"
            );

            break;


        case D_HELP_FEATURES_LOAD_MATH_FUNCTIONS:

            printf(
                "### Loading additional mathematical functions\n"
                "\n"
                "Delta supports **dynamically loading external libraries and their\n"
                "functions**, in order to expand the repertoire of available mathematical\n"
                "functions.\n"
                "\n"
                "For instance, Delta includes two small dynamic libraries just for\n"
                "testing purposes. One of them allows converting a value from degrees to\n"
                "radians, and the other does the opposite.\n"
                "\n"
                "1.  First of all, you need to **load the libraries using the `from()`\n"
                "    built-in command** (if you do not type `./`, the library will be\n"
                "    searched in the system’s default path):\n"
                "\n"
                "    >> from(\"./libdl_radians.so\")\n"
                "       Library successfully loaded\n"
                "    >> from(\"./libdl_degrees.so\")\n"
                "       Library successfully loaded\n"
                "\n"
                "2.  Secondly, you need to remember that the last loaded library is the\n"
                "    one from which new functions will be loaded. Right now,\n"
                "    `libdl_degrees.so` is the currently selected library. In order to\n"
                "    select another library, you can just use the same `from` command. Do\n"
                "    not worry, it will not load once again the library if it already is:\n"
                "\n"
                "    >> from(\"./libdl_radians.so\")\n"
                "       Library successfully selected\n"
                "\n"
                "3.  Now that `libdl_radians.so` has been set as the currently selected\n"
                "    library, we can **load** the `radians()` **mathematical function**\n"
                "    that can be found in it, **using the `import()` built-in command**:\n"
                "\n"
                "    >> ws\n"
                "       1. Mathematical constants:\n"
                "          e => 2.718282\n"
                "          pi => 3.141593\n"
                "       2. Loaded mathematical functions:\n"
                "          fabs\n"
                "          <..>\n"
                "          tanh\n"
                "       3. Your variables:\n"
                "    >> import(\"radians\")\n"
                "       Function successfully loaded\n"
                "    >> ws\n"
                "       1. Mathematical constants:\n"
                "          e => 2.718282\n"
                "          pi => 3.141593\n"
                "       2. Loaded mathematical functions:\n"
                "          fabs\n"
                "          <..>\n"
                "          tanh\n"
                "          radians\n"
                "       3. Your variables:\n"
                "\n"
                "4.  And the function can now be called just as any other mathematical\n"
                "    function:\n"
                "\n"
                "    >> radians(180)\n"
                "       3.14159265358979\n"
                "\n"
                "Beware that if a variable or a mathematical function that goes by the\n"
                "same name is already defined, it will result in an error. In any case,\n"
                "when dealing with variables, you can just clear your WorkSpace to be\n"
                "able to load that desired function:\n"
                "\n"
                "    >> degrees = 360\n"
                "       360\n"
                "    >> from(\"./libdl_degrees.so\")\n"
                "       Library successfully loaded\n"
                "    >> import(\"degrees\")\n"
                "    error[E5104]: a variable that goes by the same name is already\n"
                "    defined\n"
                "     --> internal file : commands.c : _d_commands_load_function()\n"
                "\n"
                "    >> wsc\n"
                "       Workspace successfully cleared\n"
                "    >> import(\"degrees\")\n"
                "       Function successfully loaded\n"
                "    >> degrees(pi)\n"
                "       180\n"
                "\n"
                "All loaded libraries are automatically closed by Delta upon exiting.\n"
                "\n"
            );

            break;


        case D_HELP_FEATURES_NOTEBOOKS:

            printf(
                "### Notebooks\n"
                "\n"
                "A **Delta Notebook is just a text file that lists one or multiple Delta\n"
                "statements**. A Notebook can be **loaded using the `load()` built-in\n"
                "command**, and Delta will read it line by line, executing each statement\n"
                "that is found.\n"
                "\n"
                "For instance, Delta includes three small Notebooks just for testing\n"
                "purposes. The first one is:\n"
                "\n"
                "    delta/delta/build$ cat ../test/notebooks/notebook_1.delta \n"
                "    pie = 3.14\n"
                "    sin(degrees(pie))\n"
                "\n"
                "Beware that Notebooks must end with a new line. Otherwise, Delta will\n"
                "not correctly read them.\n"
                "\n"
                "Anyways, if we load that first Notebook, Delta will define a new\n"
                "variable `pie` and print its sine, after converting it to degrees using\n"
                "one of the previously shown test dynamic libraries:\n"
                "\n"
                "    >> from(\"./libdl_degrees.so\")\n"
                "       Library successfully loaded\n"
                "    >> import(\"degrees\")\n"
                "       Function successfully loaded\n"
                "    >> load(\"../test/notebooks/notebook_1.delta\")\n"
                "       File successfully loaded\n"
                "       3.14\n"
                "       -0.743284239769269\n"
                "    >> ws\n"
                "       1. Mathematical constants:\n"
                "          e => 2.718282\n"
                "          pi => 3.141593\n"
                "       2. Loaded mathematical functions:\n"
                "          fabs\n"
                "          <..>\n"
                "          tanh\n"
                "          degrees\n"
                "       3. Your variables:\n"
                "          [floating] pie => 3.14\n"
                "\n"
                "It is worth noting that the read statements are not printed, but just\n"
                "executed and their results shown.\n"
                "\n"
                "Furthermore, **Delta supports nested Notebooks**; that is, Notebooks\n"
                "that issue a `load` on another Notebooks.\n"
                "\n"
                "For instance, the second test Notebook is as follows:\n"
                "\n"
                "    delta/delta/build$ cat ../test/notebooks/notebook_2.delta \n"
                "    hello_from_notebook_2 = 1\n"
                "    load(\"../test/notebooks/notebook_3.delta\")\n"
                "    result = hello_from_notebook_2 + hello_from_notebook_3\n"
                "\n"
                "This Notebook will initialize the variable `hello_from_notebook_2`, load\n"
                "the third Notebook, and initialize the variable `result` to the addition\n"
                "of the previous variable and another one that is defined in that third\n"
                "Notebook:\n"
                "\n"
                "    delta/delta/build$ cat ../test/notebooks/notebook_3.delta \n"
                "    hello_from_notebook_3 = 1\n"
                "    (123))\n"
                "\n"
                "Non only that, but the third Notebook will produce an error in order to\n"
                "show that Delta details where each error is found, regarding source file\n"
                "(or stdin), line and column:\n"
                "\n"
                "    >> load(\"../test/notebooks/notebook_2.delta\")\n"
                "       File successfully loaded\n"
                "       1\n"
                "       File successfully loaded\n"
                "       1\n"
                "       123\n"
                "    error[E3002]: caught a dangling parenthesis\n"
                "     --> ../test/notebooks/notebook_3.delta : ln 2 : col 7\n"
                "\n"
                "       2\n"
                "    >> ws\n"
                "       1. Mathematical constants:\n"
                "          e => 2.718282\n"
                "          pi => 3.141593\n"
                "       2. Loaded mathematical functions:\n"
                "          fabs\n"
                "          <..>\n"
                "          tanh\n"
                "       3. Your variables:\n"
                "          [integer] hello_from_notebook_2 => 1\n"
                "          [integer] hello_from_notebook_3 => 1\n"
                "          [integer] result => 2\n"
                "\n"
            );

            break;


        case D_HELP_FEATURES_ECHO:

            printf(
                "### Switching echo on and off\n"
                "\n"
                "After each recognized mathematical expression, Delta outputs its value.\n"
                "You can disable this behavior just by adding a semicolon at the end of\n"
                "the line:\n"
                "\n"
                "    >> 1 + 2\n"
                "       3\n"
                "    >> 1 + 2;\n"
                "    >> value = sin(3);\n"
                "    >> ws\n"
                "       1. Mathematical constants:\n"
                "          e => 2.718282\n"
                "          pi => 3.141593\n"
                "       2. Loaded mathematical functions:\n"
                "          fabs\n"
                "          <..>\n"
                "          tanh\n"
                "       3. Your variables:\n"
                "          [floating] value => 0.1411200081\n"
                "\n"
            );

            break;


        case D_HELP_FEATURES_HELP:

            printf(
                "### Requesting help\n"
                "\n"
                "On which topic would you like to get information?\n"
                "\n"
                "    -   10. Overview of Delta's features.\n"
                "    -   11. Data types.\n"
                "    -   12. Basic operations.\n"
                "    -   13. Basic mathematical functions.\n"
                "    -   14. Variables and constants.\n"
                "    -   15. The WorkSpace.\n"
                "    -   16. Loading additional mathematical functions.\n"
                "    -   17. Notebooks.\n"
                "    -   18. Switching echo on and off.\n"
                "    -   19. Exiting Delta.\n"
                "\n"
                "Type `dhelp(\"<topic_id>\")` for information on the specified topic.\n"
                "\n"
                "For instance, to get information on data types: `dhelp(\"11\")`\n"
                "\n"
            );

            break;


        case D_HELP_FEATURES_QUIT:

            printf(
                "### Exiting Delta\n"
                "\n"
                "Finally, you may **exit Delta just by using the `quit` built-in\n"
                "command**. As previously said, Delta will take care by itself of closing\n"
                "any dynamically loaded libraries.\n"
                "\n"
            );

            break;



        /* No match */

        default:
            d_errors_internal_show(4, D_ERR_INTERN_ARGUMENT_INVALID,
                                   "help.c",
                                   "d_help_show_topic_help",
                                   "there is not info about the specified "
                                   "topic");
            break;
    }
}
