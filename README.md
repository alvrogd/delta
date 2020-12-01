# Delta

Delta is a **mathematical expressions interpreter** written in C.

## Features

Delta is a basic command line tool that **recognizes mathematical expressions and computes them**. This project was made at the same time that the author was learning about compiler and interpreter technologies, so that it would help him in the process.

### Data types

As of now, only **base-10 numbers are supported**.

* **Integer numbers** contain one or more digits. They are represented with the `int64_t` data type.
```
>> 45
   45
```
* **Floating point numbers** can be written in multiple ways, while also supporting scientific notation. They are represented with the `double` data type.
```
>> 1.15
   1.15
>> 2.43E-4
   0.000243
>> 0.14
   0.14
>> .3
   0.3
>> .8E-3
   0.0008
>> 10E5
   1000000
```

### Basic operations

Delta supports the following **basic mathematical operations**: addition, subtraction, multiplication, division, and exponentiation.

These operations can be issued just as you would usually do in any programming language:

```
>> 1+2
   3
>> 5^3
   125
```

Both **integer and floating point arithmetic is supported**. Delta will dynamically check the data types of the values that you provide to those operations, and it will use:

* Integer arithmetic when the two operands are integer numbers.
* Floating point arithmetic otherwise.

```
>> 1 - 2.5
   -1.5
>> 4 * 3
   12
>> 2^0.5
   1.4142135623731
```

Beware that if the result of a floating point operation has no decimal digits, they will not be shown:

```
>> 0.5 + 0.5
   1
```

It is true that integer numbers could also be represented using the floating point data type. However, integer arithmetic is used whenever it is possible as floating point arithmetic is well known to have accuracy problems.

Moreover, you must be careful when issuing division operations. Just as in the C language, a division between two interger numbers will not report any decimal digits:

```
>> 5 / 2
   2
>> 5.0 / 2
   2.5
```

Divisions by zero will also be caught and reported:

```
>> 5 / 2
   2
>> 5.0 / 2
   2.5
>> 5 / (1 - 1)
error[E4001]: catched a division by 0
 --> stdin : ln 57 : col 12
```

Naturally, you may also group expressions using parentheses:

```
>> 1 + 2 * 3
   7
>> (1 + 2) * 3
   9
```

And dangling parentheses will be reported as errors:

```
>> (1 + 2))
   3
error[E3002]: catched a dangling parenthesis
 --> stdin : ln 19 : col 9
```

### Basic functions

Delta also preloads some **basic mathematical functions**:

 * **Generic functions**:
   * `fabs()`: absolute value.

 * **Exponential functions**:
   * `exp()`: e raised to the given power.
   * `exp2()`: 2 raised to the given power.
   * `log()`: natural logarithm (base _e_).
   * `log2()`: binary logarithm (base _2_).
   * `log10()`: common logarithm (base _10_).
   
 * **Power functions**:
   * `cbrt()`: cubic root.
   * `sqrt()`: square root.

 * **Trigonometic functions**:
   * `acos()`: arccosine.
   * `asin()`: arcsine.
   * `atan()`: arctangent.
   * `cos()`: cosine.
   * `sin()`: sine.
   * `tan()`: tangent.

 * **Hyperbolic functions**:
   * `acosh()`: hyperbolic arccosine.
   * `asinh()`: hyperbolic arcsine.
   * `atanh()`: hyperbolic arctangent.
   * `cosh()`: hyperbolic cosine.
   * `sinh()`: hyperbolic sine.
   * `tanh()`: hyperbolic tangent.

These functions take exactly 1 argument, and return 1 floating point value:

```
>> sqrt(2)
   1.4142135623731
>> log(5.2)
   1.64865862558738
>> exp()
error[E4002]: incorrect amount of arguments, expected 1 args
 --> stdin : ln 23 : col 6
```

Beware that trigonometic functions expect that the input value is given in radians:

```
>> sin(3.14)
   0.00159265291648683
```

### Variables and constants

Delta can **store the values of computed expressions in variables**. In order to declare a variable, you just need to give it a name, as Delta uses dynamic typing. These names must start with a letter, and contain zero or more letters/digits/underscores:

```
>> a = 12
   12
>> b123_a = 23
   23
```

By default, variables are initialized to the integer value _0_:

```
>> b
   0
>> b + 1
   1
```

Which allows some exotic expressions such as the following one, even though they are discouraged due to how obscure they are:

```
>> c = d + 1
   1
```

In the previous snippet, both `c` and `d` are declared as new variables. `d` is given the default value, which results in `c = 0 + 1`.

Just as dynamic checking is used in mathematical operations, Delta also determines which data type each variable needs to use depending on the assigned value. For instance, if we have the two following variables:

```
>> a_floating_one = 23.56
   23.56
>> an_integer_one = 45
   45
```

The **`ws` (_Workspace Show_) built-in command can be used to list the registered variables, as well as their types**.

```
>> ws
   1. Mathematical constants:
      e => 2.718282
      pi => 3.141593
   2. Loaded mathematical functions:
      fabs
      exp
      exp2
      log
      log2
      log10
      cbrt
      sqrt
      acos
      asin
      atan
      cos
      sin
      tan
      acosh
      asinh
      atanh
      cosh
      sinh
      tanh
   3. Your variables:
      [floating] a_floating_one => 23.56
      [integer] an_integer_one => 45
```

