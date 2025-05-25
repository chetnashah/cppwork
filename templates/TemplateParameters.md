Okay, let's break down in detail all the kinds of things you can pass as template parameters in C++. Templates are a powerful C++ feature for writing generic code that can operate on different types or values without being rewritten. These parameters are specified at compile time.

There are three main categories of template parameters:

1.  **Type Parameters**
2.  **Non-Type Parameters**
3.  **Template Template Parameters**

We'll also touch upon parameter packs, default arguments, and concepts, as they relate to these parameters.

---

### 1. Type Parameters

* **What it is:** A placeholder for an actual C++ data type. This is the most common kind of template parameter.
* **Syntax:**
    ```cpp
    template <typename T> class MyClass { /* ... */ };
    // or
    template <class U> U myFunction(U arg) { /* ... */ };
    ```
    (`typename` and `class` are interchangeable in this context for declaring type parameters.)
* **What can be passed as an argument:** Any valid C++ type, including:
    * Fundamental types (`int`, `double`, `char`, `bool`, etc.)
    * User-defined types (structs, classes, enums)
    * Pointer types (`int*`, `MyClass*`)
    * Reference types (`int&`, `const MyClass&`)
    * Arrays (though they often decay to pointers in function template arguments unless passed by reference)
* **Use Cases:**
    * **Generic Data Structures:** Creating containers like lists, vectors, stacks, queues, or maps that can store elements of any type.
    * **Generic Algorithms:** Writing algorithms like `sort`, `find`, `max`, `min` that can operate on various data types or containers holding various types.
    * **Type-Agnostic Utilities:** Functions or classes that perform an operation independent of the specific type, like a generic printing function or a smart pointer.
* **Simple & Clear Examples:**

    **a) Generic Box Class:**
    ```cpp
    #include <iostream>

    template <typename T> // T is a type parameter
    class Box {
    private:
        T content;
    public:
        Box(T item) : content(item) {}
        T getContent() const { return content; }
        void setContent(T item) { content = item; }
    };

    int main() {
        Box<int> intBox(123);
        std::cout << "IntBox content: " << intBox.getContent() << std::endl; // Output: 123

        Box<std::string> stringBox("Hello C++");
        std::cout << "StringBox content: " << stringBox.getContent() << std::endl; // Output: Hello C++
    }
    ```

    **b) Generic `maximum` Function:**
    ```cpp
    #include <iostream>

    template <typename T>
    T maximum(T a, T b) {
        return (a > b) ? a : b;
    }

    int main() {
        std::cout << "Max of 5, 10 is " << maximum(5, 10) << std::endl;            // T becomes int
        std::cout << "Max of 3.14, 2.71 is " << maximum(3.14, 2.71) << std::endl; // T becomes double
        std::cout << "Max of 'a', 'z' is " << maximum('a', 'z') << std::endl;    // T becomes char
    }
    ```

---

### 2. Non-Type Parameters

