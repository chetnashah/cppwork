
## std::move() does not move; it only sets a temporary marker

std::move() is nothing but a static_cast to an rvalue reference. You can achieve the same effect by
calling static_cast manually as follows:
```cpp
foo(static_cast<decltype(obj)&&>(obj)); // same effect as foo(std::move(obj))
```
## std::move does not work on const objects

## reading move signature

```cpp
void foo1(std::string&& rv); // binds to the passed object and might steal/modify the value

std::string s{"hello"};
...
foo1(s); // calls the first foo1(), s keeps its value
foo1(std::move(s)); // calls the second foo1(), s might lose its value
```

**Move Constructors and Move Assignment Operators Require Modification**:
1. Move operations typically modify the source object (e.g., by nullifying pointers or transferring ownership of resources). Move constructors are generally defined to take T&&, not const T&&, because they need to modify the source (to nullify its pointers).
2. Since const objects cannot be modified, the compiler prevents move constructors from being invoked on them.
3. As a result, copy constructors are invoked instead of move constructors when dealing with const objects.

e.g
```cpp
#include <iostream>
#include <string>
#include <utility>

int main() {
    const std::string str1 = "Hello, World!";
    std::string str2 = std::move(str1); // Attempt to move from a const object

    std::cout << "str1: " << str1 << "\n"; // Remains unchanged
    std::cout << "str2: " << str2 << "\n"; // Contains a copy of "Hello, World!"

    return 0;
}
```

## std::move summary

1. Move semantics allows us to optimize the copying of objects, where we no longer need the value. It can beused implicitly (for unnamed temporary objects or local return values) or explicitly (with std::move()).
2.  std::move() means I no longer need this value here. It marks the object as movable. An object marked with std::move() is not (partially) destroyed (the destructor still will be called).
3.  By declaring a function with a non-const rvalue reference (such as std::string&&), you define an
interface where the caller semantically claims that it no longer needs the passed value. The implementer of the function can use this information to optimize its task by “stealing” the value or do any other modification with the passed argument. Usually, the implementer also has to ensure that the passed argument is in a valid state after the call.
4. Moved-from objects of the C++ standard library are still valid objects, but you no longer know their
value.
5. Copy semantics is used as a fallback for move semantics (if copy semantics is supported). If there is no implementation taking an rvalue reference, any implementation taking an ordinary const lvalue reference (such as const std::string&) is used. This fallback is then used even if the object is explicitly marked
with std::move().
6. Calling std::move() for a const object usually has no effect.
7. If you return by value (not by reference), do not declare the return value as a whole to be const.