Furthermore, as you may see in the previous snippet, Delta also **preloads some mathematical constants**:

- _e_.
- _π_.

You can use their values just as you would with any standard variable:

```
>> a = 2
   2
>> a * pi
   6.28318530717959
```

But trying to overwrite them will result in an error:

```
>> e = 1.2
error[E3001]: mathematical constants are inmutable
 --> stdin : ln 41 : col 2
```

### The WorkSpace

The **Delta WorkSpace, which can be shown using the `ws` built-in command**, as previously seen, **contains a handful of information** about the current session:

1. Which mathematical constants have been loaded.
2. Which mathematical funcions have been loaded.
3. Which variables have been defined.

All defined **variables can be cleared using the `wsc` (_WorkSpace Clear_) built-in command**:

```
>> pie = 3.14
   3.14
>> ws
   1. Mathematical constants:
      e => 2.718282
      pi => 3.141593
   2. Loaded mathematical functions:
      fabs
      <..>
      tanh
   3. Your variables:
      [floating] pie => 3.14
>> wsc
   Workspace successfully cleared
>> ws()
   1. Mathematical constants:
      e => 2.718282
      pi => 3.141593
   2. Loaded mathematical functions:
      fabs
      <..>
      tanh
   3. Your variables:
```

It is worth noting that commands that take no arguments can be called with or without `()`.

### Additional mathematical functions

Delta supports **dynamically loading external libraries and their functions**, in order to expand the repertoire of available mathematical functions.

For instance, Delta includes two small dynamic libraries just for testing purposes. One of them allows converting a value from degrees to radians, and the other does the opposite.

