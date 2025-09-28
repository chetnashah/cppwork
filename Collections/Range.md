## Iterability: 
Any type for which `std::ranges::begin()` and `std::ranges::end()` can be called successfully is considered a range. This includes standard library containers like `std::vector`, `std::list`, `std::string`, and even C-style arrays.

## Range-based for loop
Range-based for loop (C++11 onwards): This syntax provides a convenient way to iterate over ranges:
```Cpp
    for (for-range-declaration : expression) {
        // statement
    }
```

## `ranges(begin/end)` vs begin()/end()

`std::ranges::begin(v)` also works, and in the case of vector usually ends up calling `vec.begin()`, since `std::vector` satisfies the needed concepts and is a borrowed range etc. So for vector, in normal situations, all of these give essentially the same iterator (same type, same behavior) as long as you're calling on an lvalue vector.