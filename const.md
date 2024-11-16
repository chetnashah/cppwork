## Usages

In C++, the `const` keyword is used to declare objects and variables as constant, meaning their values cannot be modified after initialization. `const` can be applied to variables, function parameters, member functions, and member variables. Here's an explanation of how `const` is used in different contexts:

1. **Constant Variables**:
   ```cpp
   const int x = 10;
   ```
   Here, `x` is a constant variable initialized with the value 10. Once initialized, the value of `x` cannot be changed.

2. **Pointer to Constant Data**:
   ```cpp
   const int* ptr = &x;
   ```
   This declares `ptr` as a pointer to a constant integer. It means that the data pointed to by `ptr` (i.e., the value of `x`) cannot be modified through `ptr`, although `ptr` itself can be modified to point to a different address.

3. **Constant Pointer**:
   ```cpp
   int y = 20;
   int* const ptr2 = &y;
   ```
   This declares `ptr2` as a constant pointer to an integer. It means that `ptr2` always points to the same address, but the value at that address (i.e., `y`) can be modified.

4. **Const Reference**:
   ```cpp
   const int& ref = x;
   ```
   This declares `ref` as a constant reference to an integer. It means that `ref` cannot be used to modify the value of `x`, but it can be used to access `x` without making a copy.

5. **Const Member Functions**:
   ```cpp
   class MyClass {
   public:
       void func() const {
           // This function is const
       }
   };
   ```
   Declares `func` as a const member function of `MyClass`. Inside a const member function, the object's member variables cannot be modified.

6. **Const Member Variables**:
   ```cpp
   class MyClass {
   public:
       const int value = 10;
   };
   ```
   Declares `value` as a constant member variable of `MyClass`. It means that each instance of `MyClass` will have its own `value`, which cannot be modified after initialization.

Using `const` helps in writing safer and more predictable code by preventing accidental modification of variables and ensuring that certain operations do not change the state of objects. It also helps in documenting the intent of code and allows compilers to perform optimizations.


## top level and low level const