1. First of all, you need to **load the libraries using the `from()` built-in command** (if you do not type `./`, the library will be searched in the system's default path):

```
>> from("./libdl_radians.so")
   Library successfully loaded
>> from("./libdl_degrees.so")
   Library successfully loaded
```

2. Secondly, you need to remeber that the last loaded library is the one from which new functions will be loaded. Right now, `libdl_degrees.so` is the currently selected library. In order to select another library, you can just use the same `from` command. Do not worry, it will not load once again the library if it already is:

```
>> from("./libdl_radians.so")
   Library successfully selected
```

3. Now that `libdl_radians.so` has been set as the currently selected library, we can **load** the `radians()` **mathematical function** that can be found in it, **using the `import()` built-in command**:

```
>> ws
   1. Mathematical constants:
      e => 2.718282
      pi => 3.141593
   2. Loaded mathematical functions:
      fabs
      <..>
      tanh
   3. Your variables:
>> import("radians")
   Function successfully loaded
>> ws
   1. Mathematical constants:
      e => 2.718282
      pi => 3.141593
   2. Loaded mathematical functions:
      fabs
      <..>
      tanh
      radians
   3. Your variables:
```

4. And the function can now be called just as any other mathematical function:

```
>> radians(180)
   3.14159265358979
```

All loaded libraries are automatically closed by Delta upon exiting.


### Notebooks

A **Delta Notebook is just a text file that lists one or multiple Delta statements**. A Notebook can be **loaded using the `load()` built-in command**, and Delta will read it line by line, executing each statement that is found.

For instance, Delta includes three small Notebooks just for testing purposes. The first one is:

```
delta/delta/build$ cat ../test/notebooks/notebook_1.delta 
pie = 3.14
sin(degrees(pie))

```

Beware that Notebooks must end with a new line. Otherwise, Delta will not correctly read them.

Anyways, if we load that first Notebook, Delta will define a new variable `pie` and print its sine, after converting it to degrees using one of the previously shown test dynamic libraries:

```
>> from("./libdl_degrees.so")
   Library successfully loaded
>> import("degrees")
   Function successfully loaded
>> load("../test/notebooks/notebook_1.delta")
   File successfully loaded
   3.14
   -0.743284239769269
>> ws
   1. Mathematical constants:
      e => 2.718282
      pi => 3.141593
   2. Loaded mathematical functions:
      fabs
      <..>
      tanh
      degrees
   3. Your variables:
      [floating] pie => 3.14
```

It is worth noting that the read statements are not printed, but just executed and their results shown.

Furthermore, **Delta supports nested NoteBooks**; that is, Notebooks that issue a `load` on another Notebooks.

For instance, the second test Notebook is as follows:

```
delta/delta/build$ cat ../test/notebooks/notebook_2.delta 
hello_from_notebook_2 = 1
load("../test/notebooks/notebook_3.delta")
result = hello_from_notebook_2 + hello_from_notebook_3

```

This Notebook will initialize the variable `hello_from_notebook_2`, load the third Notebook, and initialize the variable `result` to the addition of the previous variable and another one that is defined in that third Notebook:

```
delta/delta/build$ cat ../test/notebooks/notebook_3.delta 
hello_from_notebook_3 = 1
(123))

```

Non only that, but the third Notebook will produce an error in order to show that Delta details where each error is found, regarding source file (or stdin), line and column:

```
>> load("../test/notebooks/notebook_2.delta")
   File successfully loaded
   1
   File successfully loaded
   1
   123
error[E3002]: catched a dangling parenthesis
 --> ../test/notebooks/notebook_3.delta : ln 2 : col 7

   2
>> ws
   1. Mathematical constants:
      e => 2.718282
      pi => 3.141593
   2. Loaded mathematical functions:
      fabs
      <..>
      tanh
   3. Your variables:
      [integer] hello_from_notebook_2 => 1
      [integer] hello_from_notebook_3 => 1
      [integer] result => 2
```

### Switching echo on and off

After each recognized mathematical expression, Delta outputs its value. You can disable this behaviour just by adding a semicolon at the end of the line:

```
>> 1 + 2
   3
>> 1 + 2;
>> value = sin(3);
>> ws
   1. Mathematical constants:
      e => 2.718282
      pi => 3.141593
   2. Loaded mathematical functions:
      fabs
      <..>
      tanh
   3. Your variables:
      [floating] value => 0.1411200081
```


### Requesting help


### Exiting Delta

Finally, you may **exit Delta just by using the `quit` built-in command**. As previously said, Delta will take care by itself of closing any dynamically loaded libraries.


## Getting started

In order to try out this interpreter, start by **downloading or cloning this repository**.

### Prerequisites

You will need to have a **development environment that supports the following technologies**:

* GNU C Compiler.
* CMake 3.0 or greater.
* The Fast Lexical Analyzer (flex).
* GNU Bison.

Any recent Linux distribution should support them. You may have to manually install CMake, flex, or Bison. On Ubuntu, you would just need to run:

```
apt update
apt install cmake flex bison
```

Specifically, this project was tested under:

* Ubuntu 20.04
* GNU C Compiler 9.3.0
* CMake 3.16.3
* flex 2.6.4
* Bison 3.5.1

### Compiling the project

**CMake is the utility that will manage all the compilation process**. It is recommended that you create a directory dedicated to all the compilation resources:

```
cd delta
mkdir build && cd build
```

Now, you may request to CMake to generate all resources required to build the project:

```
cmake ..
```

By issuing this command, CMake reads the `CMakeLists.txt` file located in the parent directory. This file specifies all source and header files of Delta, as well as some desired properties that will be applied during compilation.

The actual compilation process of the project is started by running:

```
cmake --build .
```

**And the Delta's executable file will be generated right in the `build` directory** (the current one). It will also compile the two toy dynamic libraries that are intended for testing purposes.

It is worth noting that both flex's specification file (`src/analyzers/lexical.l`) and Bison's one (`src/analyzers/synsem.y`) must be translated to actual source files in order to use them alongside Delta. CMake also takes care of this translation process. In fact, you may find these generated source files in your build directory (`build/lexical.c` and `build/synsem.c`) after issuing the compilation process through CMake.

### Regarding any warnings that show up during compilation

Due to the warning-related compiler flags, GCC will report non harmful warnings, which come from the source file that flex generates. Specifically, the following warnings should show up:

* _warning: "YY_BUF_SIZE" redefined_
    * flex uses an I/O buffer size of 16 KB by default, which has been reduced to 4 KB. That is where the redefinition comes from.

* _warning: ‘input’ defined but not used [-Wunused-function]_
* _warning: ‘yyunput’ defined but not used [-Wunused-function]_
    * These are two functions that are not needed by the lexical analyzer, but that flex generates by itself. That is why they are defined anyways.

Moreover, also due to the warning-related flags, Bison will report another non harmful warning when translating the `src/analyzers/synsem.y` file:

* *warning: useless associativity for D_LC_OP_ASSIGNMENT_ASSIGN, use %precedence [-Wprecedence]*
    * This associativity parameter is actually useful as it avoids some shift/reduce conflicts when generating the syntactic and semantic analyzer. That is why it has been left anyways.

Therefore, **you may just ignore these warnings without any problems**.



### Running Delta

Once compiled, you can **launch Delta just like any common executable**:

```
./delta
```

And its prompt will be shown, ready to take any instructions from the user:

```
>>
```

## Future work

The following features were intented to be added to Delta, but have been left out due to time constraints:

* **Binary operators:** They could directly work on integer numbers, or a new data type for them could also be implemented.
* **Vectors:** Both mathematical functions that operate on multiple numbers at a time, and a new data type to represent them.

However, just for the sake of learning, it would certainly be nice to pick up this project anytime in the future, and extend it by adding these features. For instance, they would certainly allow deepening into the dynamic checking features that are present nowadays in language interpreters.

## Built With

* [C 99]() - A good ol' trusty language.
* [CMake](https://cmake.org/) - Which eases the portability accross different systems.
* [flex](https://github.com/westes/flex) - To assist in building the lexical analyzer.
* [Bison](https://www.gnu.org/software/bison/) - To assist in building the syntactic and semantic analyzer.
* [uthash](https://troydhanson.github.io/uthash/) - A handy hast table ready to be used in C.

## Authors

* **Álvaro Goldar Dieste** - [alvrogd](https://github.com/alvrogd)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
