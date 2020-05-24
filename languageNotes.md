
### One definition rule

File level `ODR`: Only one definition of any variable, function, class type, enumeration type, concept (since C++20) or template is allowed in any one translation unit (file).

Program level `ODR`: One and only one definition of every non-inline function or variable that is odr-used (see below) is required to appear in the entire program (including any standard and user-defined libraries).

Types (which include `classes`), are exempt from the part of the one-definition rule that says you can only have one definition per program. Therefore, there isn’t an issue #including class definitions into multiple code files (if there was, classes wouldn’t be of much use).
Or you can say that:
If a program contains more than one definition of a `class` (and also of an inline function, or a function template) with external linkage, all these definitions should be identical.
That is why it is ok to include headers that contain class definitions.

This is exactly what happens when you put the definition of your class in a header file and then include this header in two .cpp files. Because of how the header inclusion works in C++ (by copy-and-paste), this renders the situation where the two .cpp files define the same class. There is no other way in C++ to share class definitions between files; therefore the compiler must accept the repeated definition of a given class in different files, but on provision that you do not try to do nasty tricks, like changing the definition of the same class from file to file. You are only expected to include the same header file in each .cpp file. Anything other than that is risky.

https://stackoverflow.com/questions/4955159/is-is-a-good-practice-to-put-the-definition-of-c-classes-into-the-header-file

### uninintialized values

A variable that has not been given a known value (usually through initialization or assignment) is called an uninitialized variable.

Value of `uninitialized variables is undefined and implementation dependent`.
Visual studio: it will consistently print -858993460, because that’s the value (interpreted as an integer) that Visual Studio initializes memory with in debug configurations

Most modern compilers will attempt to detect if a variable is being used without being given a value. If they are able to detect this, they will generally issue a compile-time error.

### Forward declaration.

A forward declaration allows us to tell the compiler about the existence of an identifier before actually defining the identifier.

In the case of functions, this allows us to tell the compiler about the existence of a function before we define the function’s body. This way, when the compiler encounters a call to the function, it’ll understand that we’re making a function call, and can check to ensure we’re calling the function correctly, even if it doesn’t yet know how or where the function is defined.

```cpp
#include <iostream>
 
int add(int x, int y); // forward declaration of add() (using a function prototype)
 
int main()
{
    std::cout << "The sum of 3 and 4 is: " << add(3, 4) << '\n'; // this works because we forward declared add() above
    return 0;
}
 
int add(int x, int y) // even though the body of add() isn't defined until here
{
    return x + y;
}
```

Declaring two functions differing only in return type is not valid function overloading, and is flagged as a compile-time error
```cpp
void f();
int f();      // error, two definitions differ only in
              // return type
int g()
{
   return f();
}
```

### Determining cpp version in code

It is captured in `__cplusplus` macro by default.

```cpp
int main() {
    if (__cplusplus == 201703L) std::cout << "C++17\n";
    else if (__cplusplus == 201402L) std::cout << "C++14\n";
    else if (__cplusplus == 201103L) std::cout << "C++11\n";
    else if (__cplusplus == 199711L) std::cout << "C++98\n";
    else std::cout << "pre-standard C++\n";
}
```

### Compiler Extensions

The C++ standard defines rules about how programs should behave in specific circumstances. And in most cases, compilers will follow these rules. However, many compilers implement their own changes to the language, often to enhance compatibility with other versions of the language (e.g. C99), or for historical reasons. These compiler-specific behaviors are called compiler extensions.

Writing a program that makes use of a compiler extension allows you to write programs that are incompatible with the C++ standard. Programs using non-standard extensions generally will not compile on other compilers (that don’t support those same extensions), or if they do, they may not run correctly.

#### Expressions

C++ expressions can cause side effects e.g. `printf("x")`
Each C++ expression is charachterized by two independent properties:
a type and a value category.

