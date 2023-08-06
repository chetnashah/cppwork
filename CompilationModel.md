
https://www.youtube.com/watch?v=0kgTuWkyorc

## Preprocessing

Precprocessor does three text transformations steps:
1. header inclusion (`#include`)
2. macro substitution (`#define`)
3. conditional compilation (`#if`, `#ifdef`, `#ifndef`, `#else`, `#endif`)

**Translation Unit (.i files)** - The output of the pre-processor is called a **Translation Unit**. It is the input to the compiler. Some compilers have an option to generate these files as `.i` or `.ii` files. So the pipeline is `.h + .cpp` -> `.i` -> `.o` -> `.exe`. It is useful to see `.i` files to debug pre-processor issues.


`.i` file: A file that has already been pre-processed and ready for compilation.

### Why header files?

The true purpose of a header file is to share code amongst multiple source files. It is commonly used to separate declarations from implementations for better code management, but that is not a requirement. It is possible to write code that does not rely on header files, and it is possible to write code that is made up of just header files (the STL and Boost libraries are good examples of that). 

Remember, when the preprocessor encounters an #include statement, it replaces the statement with the contents of the file being referenced, then the compiler only sees the completed pre-processed code.



Notion of separate compilation where user code only sees declarations
of the `types`(classes for example) and `functions` used.
The definitions of those types and functions are in separate source files and compiled separately.

A library is often a collection of separately compiled code fragments.

### Usage of .h and .cpp

`.h` exposes interface generally and `.cpp` of same name will have implementation.

To help ensure consistency, `.cpp` file providing implementation of `types and functions` in `.h` will also include `.h` file,
and also the consumers of these types and functions will also use same `.h` header files for interface 

## Best practices

### Always import header files that declare the types and functions you use

Do not rely on transitive inclusion for the declarations that you use.