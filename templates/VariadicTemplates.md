
## What are template parameter packs?

A template parameter pack is a special kind of template parameter that can represent zero or more template arguments. It acts like a placeholder for a list of types, non-types, or even other templates.

Purpose:

1. To create functions that **accept any number of arguments** (like printf, but type-safe).
2. To create data structures that can **hold a collection of heterogeneous types (like std::tuple)**.
3. To enable generic programming **patterns like perfect forwarding for any number of arguments.**

There are two kinds of parameter packs in C++:
1. **Type parameter packs**: Used in template definitions to accept multiple types.
2. **Function parameter packs**: Used in function definitions to accept multiple arguments.

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
## Recursion with Template Parameter Packs

```cpp
#include <iostream>
using namespace std::string_literals;

template <typename T>                 // [1] overload with fixed
T add(T value)                        //     number of arguments
{
  return value;
}
template <typename T, typename... Ts> // [2] typename... Ts
T add(T head, Ts... rest)             // [3] Ts... rest
{
  return head + add(rest...);         // [4] rest...
}

int main() {
    int k = add(1,2,3,4);
    std::string ss = add("asdf"s, " "s, "dsf"s);
    std::cout << k << std::endl;
    std::cout << ss << std::endl;
}
```

## sizeof... operator

The `sizeof...` operator in C++ is a compile-time operator used **with variadic templates**. It allows you to determine the **number of arguments** in a parameter pack.

---

## **What is a Parameter Pack?**
A parameter pack is a feature of variadic templates that lets you accept an arbitrary number of template arguments or function arguments.

---

## **Syntax**
```cpp
sizeof...(parameter_pack)
```
- `parameter_pack` can be either a template parameter pack or a function parameter pack.

---

## **What Does It Return?**
- It returns the **count of arguments** in the pack, as a constant `std::size_t` value.

---

## **Example 1: Template Parameter Pack**
```cpp
#include <iostream>

template<typename... Args>
void printCount(Args... args) {
    std::cout << "Number of arguments: " << sizeof...(Args) << '\n';
    std::cout << "Number of function arguments: " << sizeof...(args) << '\n';
}

int main() {
    printCount(1, 2.5, "hello");
    printCount('a');
}
```

**Output:**
```
Number of arguments: 3
Number of function arguments: 3
Number of arguments: 1
Number of function arguments: 1
```

- `sizeof...(Args)` counts the number of template type arguments.
- `sizeof...(args)` counts the number of function arguments.

---

## **Example 2: Usage in Static Assertions**
```cpp
template<typename... Ts>
struct Count {
    static_assert(sizeof...(Ts) > 0, "At least one type required!");
};

Count<int, double> ok; // OK
// Count<> fail; // Error: At least one type required!
```

---

## **Summary Table**

| Usage             | Returns                          |
|-------------------|----------------------------------|
| `sizeof...(Types)`| Number of types in parameter pack|
| `sizeof...(args)` | Number of function arguments     |

---

## **Key Points**
- **Only works with parameter packs.**
- **Evaluated at compile time.**
- **Returns a `std::size_t` constant.**

---

**In short:**  
`sizeof...` is a handy tool for working with variadic templates, letting you count the number of types or values passed in a pack.