* **What it is:** A placeholder for a compile-time constant value, rather than a type.
* **What can be passed as an argument:** The argument must be a compile-time constant expression of a type compatible with the parameter. This means its value must be known at compile time.
* **Categories, Syntax, Use Cases & Examples:**

    **A. Integral Types (and Enums)**
    * **Syntax:** `template <int N>`, `template <size_t Size>`, `template <bool Flag>`, `template <MyEnum E>`
    * **Argument:** Integer literals (`10`, `0x0F`), `char` literals (`'A'`), `bool` literals (`true`, `false`), enum constants (`MyEnum::Value`), `constexpr` variables of integral type.
    * **Use Case:** Defining fixed sizes for arrays or buffers within a class, specifying bitmasks, compile-time feature flags, iteration counts.
    * **Example: Fixed-Size Array Wrapper**
        ```cpp
        #include <iostream>
        #include <array> // For comparison, std::array also uses non-type params

        template <typename T, size_t Capacity> // Capacity is a non-type parameter
        class FixedBuffer {
        private:
            T data[Capacity]; // Use Capacity to define array size
            size_t current_size = 0;
        public:
            bool add(const T& item) {
                if (current_size < Capacity) {
                    data[current_size++] = item;
                    return true;
                }
                return false;
            }
            void print() const {
                for (size_t i = 0; i < current_size; ++i) {
                    std::cout << data[i] << " ";
                }
                std::cout << std::endl;
            }
            static constexpr size_t getCapacity() { return Capacity; }
        };

        int main() {
            FixedBuffer<int, 5> smallBuffer; // Capacity is 5
            smallBuffer.add(10);
            smallBuffer.add(20);
            std::cout << "Buffer capacity: " << smallBuffer.getCapacity() << std::endl; // Output: 5
            smallBuffer.print(); // Output: 10 20

            FixedBuffer<char, 3> charBuffer;
            charBuffer.add('a');
            charBuffer.add('b');
            charBuffer.add('c');
            charBuffer.add('d'); // Fails, buffer is full
            charBuffer.print();  // Output: a b c
        }
        ```

    **B. Pointers (to objects/functions with external/internal linkage, or to members)**
    * **Syntax:** `template <int* PtrToInt>`, `template <void (*FuncPtr)(int)>`, `template <int MyClass::*MemberPtr>`
    * **Argument:** The address of a global/static variable or function (e.g., `&myGlobalVar`, `&myFunction`), `nullptr` (for pointer types), or a pointer-to-member constant (e.g., `&MyClass::dataMember`).
    * **Use Case:** Making a template operate on a specific global instance, using a compile-time specified callback function, or accessing a specific class member determined at compile time.
    * **Example: Function Runner**
        ```cpp
        #include <iostream>

        void task1() { std::cout << "Executing Task 1" << std::endl; }
        void task2() { std::cout << "Executing Task 2" << std::endl; }

        template <void (*Action)()> // Action is a non-type pointer-to-function
        class Runner {
        public:
            void run() {
                Action(); // Call the function via the pointer
            }
        };

        int main() {
            Runner<task1> runner1;
            runner1.run(); // Output: Executing Task 1

            Runner<task2> runner2;
            runner2.run(); // Output: Executing Task 2
        }
        ```

    **C. Lvalue References (to objects/functions with external/internal linkage)**
    * **Syntax:** `template <int& RefToInt>`, `template <MyType& ObjRef>`
    * **Argument:** An lvalue expression that refers to an object or function with static storage duration and external or internal linkage (e.g., a global/static variable).
    * **Use Case:** Similar to pointers, but ensures the argument is not null and uses reference semantics. Useful for compile-time configuration objects.
    * **Example: Configuration Printer**
        ```cpp
        #include <iostream>
        #include <string>

        struct AppConfig {
            std::string appName = "MyGenericApp";
            int version = 1;
        };

        AppConfig globalConfig; // A global configuration object

        template <AppConfig& ConfigInstance> // ConfigInstance is a reference
        class ConfigurableApp {
        public:
            void displayConfig() {
                std::cout << "App Name: " << ConfigInstance.appName << std::endl;
                std::cout << "Version: " << ConfigInstance.version << std::endl;
            }
        };

        int main() {
            globalConfig.version = 2;
            ConfigurableApp<globalConfig> myApp;
            myApp.displayConfig();
            // Output:
            // App Name: MyGenericApp
            // Version: 2
        }
        ```

    **D. `auto` (C++17 onwards)**
    * **Syntax:** `template <auto Value>`
    * **Argument:** A compile-time constant expression. The type of `Value` is deduced from the argument. Before C++20, this was limited to types traditionally allowed for non-type parameters (integrals, pointers, etc.). C++20 expanded this (see next point).
    * **Use Case:** When you want to accept various kinds of compile-time constants without explicitly specifying their type in the template parameter list, making the template more generic for different constant types.
    * **Example: Compile-time Constant Wrapper**
        ```cpp
        #include <iostream>

        template <auto Value> // Value's type is deduced
        struct ConstantWrapper {
            static constexpr auto get() { return Value; }
            void print() {
                std::cout << "Compile-time value: " << Value << std::endl;
            }
        };

        int main() {
            ConstantWrapper<42> intConst;         // Value becomes int 42
            intConst.print();

            ConstantWrapper<'X'> charConst;       // Value becomes char 'X'
            charConst.print();

            static int myStaticVar = 100;
            ConstantWrapper<&myStaticVar> ptrConst; // Value becomes int* &myStaticVar
            std::cout << "Compile-time pointer value: " << *ptrConst.get() << std::endl;
        }
        ```

    **E. C++20 Extended Non-Type Parameters (Floating-point types, Literal Class Types)**
    * **Syntax:** `template <double PiValue>`, `template <MyLiteralType Obj>`
    * **Argument (C++20):**
        * Floating-point literals (`3.14f`, `2.718`) or `constexpr` floating-point variables.
        * Objects of "literal class types" (also called "structural types"). These are class types whose equality can be determined at compile time by comparing their members. They typically have all public, non-mutable data members of literal types and no virtual functions/bases.
    * **Use Case:** Passing floating-point constants (like mathematical constants) directly, or passing simple, small, value-like configuration objects directly as template arguments.
    * **Example: Floating Point & Literal Class Type (C++20)**
        ```cpp
        #include <iostream>
        #include <string_view> // string_view can be a structural type in some contexts

        // C++20 required for floating point non-type template parameters
        template <double Factor>
        double scale(double value) {
            return value * Factor;
        }

        // Example of a simple literal class type (structural type)
        struct Color {
            unsigned char r, g, b;
            // For C++20 non-type template params, we need default comparison
            // The compiler implicitly generates one if members are public & comparable
            constexpr bool operator==(const Color&) const = default; // For structural type
        };

        template <Color C>
        void printColor() {
            std::cout << "Color: R=" << (int)C.r
                      << " G=" << (int)C.g
                      << " B=" << (int)C.b << std::endl;
        }

        int main() {
            // Floating point non-type parameter
            std::cout << "Scaled 10.0 by 2.5: " << scale<2.5>(10.0) << std::endl; // Factor is 2.5

            // Literal class type non-type parameter
            constexpr Color RED = {255, 0, 0};
            printColor<RED>(); // Output: Color: R=255 G=0 B=0

            printColor<{0, 255, 0}>(); // Directly passing a Color literal
                                       // Output: Color: R=0 G=255 B=0
        }
        ```

