
## Struct initialization

In C++, you can initialize a struct using several different methods. Here are some common ways to initialize a struct:

1. **Default Initialization**:
   If you don't provide any initializers, the struct members will be default-initialized. For built-in types, this means they will have an indeterminate value, while for user-defined types, the default constructor will be called.

2. **Member-wise Initialization**:
   You can explicitly initialize each member of the struct using the member initialization syntax. This involves providing a comma-separated list of member initializers within braces `{}` after the struct name.

   ```cpp
   struct MyStruct {
       int x;
       double y;
   };

   MyStruct s = {10, 3.14};
   ```

   In this example, the struct `MyStruct` has two members `x` and `y`. The `s` object is initialized with the values `10` for `x` and `3.14` for `y`.

3. **Designated Initializers** (C++20):
   Starting from C++20, you can use designated initializers to initialize specific members of a struct, even if they are not in the order of declaration.

   ```cpp
   struct MyStruct {
       int x;
       double y;
       char z;
   };

   MyStruct s = {.y = 3.14, .x = 10};
   ```

   In this example, the struct `MyStruct` has three members `x`, `y`, and `z`. The `s` object is initialized with the values `10` for `x` and `3.14` for `y`, while `z` will be default-initialized.

4. **Aggregate Initialization**:
   If a struct is an aggregate type, you can use aggregate initialization. **An aggregate type is a struct that has no user-declared constructors, no private or protected non-static data members, no base classes, and no virtual functions.**

   ```cpp
   struct MyStruct {
       int x;
       double y;
   };

   MyStruct s{10, 3.14};
   ```

   In this example, the struct `MyStruct` is an aggregate type, so you can directly initialize `s` using braces `{}` with the values `10` for `x` and `3.14` for `y`.

These are some of the common ways to initialize a struct in C++. The choice of initialization method depends on the specific requirements of your code and the version of C++ you are using.