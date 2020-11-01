# Delta

Delta is a work-in-progress **compiler for the D language** written in C.

## Features

This project focuses on being able to compile a subset of the D language, so that is helps its author in learning about compiler technologies.

## Getting started

In order to try out this compiler, start by **downloading or cloning this repository**.

### Prerequisites

You will need to have a **development environment that supports the following technologies**:

* GNU C Compiler.
* CMake 3.0 or greater.

Any recent Linux distribution should support them out of the box. If anything, you may have to manually install CMake. On Ubuntu, you would just need to run:

```
apt update
apt install cmake
```

### Compiling the project

CMake is the utility that will manage all the compilation process. It is recommended that you create a directory dedicated to all the compilation resources:

```
cd delta
mkdir build && cd build
```

Now, you may request to CMake to generate all resources required to build the project:

```
cmake ..
```

By issuing this command, CMake reads the `CMakeLists.txt` file located in the parent directory. This file specifies all source and header files of the compiler, as well as some desired properties that will be applied during compilation.

The actual compilation process of the project is started by running:

```
cmake --build .
```

And the Delta's executable file will be generated right in the `build` directory (the current one).

### Regarding any warnings that show up during compilation

Due to the warning-related compiler flags, GCC will report non harmful warnings; specifically, _unused parameter_ warnings. These unused parameters have been left this way on purpose, in order to define more comprehensive function interfaces that would ease the task of extending its module's functionality in the future.

Therefore, you may just ignore this kind of warnings without any problems.

### Running Delta

Once compiled, you can use Delta just like any other compiler:

```
./delta <source file>
```

As previosly said, this project is focused on compiling a subset of the D language. In particular, Delta will be able to parse any of the source files that can be found in `delta/test/source_files`.

The only module of the compiler that has been implemented for the time being is the lexical analyzer. Therefore, Delta will just output every lexical component that it can recognize in the given source file.

There are definitely more modules cooming soon, so stay tuned!

## Built With

* [C 99]() - A good ol' trusty language.
* [CMake](https://cmake.org/) - Which eases the portability accross different systems.
* [uthash](https://troydhanson.github.io/uthash/) - A handy hast table ready to be used in C.

## Authors

* **Álvaro Goldar Dieste** - [alvrogd](https://github.com/alvrogd)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