---

### 3. Template Template Parameters

* **What it is:** A placeholder for another template. Essentially, you are passing a "template name" as an argument.
* The most common use case is to Allow a template to accept another template as a parameter. **This enables generic code that operates on type constructors (e.g., std::vector, std::list).**, e.g., a class that can work with any container type.

* **Syntax:**
    ```cpp
    #include <vector>
    #include <list>

    // vector has two type params, which is for Container here.
    // https://en.cppreference.com/w/cpp/container/vector
    template<template<typename, typename> class Container, typename T, typename Alloc = std::allocator<T>>
    Container<T, Alloc> transform(const Container<T, Alloc>& input, T (*func)(T)) {
        Container<T, Alloc> result;
        for (const auto& elem : input) {
            result.push_back(func(elem));
        }
        return result;
    }

    // Usage:
    std::vector<int> vec = {1, 2, 3};
    auto transformed_vec = transform(vec, [](int x) { return x + 1; }); //  {2, 3, 4}

    std::list<float> lst = {1.5, 2.5};
    auto transformed_lst = transform(lst, [](float x) { return x * 2; }); // {3.0, 5.0}
    ```
* **What can be passed as an argument:** The name of a class template or an alias template that matches the signature of the template template parameter.
* **Use Cases:**
    * **Generic Container Adapters:** Creating a class that adapts or adds functionality to various underlying container types (e.g., `std::vector`, `std::list`, `std::deque`) without knowing the specific container in advance.
    * **Policy-Based Design:** Where policies are themselves templates, and you want to parameterize a class by a "template policy."
* **Simple & Clear Example:**
    ```cpp
    #include <iostream>
    #include <vector>
    #include <list>
    #include <deque> // Another container

    // This template template parameter 'Container' expects a template
    // that takes one type parameter (like 'T' in std::vector<T>)
    // and an allocator (which has a default, so we can specify it like this).
    template <typename ValueType,
              template <typename Elem, typename Alloc = std::allocator<Elem>> class Container>
    class DataStore {
    private:
        Container<ValueType> store;
    public:
        void add(const ValueType& val) {
            store.push_back(val); // Assumes push_back like vector/list/deque
        }

        void print() const {
            for (const auto& item : store) {
                std::cout << item << " ";
            }
            std::cout << std::endl;
        }
    };

    int main() {
        // Using DataStore with std::vector
        DataStore<int, std::vector> vectorStore;
        vectorStore.add(1);
        vectorStore.add(2);
        std::cout << "VectorStore: ";
        vectorStore.print(); // Output: VectorStore: 1 2

        // Using DataStore with std::list
        DataStore<std::string, std::list> listStore;
        listStore.add("apple");
        listStore.add("banana");
        std::cout << "ListStore: ";
        listStore.print(); // Output: ListStore: apple banana

        // Using DataStore with std::deque
        DataStore<double, std::deque> dequeStore;
        dequeStore.add(3.14);
        dequeStore.add(2.71);
        std::cout << "DequeStore: ";
        dequeStore.print(); // Output: DequeStore: 3.14 2.71
    }
    ```

