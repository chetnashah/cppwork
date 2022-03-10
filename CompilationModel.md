
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
