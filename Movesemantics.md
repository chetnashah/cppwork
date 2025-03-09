
## std::move() does not move; it only sets a temporary marker

**The actual movement or stealing happens when a move assingment or constructor is invoked, i.e a value is created without && reference**.

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

## Q: why do move constructors need to have non-const rvalue references?

We need a move constructor like `MyObj(MyObj&& obj)`, instead of `MyObj(const MyObj&& obj)`,
**Because to effectively steal and make original reference empty (but valid), you need to make it modifiable, i.e. non-const**

So effectively, **const objects cannot be moved!!!**

## Q: why can't const objects be moved?

We need a constructor like `MyObj(MyObj&& obj)`, instead of `MyObj(const MyObj&& obj)`,
**Because to effectively steal and make original reference empty (but valid), you need to make it modifiable, i.e. non-const**

So effectively, **const objects cannot be moved!!!**

## Gotcha: Move stealing does not automatically happen on pass by rvalue references to regular functions or constructors, it depends on implementation.

**Accepting rvalue references just mentions intention to steal, actual stealing depends on implementation**

In other words, **the mere act of accepting an rvalue reference does not impose move semantics automatically**. It's the explicit use of move operations (or move-constructors/assignment operators) within the function that causes the resource transfer (stealing). i.e. **rvalue references in function signature just mentions the intention to steal, but the actual stealing depends on the function implementation, i.e. stealing memory and nullifying the source as the part of function implementation**


accepting an rvalue reference in a function does not automatically "steal" the object's resources. The behavior depends on **what the function does with that rvalue reference**.

- **Move Constructor/Assignment Operator:**  
  These are specifically designed to "steal" or transfer the ownership of resources from one object to another. Their implementation moves resource ownership and typically leaves the source object in a valid but indeterminate state.

- **Functions Accepting Rvalue References:**  
  When you write a function that takes an rvalue reference (e.g., `void func(T&& value)`), the function has the flexibility to:
  - **Move from the rvalue:** Explicitly transfer resource ownership by using operations like `std::move(value)` inside the function.
  - **Copy from the rvalue:** If needed, it can create a copy.
  - **Just Inspect or Modify:** Use the object without transferring its state.


**Example Demonstration:**

```cpp
#include <iostream>
#include <string>
#include <utility>

// Function that accepts an rvalue reference
void processString(std::string&& str) {
    // Option 1: Move the string into a local variable,
    // effectively "stealing" its resources.
    std::string local_str = std::move(str);
    std::cout << "Moved string: " << local_str << "\n";

    // Option 2: Alternatively, you could inspect or modify 'str' without moving.
    // For example, if you just want to display the original:
    // std::cout << "String remains: " << str << "\n";
}

int main() {
    std::string myStr = "Hello, world!";
    
    // Call function with std::move to mark it as rvalue:
    processString(std::move(myStr));
    
    // At this point, myStr is in a valid but unspecified state.
    std::cout << "After processString, myStr: " << myStr << "\n";
    
    return 0;
}
```

In this example:

- When `processString` is called, we use `std::move(myStr)` to cast `myStr` to an rvalue.
- Inside `processString`, we **choose** to move from `str` by creating a local variable `local_str` with `std::move(str)`.
- If we did not apply `std::move(str)` inside the function, then no move would occur by default—the function could merely inspect or modify `str` without transferring its resources.

The key takeaway is that **move semantics are opt-in**; you must explicitly move the resources. Simply having an rvalue reference parameter isn't enough to steal the object's ownership—it's the operations you perform within the function that determine whether a move (resource transfer) occurs.

