
## Use __PRETTY_FUNCTION__ for debugging and logging

## What kind of templates are there?
- **Function Templates**: Allow functions to operate with generic types.
- **Class Templates**: Allow classes to operate with generic types.
- **Variable Templates**: Allow variables to be defined with generic types.
- **Alias Templates**: Allow type aliases to be defined with generic types.

## If template parameters are passed explicitly, compiler does not deduce them.

```cpp
#include <iostream>

template<typename T, typename U>
void add(T a, U b) {
  puts(__PRETTY_FUNCTION__);
}

int main() {
	// see how explicit template type parameters override type deduction
    add<int, int> ('x', 3.1);// void add(T, U) [with T = int; U = int]
  	add<int>('x', 3.1);      // void add(T, U) [with T = int; U = double]
  	add<>('x', 3.1);         // void add(T, U) [with T = char; U = double]
  	add('x', 3.1);           // void add(T, U) [with T = char; U = double]
    
    return 0;
}
```

## Feature list

# C++ Template Programming Features Matrix

Here's a comprehensive table of C++ template programming features:

| Feature / Template Type | Function Template | Class Template | Variable Template | Alias Template |
|------------------------|-------------------|----------------|-------------------|----------------|
| **Type Deduction** | ✅ Yes (auto parameters, return type) | ✅ Yes (C++17 CTAD) | ✅ Yes (auto) | ❌ No |
| **Full Specialization** | ✅ Yes | ✅ Yes | ✅ Yes | ❌ No |
| **Partial Specialization** | ❌ No | ✅ Yes | ✅ Yes (C++14) | ❌ No |
| **Variadic Templates** | ✅ Yes | ✅ Yes | ✅ Yes (C++14) | ✅ Yes |
| **Template Template Parameters** | ✅ Yes | ✅ Yes | ✅ Yes (C++14) | ✅ Yes |
| **Default Template Arguments** | ✅ Yes | ✅ Yes | ✅ Yes (C++14) | ✅ Yes |
| **Non-Type Template Parameters** | ✅ Yes | ✅ Yes | ✅ Yes (C++14) | ✅ Yes |
| **Concepts/Constraints (C++20)** | ✅ Yes | ✅ Yes | ✅ Yes | ✅ Yes |
| **SFINAE** | ✅ Yes | ✅ Yes | ✅ Yes | ✅ Yes |
| **`if constexpr` (C++17)** | ✅ Yes (in body) | ✅ Yes (in methods) | ❌ N/A | ❌ N/A |
| **Friend Templates** | ✅ Yes | ✅ Yes | ❌ N/A | ❌ N/A |
| **Member Templates** | ❌ N/A | ✅ Yes | ❌ N/A | ❌ N/A |
| **Nested Templates** | ❌ No | ✅ Yes | ❌ No | ❌ No |
| **Explicit Instantiation** | ✅ Yes | ✅ Yes | ✅ Yes (C++14) | ❌ N/A |
| **`auto` Return Type** | ✅ Yes | ❌ N/A | ❌ N/A | ❌ N/A |

### Notes:
- **Variable templates** were introduced in C++14
- **Class Template Argument Deduction (CTAD)** was introduced in C++17
- **Concepts** were introduced in C++20
- Alias templates cannot be specialized (they are transparent aliases)
- Function templates cannot be partially specialized (overloading is used instead)
- Some features marked as N/A are not applicable due to the nature of the template type