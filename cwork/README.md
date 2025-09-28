
---

# 1. **Declaration**

A *declaration* tells the compiler about the type and name of something (variable, function, typedef, struct, etc.).

### Properties:

* Introduces a name into scope.
* Doesn’t necessarily allocate storage (e.g. `extern int x;`).
* Can appear at file scope or block scope.

### Examples:

```c
extern int x;         // declaration only (no storage)
int f(int a, int b);  // function declaration (prototype)
typedef unsigned long ulong;  // typedef declaration
```

---

# 2. **Definition**

A *definition* is a declaration that **actually allocates storage** (for objects) or **provides a body** (for functions).

### Properties:

* All definitions are declarations.
* Not all declarations are definitions.

### Examples:

```c
int x;                 // definition (and declaration), allocates storage
int y = 42;            // definition with initialization
int f(int a, int b) {  // function definition
    return a + b;
}
```

### Gotcha:

* Multiple declarations of the same identifier are allowed.
* But multiple **definitions** of the same identifier in one program violate the **One Definition Rule (ODR)** in C.

---

# 3. **Statement**

A *statement* is an executable action inside a function body.

### Properties:

* Generates runtime code.
* Only allowed inside function bodies.
* Can be simple (expression statement) or complex (compound statement, selection, iteration, jump).

### Examples:

```c
x = 5;                  // expression statement
if (x > 0) foo();       // selection statement
while (x--) bar();      // iteration statement
return 0;               // jump statement
{ int y = 2; y++; }     // compound statement
```

---

# 4. **Expression**

An *expression* is a combination of operators, operands, and function calls that evaluates to a value.

### Properties:

* Building block of statements.
* Has a type and may have side effects.
* Can be a constant, variable, arithmetic operation, function call, etc.

### Examples:

```c
5 + 2           // arithmetic expression
x = y + 3       // assignment expression (also returns a value!)
foo(bar(), 42)  // function call expression
*p++            // pointer + side effect expression
```

---

# 5. **Crossovers & Combinations**

Here’s where lines blur:

1. **Declaration + Definition**

```c
int a;          // both declaration and definition
```

2. **Declaration only**

```c
extern int a;   // only declaration
```

3. **Definition + Statement**

```c
void f() {
    int a = 10;   // local variable: declaration + definition + statement
}
```

4. **Expression + Statement**

```c
x = y + 3;     // expression (y+3) used inside an expression statement
```

5. **Expression vs Statement**

```c
x + 1;         // ❌ expression alone is not a statement in C
x + 1;         // ✅ becomes a statement if you add `;` (expression statement)
```

6. **Function Definition vs Call**

```c
int foo() { return 42; }  // definition
foo();                    // statement (expression statement using a call expression)
```

---

# 6. **Gotchas & Edge Cases**

* **Global initializers:**

  ```c
  int g = 5;    // definition + declaration, but NOT a statement
  ```

  No executable code runs; value is placed in `.data`.

* **No function calls in global initializers (in C):**

  ```c
  int g = foo();   // ❌ illegal in C (but legal in C++).
  ```

* **Multiple declarations:**

  ```c
  extern int x;  // ok
  extern int x;  // ok
  int x;         // one definition required
  ```

* **Typedef confusion:**

  ```c
  typedef unsigned int u32;   // declaration, not definition (no storage)
  ```

* **Declarations as statements:**
  Inside a block, `int a = 1;` is both a declaration, a definition, and a statement.

* **Compound statements with declarations first (ANSI C):**

  ```c
  void foo() {
      int a = 1;     // allowed
      a++;           // statement
      int b = 2;     // ❌ illegal in ANSI C89 (ok in C99+)
  }
  ```

---

# 7. **Cheat Table**

| Construct                  | Declaration | Definition | Statement | Expression | Notes                        |
| -------------------------- | ----------- | ---------- | --------- | ---------- | ---------------------------- |
| `extern int x;`            | ✅           | ❌          | ❌         | ❌          | says "x exists somewhere"    |
| `int x;` (global)          | ✅           | ✅          | ❌         | ❌          | storage allocated            |
| `int x = 3;` (inside func) | ✅           | ✅          | ✅         | ❌          | declaration statement        |
| `x = 5;`                   | ❌           | ❌          | ✅         | ✅          | expression statement         |
| `return 0;`                | ❌           | ❌          | ✅         | ❌          | jump statement               |
| `int f(int);`              | ✅           | ❌          | ❌         | ❌          | function prototype           |
| `int f(){ return 1; }`     | ✅           | ✅          | ❌         | ❌          | function definition          |
| `foo();`                   | ❌           | ❌          | ✅         | ✅          | call expression as statement |

---

### 8. **Mental model**

* **Declaration**: “Hey compiler, this thing exists and has this type.”
* **Definition**: “Allocate it / give it a body.”
* **Statement**: “At runtime, do this.”
* **Expression**: “Evaluate this to a value.”

---

Do you want me to now draw a **visual hierarchy diagram** (like a tree) showing how these concepts nest and overlap in C grammar? That often makes it easier to remember which constructs can be both or all three.
