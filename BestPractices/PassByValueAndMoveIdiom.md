Okay, let's break down the "Pass by value and use `std::move`" advice (often called the "sink parameter" idiom) from first principles.

**The Goal: Efficiently Getting Data into a Function**

Imagine you have a class and want to set a member variable, like a string. The data might come from somewhere else.

```c++
#include <string>
#include <iostream>
#include <utility> // For std::move

class Person {
private:
    std::string name_;

public:
    // How should we write setName?
    // void setName(???);

    void printName() const {
        std::cout << "Person's name: " << name_ << std::endl;
    }
};
```

We want `setName` to store the provided name into the `name_` member. The key challenge arises because the caller might provide the name in different ways:

1.  **As an Lvalue:** An object that persists beyond the call, which the caller might still need.
    ```c++
    std::string originalName = "Alice";
    person.setName(originalName); // We still want to use originalName later maybe
    std::cout << "Original variable: " << originalName << std::endl; // Should still be "Alice"
    ```
2.  **As an Rvalue:** A temporary object or something the caller explicitly marks as movable (doesn't need it anymore).
    ```c++
    person.setName("Bob"); // "Bob" is a temporary C-string literal, converted to a temporary std::string
    // Or:
    std::string tempName = "Charlie";
    person.setName(std::move(tempName)); // Explicitly say: you can steal from tempName
    // std::cout << "Temp variable after move: " << tempName << std::endl; // tempName is now likely empty or unspecified
    ```

**Pre-C++11 Approaches (and their drawbacks)**

1.  **Pass by `const` reference:**
    ```c++
    // Option 1: Pass by const reference
    void setName(const std::string& newName) {
        std::cout << "setName(const&): Copying inside." << std::endl;
        name_ = newName; // Always makes a copy inside the function
    }
    ```
    * **Pros:** Efficient if the caller passes an lvalue (no copy *into* the function parameter).
    * **Cons:** If the caller passes an rvalue (a temporary like `"Bob"`), this approach still forces a **copy** inside the function (`name_ = newName;`). We could have potentially avoided this copy if we knew the source was temporary.

2.  **Pass by value (Pre-C++11 thinking):**
    ```c++
    // Option 2: Pass by value (without considering move semantics yet)
    void setName(std::string newName) { // newName is always a copy of the argument
        std::cout << "setName(value): Copying inside (assigning)." << std::endl;
        name_ = newName; // Assigns (potentially another copy) from the parameter copy
    }
    ```
    * **Pros:** Simple? You get your own copy immediately in `newName`.
    * **Cons:** Forces a **copy** when the parameter `newName` is created, *regardless* of whether the caller passed an lvalue or rvalue. Might involve two copies in total (one into the parameter, one into the member). Very inefficient for rvalues.

**C++11 Move Semantics and the "Perfect" Overload Solution**

C++11 introduced move semantics to avoid unnecessary copies for rvalues. We *could* write two separate functions (overloads):

```c++
// Option 3: Overloading for lvalues and rvalues (Most Efficient)
public:
    // Version for lvalues: Takes by const ref, copies internally
    void setName(const std::string& newName) {
        std::cout << "setName(const&): Copying." << std::endl;
        name_ = newName; // Copy assignment
    }

    // Version for rvalues: Takes by rvalue ref (&&), MOVES internally
    void setName(std::string&& newName) {
        std::cout << "setName(&&): Moving." << std::endl;
        name_ = std::move(newName); // Move assignment (steals resources)
    }
```

* **Pros:** This is technically the *most efficient* solution.
    * Lvalue input (`setName(originalName);`): Makes 1 copy (inside the `const&` version).
    * Rvalue input (`setName("Bob");` or `setName(std::move(tempName));`): Makes 0 copies, just 1 move (inside the `&&` version). Moves are usually very cheap for types like `std::string` (just swapping pointers).
* **Cons:** Code duplication. You have to write and maintain two versions of the function. For complex functions or classes with many such parameters, this becomes tedious ("overload hell").

**The Clang-Tidy Solution: Pass by Value and `std::move`**

This idiom offers a compromise: write **one** function that leverages move semantics smartly.

```c++
// Option 4: Pass by value and use std::move (The Idiom)
public:
    void setName(std::string newName) { // Pass by value
        std::cout << "setName(value): Moving from parameter." << std::endl;
        // newName is our local parameter copy/move.
        // Now, efficiently move it into the final destination (name_).
        name_ = std::move(newName);
    }
```

**Motivation and Reason: How it Works**

Let's trace the execution for both lvalue and rvalue inputs:

1.  **Caller passes LVALUE (`setName(originalName);`)**
    * `setName(std::string newName)`: Parameter `newName` is created **by copy** from `originalName` (using `std::string`'s copy constructor). `originalName` is untouched. Cost: **1 Copy**.
    * `name_ = std::move(newName);`: The content of the *parameter* `newName` is **moved** into the member `name_` (using `std::string`'s move assignment). This is cheap. Cost: **1 Move**.
    * Total: 1 Copy + 1 Move.

2.  **Caller passes RVALUE (`setName("Bob");` or `setName(std::move(tempName));`)**
    * `setName(std::string newName)`: Parameter `newName` is created **by move** from the incoming rvalue (using `std::string`'s move constructor). This is cheap and avoids a copy! Cost: **1 Move**.
    * `name_ = std::move(newName);`: The content of the *parameter* `newName` is **moved** into the member `name_` (using `std::string`'s move assignment). This is cheap. Cost: **1 Move**.
    * Total: 1 Move + 1 Move.

**Why is this good? (The Motivation)**

* **Simplicity:** You write only **one** function, not two. Less code, easier to maintain.
* **Good Performance Trade-off:**
    * For rvalue inputs, it's very efficient (2 cheap moves) – vastly better than the old pass-by-const-ref-and-copy approach (1 expensive copy). It's almost as good as the perfect overload (2 moves vs 1 move).
    * For lvalue inputs, it costs 1 copy + 1 cheap move, compared to the perfect overload's 1 copy. The extra move is usually negligible.
* **Leverages Move Semantics:** It automatically uses the move constructor for parameter creation when available (when called with an rvalue), which is a key optimization.
* **Clear Intent:** Taking by value often signals that the function intends to make its own copy or take ownership anyway. Using `std::move` inside makes it explicit that you are potentially consuming the parameter's state.

**When to Use This Idiom:**

* When your function needs its **own copy** of the argument or needs to **take ownership** (e.g., storing it in a member variable, putting it in a container).
* When the argument type has **efficient move semantics** (like `std::string`, `std::vector`, `std::unique_ptr`, and well-designed user classes).
* When the simplicity of writing one function outweighs the small potential performance cost of an extra move compared to writing separate lvalue/rvalue overloads.

**When NOT to Use This Idiom:**

* If the function only needs to **read/inspect** the argument without modifying or storing it. Use **pass by `const` reference** (`const T&`).
* If the type is **cheap to copy** (like `int`, `float`, `double`, `bool`, pointers, simple POD structs). Just pass by value; the `std::move` inside provides no benefit and might even hinder optimization.
* If the type is **not movable**, or its move operation is just as expensive as its copy operation.
* If you are in a performance-critical context where the cost of that extra move (in both lvalue and rvalue cases compared to the perfect overload) is actually measurable and unacceptable. In such rare cases, use the two-overload approach.

In essence, "Pass by value and use `std::move`" is a pragmatic and widely recommended idiom in modern C++ for functions that need to capture arguments by value, providing a good balance between code simplicity and performance by effectively utilizing move semantics.