Value categories(lvalue, rvalue, glvalue, prvalue, xvalue) classify
expressions by their value. (http://www.stroustrup.com/terminology.pdf)

##### Simplistic early definitions:

An lvalue (locator value) represents an object that occupies some identifiable location in memory (i.e. has an address).

rvalues are defined by exclusion, by saying that every expression is either an lvalue or an rvalue. Therefore, from the above definition of lvalue, an rvalue is an expression that does not represent an object occupying some identifiable location in memory.

##### New definitions

http://en.cppreference.com/w/cpp/language/value_category
Each expression has some non-reference type, and each expression belongs
to exactly one of three primary value categories: prvalue, xvalue and lvalue.

1. glvalue (generalized lvalue) is an expression whose evaluation determines identity of object, bit-field or function.

2. a prvalue (pure rvalue) is an expression whose evaluation either:
  - computes value of the operand of an operator
  - initializes an object or a bit-field
  - can be moved

3. an xvalue is a glvalue that denotes an object or bit-field whose resources can be reused.

* an lvalue is a glvalue that is not an xvalue.
* an rvalue is a prvalue or an xvalue.

Examples of lvalues:
* name of variable, function, or data member.
* a string literal like "hello world"

Examples of prvalue:
* a literal (except string literal) such as 42, true or nullptr

### Internal linkage

Global variables with internal linkage are sometimes called internal variables.
However, it’s worth noting that internal objects (and functions) that are defined in different files are considered to be independent entities (even if their names and types are identical), so there is no violation of the one-definition rule. Each internal object only has one definition.


```cpp
static int g_x; // non-const clobals have external linkage, but if you add static, they are given internal linkage

const int g_y {1}; // const globals have internal linkage by default.

constexpr int g_z {2}; // constexpr globals have internal linkage by default
```

Functions default to external linkage, but if used with `static` keyword, will have internal linkage
```cpp
//add.cpp
static int add(int x, int y) {
    return x + y;
}
```
```cpp
// main.cpp
#include iostream
int add(int x, int y); // forward declaration for fn
int main(){
    std::cout << add(3, 4) << std::endl;
    return 0;
}
```

### External linkage

An identifier with external linkage can be seen and used both from the file in which it is defined, and from other code files (via a `forward declaration`). In this sense, identifiers with external linkage are truly `global` in that they can be used anywhere in your program!

`constexpr` variables can’t be forward declared, even if they have external linkage

Functions have external linkage by default.
In order to call a function defined in another file, you must place a forward declaration for the function in any other files wishing to use the function. The forward declaration tells the compiler about the existence of the function, and the linker connects the function calls to the actual function definition. No need of `extern` keyword for function forward declarations.

```cpp
// a.cpp
#include <iostream>
void sayHi() {
    std::cout << "Hi";
}
```
```cpp
void sayHi(); // forward declaration makes sayHi accessible in this file
int main(){
    sayHi();
    return 0;
}
```

For variables to have external linkage, use `extern` keyword in front of a global variable in it's forward declaration.
`extern` with definition makes it eligible for export (but it is optional)
`extern` without definition is equivalent to import/forward declaration for usage.

```cpp
// a.cpp
int g_x {2}; // non-const globals have external linkage by default
extern const int g_y { 3}; // this gives const g_y an external linkage, required for const
```

```cpp
// main.cpp
#include <iostream>
extern int g_x;
extern const int g_y;

int main(){
    std::cout << g_x;
    return 0;
}
```

### Initialization order problem of global variables

Initialization of global variables happens as part of program startup,
before execution of main function.

It has two phases:
1. static initialization: Initialize global variables with constexpr initializers, global vars without initializers are zero initialized.

2. dynamic initialization: global vars with non-constexpr initializers are initialized.

Dynamic initialization of global variables causes a lot of problems in C++. Avoid it whenever possible.

### Common way to manage constants in app

```cpp
// constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H
 
// define your own namespace to hold constants
namespace constants
{
    // constants have internal linkage by default
    constexpr double pi { 3.14159 };
    constexpr double avogadro { 6.0221413e23 };
    constexpr double my_gravity { 9.2 }; // m/s^2 -- gravity is light on this planet
    // ... other related constants
}
#endif
```

### When to put code in `.h` vs `.cpp`

Code in `.h` is blindly copied into files. It will be exected any number of times, the copied destination executes. 
So `.h` usually only has forward declarations (promise of availability of implementation)

Code in `.cpp` is instantiated once (implementation available once)

the common way is and always has been to put code in `.cpp` files (or whatever extension you like) and declarations in `headers`


### Classes

https://stackoverflow.com/questions/4955159/is-is-a-good-practice-to-put-the-definition-of-c-classes-into-the-header-file

A function defined within a class definition is an inline function.

