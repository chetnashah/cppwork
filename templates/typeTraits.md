They are a meta programming technique which appears to use types and their sub types like functions at compile time, to control what the compiler actually compiles.

## Useful for type level programming

### Why are structs useful?

Structs are useful in C++ for type-level programming because they can hold compile time constant values as data members with `static constexpr` members like `static constexpr bool value` accessibile as `is_integral::value` or `is_same_type::value`.

### `::value` convention (or _v convention, e.g. `std::is_integral_v<int> == true`)

::value (Compile-Time Constants)
When a template computes a compile-time value (e.g., int, bool), it typically stores the result in a static member constant named value.

Example: Factorial Metafunction

```cpp
template<int N>
struct factorial {
    static const int value = N * factorial<N-1>::value;
};
// base case is usually a specialization
template<>
struct factorial<0> {
    static const int value = 1;
};

int x = factorial<5>::value;  // x = 120
```
Here, value is a compile-time constant. This pattern was critical for enabling SFINAE-based checks (e.g., `std::is_integral<T>::value`)

**Note** - Legacy code still uses `::type`/`::value` for backward compatibility, but modern TMP prefers aliases and constexpr.

### Need for `::type` or `_t` convention 

Here is an example of use of `_t` convention:

```cpp
static_assert(std::is_same_v<int, 
    std::remove_pointer_t<int*>> == true // both are same
);
```

Usually needed in type transformation or type traits, where you want to extract a type from a template. or strip away const/volatile qualifiers.



### Example: A Metafunction to Add a Pointer to a Type
```cpp
#include <iostream>
#include <type_traits>

// Define a metafunction that adds a pointer to a type
template <typename T>
struct add_pointer {
    using type = T*; // The resulting type is T*
};

int main() {
    // Using the metafunction to declare a variable
    add_pointer<int>::type x = nullptr; // x is of type "int*"
    
    // Static assertion to verify the type
    static_assert(std::is_same<decltype(x), int*>::value, "Type mismatch!");

    // Output "1" (true) to confirm x is a pointer
    std::cout << std::is_pointer<decltype(x)>::value << std::endl;
    return 0;
}
```

### Key Points:
1. **Metafunction Definition**:  
   The `add_pointer` template is a metafunction that transforms a type `T` into `T*` by defining a nested alias `type = T*` [en.wikibooks.org](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Metafunction).

2. **Accessing the Result**:  
   The `::type` syntax retrieves the computed type. For `add_pointer<int>::type`, this resolves to `int*`.

3. **When `typename` is Needed**:  
   Inside a template, if the type depends on a template parameter, you must use `typename` to tell the compiler `::type` refers to a type. For example:
   ```cpp
   template <typename T>
   void example() {
       typename add_pointer<T>::type ptr; // "typename" required here
   }
   ```
   This avoids ambiguity, as explained in the [StackOverflow answer](https://stackoverflow.com/questions/40420352/how-to-get-a-type-from-other-type-c-template-metaprogramming).

This pattern is foundational in libraries like Boost.MPL and the C++ Standard Library for compile-time type transformations.

### Implement `is_same<T, U>` type level function

```cpp
// (A)
template <typename T, typename U>
struct is_same_type
{
    static constexpr bool value {false};
};

// (B)
template <typename T>
struct is_same_type<T, T>
{
    static constexpr bool value {true};
};
// Usage example
static_assert(is_same_type<int, char>::value == false, "");
static_assert(is_same_type<int, int >::value == true,  "");
```

### Implement `not<Bool X>` type level function

Remember we are still doing compile time programming!

Let's try to implement `not<T>` which is a type level function that negates a boolean type.

```cpp
// X is a compile time constant boolean value - true/false
template <bool X>
struct not
{
    // statically expose negation of compile time constant boolean value X passed
    // e.g. not<true>::value == false
    static constexpr bool value {!X};
};
```
### Implementation with template specialization
```cpp
// (A)
template <bool X>
struct not
{
    static constexpr bool value {false};
};

// (B)
template <>
struct not<false>
{
    static constexpr bool value {true};
};
```