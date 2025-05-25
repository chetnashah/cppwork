`__PRETTY_FUNCTION__` is a **compiler-specific predefined identifier** in C++ (supported by GCC, Clang, and some other compilers) that expands to a string containing the **decorated name of the current function**, including its signature (parameter types, constness, template parameters, etc). In MSVC it is equivalent to `__FUNCSIG__`.

It is mainly used for **debugging, logging, and diagnostics** to print out detailed information about which function is currently executing.

**Very useful** when working with templates, as it provides the full type information of the template parameters.

---

## **How does it work?**

- When you use `__PRETTY_FUNCTION__` inside a function, it expands to a `const char*` string describing the function's declaration.
- This string is more informative than `__FUNCTION__` (which gives just the function name).

---

## **Example**

```cpp
#include <iostream>

void foo(int x) {
    std::cout << "__FUNCTION__      : " << __FUNCTION__ << "\n";
    std::cout << "__PRETTY_FUNCTION__: " << __PRETTY_FUNCTION__ << "\n";
}

template<typename T>
void bar(T t) {
    std::cout << "__PRETTY_FUNCTION__: " << __PRETTY_FUNCTION__ << "\n";
}

int main() {
    foo(10);

    bar(3.14);
}
```

**Output (GCC/Clang):**
```
__FUNCTION__      : foo
__PRETTY_FUNCTION__: void foo(int)
__PRETTY_FUNCTION__: void bar(T) [with T = double]
```

---

## **Comparison Table**

| Identifier             | Expands to…                                                     |
|------------------------|-----------------------------------------------------------------|
| `__FUNCTION__`         | The unadorned function name (e.g., `foo`)                       |
| `__func__`             | The unadorned function name (standard C99/C++11)                |
| `__PRETTY_FUNCTION__`  | The full decorated function signature (GCC/Clang extension)     |
| `__FUNCSIG__`          | The MSVC equivalent for function signature                      |

---

## **Use Cases**

- **Debugging:** Print which function was called, including template parameters and overloads.
- **Logging:** More informative logging messages with function context.
- **Assertions:** Traceback with full function signatures for error reporting.

---

## **Portability**

- `__PRETTY_FUNCTION__` is **not part of the C++ standard**, but it is widely supported by GCC and Clang.
- MSVC provides a similar macro: `__FUNCSIG__`.

---

## **Summary**

- `__PRETTY_FUNCTION__` gives you a **human-readable string** describing the exact function (with parameters and template args) where it's used.
- It's **very useful** for debugging and logging in C++ code, especially with templates and overloads.

---
