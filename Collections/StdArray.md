## `std::array` (introduced in C++11, different from C-style arrays)

The `std::array` class in C++ was introduced in C++11, which was released in 2011.

Prior to C++11, there was no standard container in C++ that provided a fixed-size array with a standardized interface. Developers had to use built-in C-style arrays, which lacked certain features and convenience methods that are available with the `std::array` class.

The `std::array` class was introduced as part of the C++11 standard library, providing the following key features:

1. **Fixed Size**: `std::array` has a fixed size that is part of its type, which is determined at compile-time. This allows the compiler to perform optimizations and provide better type safety compared to using C-style arrays.

2. **Standard Interface**: `std::array` provides a standard set of member functions and operations, such as `at()`, `front()`, `back()`, `data()`, and iteration methods, making it more convenient to work with than raw C-style arrays.

3. **Compatibility with Standard Algorithms**: `std::array` can be used with the standard algorithms in the C++ Standard Template Library (STL), such as `std::sort()`, `std::find()`, and `std::accumulate()`.

4. **Compatibility with C-style Arrays**: `std::array` is designed to be compatible with C-style arrays, allowing you to pass a `std::array` to functions that expect a C-style array, and vice versa.

Since its introduction in C++11, the `std::array` class has become a widely used and recommended alternative to C-style arrays in modern C++ code, particularly when working with fixed-size collections of elements.

### Definition

```cpp
template<
    class T, // array element type 
    std::size_t N // array size
> struct array;
```
i.e.
```cpp
// type parameters specify element type and size
std::array<int, 3> a = {1, 2, 3};.
```