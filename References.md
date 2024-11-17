
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