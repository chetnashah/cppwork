
## What are template parameter packs?

A template parameter pack is a special kind of template parameter that can represent zero or more template arguments. It acts like a placeholder for a list of types, non-types, or even other templates.

Purpose:

1. To create functions that **accept any number of arguments** (like printf, but type-safe).
2. To create data structures that can **hold a collection of heterogeneous types (like std::tuple)**.
3. To enable generic programming **patterns like perfect forwarding for any number of arguments.**

## Syntax
```cpp
template <typename... Args> // Args is a type parameter pack
void myFunction(Args... args) { // args is a function parameter pack
    // Args... args means: type1 arg1, type2 arg2, type3 arg3, ...
    // Here, args... expands the function parameters
}
```


## Using Template Parameter Packs

```cpp
#include <iostream>

// Base case: called when the parameter pack is empty
void printAll() {
    std::cout << std::endl; // End with a newline
}

// Recursive step
template <typename T, typename... Rest> // destructuring the parameter pack
void printAll(const T& first, const Rest&... rest) {
    std::cout << first;
    if constexpr (sizeof...(Rest) > 0) { // C++17: if constexpr for compile-time check
        std::cout << ", ";
    }
    printAll(rest...); // Recursive call with the rest of the arguments
}

int main() {
    printAll("Hello", 42, 3.14, 'A'); // Output: Hello, 42, 3.14, A
    printAll(100);                     // Output: 100
    printAll();                        // Output: (newline)
    return 0;
}
```
