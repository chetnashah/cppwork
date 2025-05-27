
## Problem - cryptic error message when trying to modify a `const` vector

explain this cpp error in the program:

```
<source>:6:16: error: passing 'const std::vector<int>' as 'this' argument discards qualifiers [-fpermissive]
    6 |     v.push_back(5);
      |     ~~~~~~~~~~~^~~
In file included from /opt/compiler-explorer/gcc-15.1.0/include/c++/15.1.0/vector:68,
                 from <source>:2:
/opt/compiler-explorer/gcc-15.1.0/include/c++/15.1.0/bits/stl_vector.h:1433:7: note:   in call to 'void std::vector<_Tp, _Alloc>::push_back(value_type&&) [with _Tp = int; _Alloc = std::allocator<int>; value_type = int]'
 1433 |       push_back(value_type&& __x)
      |       ^~~~~~~~~
```

Program:

```cpp
#include <iostream>
#include<vector>

int main() {
    const std::vector<int> v = {1,2,3};
    v.push_back(5);
}
```

Why is the error message so cryptic and how to make sense of it usually?

## The Core Problem

You declared `v` as `const`, but you're trying to call `push_back()` which modifies the vector. This violates const-correctness.

## Decoding the Error Message

**"passing 'const std::vector<int>' as 'this' argument discards qualifiers"**

- **'this' argument**: In C++, when you call `v.push_back(5)`, it's equivalent to calling `push_back(5)` with `v` as the implicit `this` pointer
- **discards qualifiers**: "qualifiers" refers to `const`, `volatile`, etc. The error means you're trying to pass a `const` object to a method that expects a non-const object
- **discards**: The `const` qualifier would be "discarded" (ignored) if this call were allowed

## Why the Error Message Seems Cryptic

C++ error messages can be confusing because:

1. **Implementation details exposed**: The error shows internal compiler mechanics (like the `this` pointer concept)
2. **Template complexity**: The full template signature is shown: `std::vector<_Tp, _Alloc>::push_back(value_type&&)`  
3. **Multiple locations**: It shows both where the error occurred and where the conflicting declaration is

## How to Read C++ Error Messages Generally

1. **Start with the first line**: `error: passing 'const std::vector<int>' as 'this' argument discards qualifiers`
2. **Identify the operation**: Look for what you're trying to do (`push_back`)
3. **Look for const/mutable conflicts**: Words like "discards qualifiers", "const", "mutable"
4. **Ignore template noise**: The `_Tp`, `_Alloc` parts are usually not important for understanding the core issue

## The Fix

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {1,2,3};  // Remove 'const'
    v.push_back(5);
    
    // Or if you need it const after initialization:
    // std::vector<int> temp = {1,2,3};
    // temp.push_back(5);
    // const std::vector<int> v = std::move(temp);
}
```

## Pattern Recognition

When you see "discards qualifiers" in C++ errors, it almost always means:
- You're calling a non-const method on a const object
- You're assigning a const object to a non-const reference  
- You're passing a const object where a mutable one is expected

The key insight is that `push_back()` is a **mutating operation** - it changes the vector's state, so it cannot be called on a `const` vector.