---

### Other Related Concepts:

#### A. Parameter Packs (Variadic Templates)

Any of the above kinds of parameters (type, non-type, template template) can be a "parameter pack," meaning it can accept zero or more arguments.

* **Syntax:** An ellipsis (`...`) is used.
    * `template <typename... Args>` (pack of types)
    * `template <int... Ns>` (pack of non-type `int`s)
    * `template <template <typename> class... Containers>` (pack of template templates)
* **Use Case:** Creating functions that take a variable number of arguments (e.g., a type-safe `printf`), classes that can hold a variable number of members of different types (e.g., `std::tuple`), or applying an operation to a list of types/values.
* **Example: Variadic Print Function**
    ```cpp
    #include <iostream>

    // Base case for recursion (when pack is empty)
    void printItems() {
        std::cout << std::endl;
    }

    // Recursive variadic template function
    template <typename T, typename... Rest>
    void printItems(const T& first, const Rest&... rest) {
        std::cout << first << " ";
        printItems(rest...); // Recurse with the rest of the arguments
    }

    int main() {
        printItems("Hello", 42, 3.14, 'C'); // Output: Hello 42 3.14 C
        printItems(100, "World");           // Output: 100 World
        printItems();                       // Output: (newline)
    }
    ```

#### B. Default Template Arguments

Template parameters of any kind can have default arguments. If a template argument is not provided during instantiation, the default is used.

* **Use Case:** Making templates easier to use by providing common defaults, making certain parameters optional.
* **Example:**
    ```cpp
    #include <iostream>
    #include <vector>

    template <typename T = int, // Default type is int
              template <typename E, typename A = std::allocator<E>> class Container = std::vector, // Default container is std::vector
              int DefaultCapacity = 16> // Default non-type is 16
    class MyFlexibleContainer {
        Container<T> data;
    public:
        MyFlexibleContainer() {
            // data.reserve(DefaultCapacity); // If container has reserve
            std::cout << "Created container with capacity hint: " << DefaultCapacity << std::endl;
        }
        // ... other methods
    };

    int main() {
        MyFlexibleContainer<> c1; // Uses MyFlexibleContainer<int, std::vector, 16>
        MyFlexibleContainer<double> c2; // Uses MyFlexibleContainer<double, std::vector, 16>
        MyFlexibleContainer<char, std::list> c3; // Uses MyFlexibleContainer<char, std::list, 16>
        MyFlexibleContainer<short, std::vector, 32> c4; // All specified
    }
    ```

#### C. Constraints and Concepts (C++20)

With C++20, you can apply "concepts" to template parameters to constrain the kinds of arguments that are acceptable. This improves type checking and error messages.

* **Use Case:** Ensuring that a type passed as a template argument has certain properties (e.g., is an integral type, is printable, has a `begin()` and `end()` method).
* **Example:**
    ```cpp
    #include <iostream>
    #include <concepts> // For standard concepts

    // T is constrained to be an integral type
    template <std::integral T>
    T add(T a, T b) {
        return a + b;
    }

    // Printable must support operator<< with std::ostream
    template <typename T>
    concept Printable = requires(std::ostream& os, T val) {
        { os << val } -> std::same_as<std::ostream&>;
    };

    template <Printable P>
    void printMe(P val) {
        std::cout << "Printed: " << val << std::endl;
    }

    int main() {
        std::cout << add(5, 10) << std::endl; // OK, int is integral
        // std::cout << add(5.5, 10.2) << std::endl; // Compile error: double is not std::integral

        printMe(123);       // OK, int is Printable
        printMe("test");    // OK, const char* is Printable
        struct NonPrintable {};
        // printMe(NonPrintable{}); // Compile error if NonPrintable doesn't support <<
    }
    ```

This covers the landscape of C++ template parameters. They are a fundamental tool for writing flexible, reusable, and efficient code.