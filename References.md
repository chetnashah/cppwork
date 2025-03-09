
## Reference binding rules

| Reference Type         | Can Bind to LValue | Can Bind to RValue |
| ---------------------- | :----------------: | :----------------: |
| LValue Reference       |         ✓          |         ✗          |
| Const LValue Reference |         ✓          |         ✓          |
| RValue Reference       |         ✗          |         ✓          |
| Const RValue Reference |         ✗          |         ✓          |

## Examples

Sure! Let's go through each reference type and understand how they bind to lvalues and rvalues with examples.

### LValue Reference
- **Can Bind to LValue:** Yes
- **Can Bind to RValue:** No

**Example:**
```cpp
int x = 10;
int& ref = x; // OK: LValue reference can bind to an lvalue
// int& ref = 10; // Error: LValue reference cannot bind to an rvalue
```

### Const LValue Reference
- **Can Bind to LValue:** Yes
- **Can Bind to RValue:** Yes

**Example:**
```cpp
int x = 10;
const int& cref = x; // OK: Const LValue reference can bind to an lvalue
const int& cref2 = 10; // OK: Const LValue reference can bind to an rvalue
```

### RValue Reference
- **Can Bind to LValue:** No
- **Can Bind to RValue:** Yes

**Example:**
```cpp
int&& rref = 10; // OK: RValue reference can bind to an rvalue
// int&& rref = x; // Error: RValue reference cannot bind to an lvalue
```

### Const RValue Reference
- **Can Bind to LValue:** No
- **Can Bind to RValue:** Yes

**Example:**
```cpp
const int&& crref = 10; // OK: Const RValue reference can bind to an rvalue
// const int&& crref = x; // Error: Const RValue reference cannot bind to an lvalue
```

### Summary of Binding Rules
- **LValue Reference:** Can only bind to lvalues.
- **Const LValue Reference:** Can bind to both lvalues and rvalues.
- **RValue Reference:** Can only bind to rvalues.
- **Const RValue Reference:** Can only bind to rvalues.

These rules help in understanding how references interact with different types of values in C++, ensuring type safety and enabling features like move semantics and perfect forwarding.

## Special member function generation

In C++ the special member functions (such as copy and move constructors/assignment operators) are implicitly generated based on the properties of your data members and base classes. In particular, if *all* of your class’s members are only movable (i.e., their copy constructors and copy assignment operators are deleted or inaccessible) and support moving, then the compiler‐generated copy constructor and copy assignment operator for your class will be **implicitly defined as deleted**. Meanwhile, the move constructor and move assignment operator will still be generated (provided that every member supports moving). In effect, your class becomes move-only.

Below are some common combinations and their effects on the implicit special member functions of a class:

---

### 1. All Members Are Copyable and Movable

- **Members:** Each member type provides both copy and move operations.
- **Result:**  
  - The compiler implicitly generates both copy and move constructors/assignment operators.  
  - Your class is **both copyable and movable**.

*Example:*
```cpp
#include <string>
#include <vector>

class Both {
public:
    std::string s;               // copyable and movable
    std::vector<int> vec;        // copyable and movable
};
```

---

### 2. All Members Are Copyable Only (No Move Operations)

- **Members:** Each member type provides a copy constructor/assignment operator but does not support moving (or has its move operations deleted).
- **Result:**  
  - The compiler will generate the copy operations normally.
  - The move constructor and move assignment operator will be implicitly declared as deleted (or not provided).
  - Your class is **copyable only**.

*Example:*
```cpp
#include <memory>

class CopyableOnly {
public:
    // Imagine a type that disables moving explicitly:
    class NonMovable {
    public:
        NonMovable() = default;
        NonMovable(const NonMovable&) = default;
        NonMovable& operator=(const NonMovable&) = default;
        NonMovable(NonMovable&&) = delete;
        NonMovable& operator=(NonMovable&&) = delete;
    };

    NonMovable nm;
};
```

---

### 3. All Members Are Movable Only (No Copy Operations)

- **Members:** Each member type provides a move constructor/assignment operator but deletes (or does not provide) its copy operations.
- **Result:**  
  - The compiler will generate a move constructor/assignment operator.
  - The copy constructor and copy assignment operator will be declared as deleted.
  - Your class is **move-only**.

*Example:*
```cpp
#include <utility>

class MoveOnlyMember {
public:
    MoveOnlyMember() = default;
    MoveOnlyMember(MoveOnlyMember&&) = default;
    MoveOnlyMember& operator=(MoveOnlyMember&&) = default;

    // Delete copy operations
    MoveOnlyMember(const MoveOnlyMember&) = delete;
    MoveOnlyMember& operator=(const MoveOnlyMember&) = delete;
};

class Container {
public:
    MoveOnlyMember m;  // move-only member

    // Container will be move-only too because copying m is not allowed.
};
```

---

### 4. A Mix of Copyable and Movable-Only Members

- **Members:** Suppose one member is move-only while the others are copyable (or copy-and-movable).
- **Result:**  
  - The copy constructor and copy assignment operator for the containing class will be implicitly defined as deleted because copying the move-only member isn’t allowed.
  - The move constructor and move assignment operator can still be implicitly generated provided all members are accessible via move.
  - The class becomes **move-only**.

*Example:*
```cpp
#include <string>

class MoveOnly {
public:
    MoveOnly() = default;
    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;
};

class MixedContainer {
public:
    std::string data;  // copyable and movable
    MoveOnly moveOnlyData;  // move-only

    // The implicitly generated copy constructor is deleted because moveOnlyData isn’t copyable.
    // The move constructor is generated and moves both members.
};
```

---

### 5. A Member That Is Neither Copyable Nor Movable

- **Members:** At least one member type deletes both its copy and move operations.
- **Result:**  
  - Neither the copy constructor/assignment operator nor the move constructor/assignment operator can be automatically generated.
  - Your class ends up being **neither copyable nor movable** unless you provide custom implementations that somehow work around that (which is uncommon).

*Example:*
```cpp
class NonCopyNonMove {
public:
    NonCopyNonMove() = default;
    NonCopyNonMove(const NonCopyNonMove&) = delete;
    NonCopyNonMove& operator=(const NonCopyNonMove&) = delete;
    NonCopyNonMove(NonCopyNonMove&&) = delete;
    NonCopyNonMove& operator=(NonCopyNonMove&&) = delete;
};

class Fixed {
public:
    NonCopyNonMove member;

    // Neither copy nor move operations can be implicitly generated.
};
```

---

### Summary

- **If all members are only movable (and not copyable):**  
  → The class’s copy constructor and copy assignment operator are implicitly deleted, and the move operations are available.  
  → **Result:** The class is move-only.

- **Other combinations:**  
  - If all are copyable & movable → class is both copyable and movable.  
  - If all are copyable only → class is copyable only.  
  - If one member is only movable while others are copyable → class is move-only.  
  - If any member is neither copyable nor movable → class is typically neither copyable nor movable.

Understanding these rules helps ensure that your classes automatically inherit the intended semantics from their members without surprises.