
lambdas are anonymous functions

https://learn.microsoft.com/en-us/cpp/cpp/lambda-expressions-in-cpp?view=msvc-170&viewFallbackFrom=vs-2019

 a function object is an instance of a class, for which the call operator ( operator`()` ) is overloaded. This means that a function object is an object that behaves like a function. The main difference between a function and a function object is: a function object is an object and can, therefore, have stated.

## Best practices

**Best practice 1** - Do not use default catpures `[=]` or `[&]` unless you are sure that you need them,e.g. they might capture `this` implicitly misleading us to believe members are captured. It is better to be explicit about what you are capturing using generalized/init capture syntax.

**Note**:
1. default `by-reference` captures i.e. `[&]` can lead to dangling references if the lambda outlives the captured variables.
2. default `by-value` captures i.e. `[=]` can lead to dangling pointers especially `this`, and it misleadingly suggests that lambdas are self-contained.

### core syntax

```cpp
[capture-list](parameters) -> returntype {
    // function body
}
```

`returntype` is optional

### most basic example

```cpp
auto func = []() {
    std::cout << "hello world" << std::endl;
};

func();
```


### How to think about capture?

Just add `auto` in front of the statements in `[]`.
If there is a single name, e.g. `[a]`, then think of it as `auto a = a` i.e. copy the value of `a` into the lambda function.

If there is a single name reference e.g. `[&a]`, then think of it as `auto& a = a` i.e. reference the value of `a` into the lambda function.

### capture lists

all free variables need to be explicitly specified in capture list
else it will be compiler error

#### capture by value

`Capture by value`: what we have in the
lambda function is a copy. 

**values captured by value can't be modified in the body of the lambda function by default**

Value capturing (i.e copy) happens at lambda definition time,
and further changes to the captured value outside lambda are unknon to the lambda.

```cpp
int c{42};
auto func = [c](){
    std::cout << "Value of c inside lambda" << c << std::endl;// always prints 42, even after c is modified later in the program
};

for(size_t i{}; i<5; i++) {
    std::cout << "outer value : " << c << std::endl;
    func();
    ++c;
}
```

#### capture all by value

Use `=` inside capture list

```cpp
auto func = [=](){
    std::cout << "Value of c inside lambda" << c << std::endl;// always refers outer one when executing
};
```


#### capture by reference

`Capture by value`: what we have in the
lambda function is an alias for the outer one.

```cpp
int c{42};
auto func = [&c](){
    std::cout << "Value of c inside lambda" << c << std::endl;// always refers outer one when executing
};

for(size_t i{}; i<5; i++) {
    std::cout << "outer value : " << c << std::endl;
    func();
    ++c;
}
```


#### capture all by reference

Use `&` inside capture list

```cpp
auto func = [&](){
    std::cout << "Value of c inside lambda" << c << std::endl;// always refers outer one when executing
};
```

## lambdas cannot capture class data members

See how lambda cannot capture class data member named `member`
```cpp
class Widget
{
public:
    Widget() = default;
    void doSomething() const;

private:
    int member;
};

void Widget::doSomething() const
{
    // Error: member "Widget::member" is not a variable
    auto f = [member]()
    { return member * 2; };
    f();
}
```

**Soltuion** - capture `[this]` or capture all `[=]` or capture all by reference `[&]` to capture class data members. Note that it will capture `this`, not the data memeber, and use of datamember in lambda will be internally done as `this->member`.

**Insight** - `this` is auto captured when lambda uses `[=]` or `[&]` capture list in a non-static member function.

```cpp
class Example {
    int data = 42;
    
    void foo() {
        // Explicit this capture
        auto lambda1 = [this]() { return data; };
        
        // Implicit this capture (= captures everything by value including this)
        auto lambda2 = [=]() { return data; };
        
        // Capture this by reference
        auto lambda3 = [this]() { data = 100; };
    }
};
```
With references:
```cpp
class Example {
    int data = 42;
    
    void foo() {
        // [&] captures everything by reference, including this
        auto lambda = [&]() { data = 100; };  // works, modifies data member
        
        // equivalent to:
        auto lambda2 = [this]() { data = 100; };
    }
};
```

## lambdas are const function objects

Lambdas are const function objects by default, meaning they cannot modify captured variables unless they are marked as `mutable` which implicitly removes the constness of the lambda.

**So if you want stateful lambdas, you need to use `mutable` keyword.** - e.g. a lambda that counts the number of times it has been called.

```cpp
#include<iostream>
int main() {
    // allows per lambda mutable state
    // note: ctr is a member of the lambda object, which is copied when lambda is copied
    auto countsCalls = [ctr=0]() mutable { return ++ctr; };
    
    // lambdas are copyable, different internal/mutable state
    auto c1 = countsCalls;
    c1();
    c1();
    std::cout << c1() << std::endl; // prints "3"

    // copy of lambda, different internal/mutable state
    auto c2 = countsCalls;
    c2();
    std::cout << c2() << std::endl; // prints "2"

    return 0;
}

```

## Non-local lambda function cannot have a capture default

The error message "non-local lambda expression cannot have a capture-default" typically occurs in C++ when you're trying to use a capture-default (= or &) in a lambda expression that is not local to a function.

A capture-default is only allowed for lambdas inside a function.

```cpp
#include <stdio.h>

int g = 10;

auto kitten = [=]() { return g+1; };// Compiler error:  non-local lambda expression cannot have a capture-default
auto cat = [g=g]() { return g + 1; };

int main() {
  g = 20;
  printf("%d %d", kitten(), cat());
}
```


## Captureless lambdas can be converted to raw function pointers

```cpp
#include <stdio.h>
int main(){
    int (*fp)(int) = [](int x) { return x +1; };// saving lambda in a raw function pointer
}
```


## Per-lambda mutable state

**By default mutation of data member is not allowed**

```cpp
#include<iostream>

int main() {

// per_lambda_mutable_state.cpp:7:34: error: cannot assign to a variable captured by copy in a non-mutable lambda
  auto counter = [ctr=0]() { ctr = ctr+1; return ctr; };

  std::cout << "ctr = " << counter() << std::endl;
  std::cout << "ctr = " << counter() << std::endl;

  return 0;
}
```

We have to mark it `mutable` to make it non-const.

```cpp
#include<iostream>

int main() {

  auto counter = [ctr=0]() mutable { ctr = ctr+1; return ctr; };

  std::cout << "ctr = " << counter() << std::endl; // 1
  std::cout << "ctr = " << counter() << std::endl; // 2

  return 0;
}
```

## Use lambdas for complex initialization of const variables

```cpp
#include <iostream>
#include <vector>
int main() {
    // inline const initialization using a immediate lambda
    const std::vector<int> v = [] {
        std::vector<int> v;
        for (int i = 0; i < 10; ++i) {
        v.push_back(i);
        }
        return v;
    }();

    const widget x = [&]{
        widget val;                                // assume that widget has a default constructor
        for (auto i = 2; i <= N; ++i) {            // this could be some
            val += some_obj.do_something_with(i);  // arbitrarily long code
        }                                          // needed to initialize x
        return val;
    }();
}
```

## Expansion insights


### Capture by value

```cpp
#include <cstdio>
int main()
{
  int j = 2;
  	auto isEven = [=](int i)
  {
      printf("j = %d",j);
    return (i % 2) == 0;
  };
}
```

Desugared version on cppinsights.io
```cpp
#include <cstdio>

int main()
{
  int j = 2;
    
  class __lambda_6_18
  {
    public: 
    inline /*constexpr */ bool operator()(int i) const
    {
      printf("j = %d", j);
      return (i % 2) == 0;
    }
    
    private: 
    int j;
    
    public:
    __lambda_6_18(int & _j)
    : j{_j}
    {}
    
  };
  
  __lambda_6_18 isEven = __lambda_6_18{j}; // capture init/copying happens on lambda declaration itself, not waiting till use
  return 0;
}
```

## `this` capture in lambdas, captures the enlosing object

By default, `this` is not captured in lambdas, it's capture needs to be specified explicitly or you will get an error.

```cpp
#include <iostream>

struct MyStruct
{
    void Function()
    {
        auto f = [this]()
        {
            // capturing this and renaming this as self inside the lambda
            auto &self = *this; // this is a pointer to obj
            self.counter++;
            std::cout << " counter inside lambda " << self.counter << std::endl;
        };
        f();
        std::cout << " counter outside lambda " << counter << std::endl;
    }
    int counter{0};
};

int main()
{
    MyStruct instance;
    instance.Function();
    instance.Function();
    instance.Function();
    return 0;
}
```

Or you can make `self` variable before making lambda like `auto &self = *this;` and use `self` inside lambda:
```cpp
class MyClass {
public:
    int value;

    MyClass(int v) : value(v) {}

    void printValue() const {
        auto& self = *this; // Capture `this` as `self` before defining the lambda
        auto lambda = [&self]() {
            std::cout << "Value: " << self.value << std::endl;
        };

        lambda();
    }
};
```

### this capture in lambda

Here, `*this` means the current object, and therefore `&(*this)` denotes a reference to the current object. Also, if a capture clause implicitly or explicitly captures this, conceptually, it captures `&(*this)`.

credits to guide at - https://www.nextptr.com/tutorial/ta1430524603/capture-this-in-lambda-expression-timeline-of-change

![this_capture_in_lambda](./images/thiscaptureinlambdas.png)

## Generic lambdas

Generic lambdas, introduced in C++14, are a powerful feature that allows lambdas to accept arguments of any type. **This is achieved using the `auto` specifier in the parameter list of the lambda.** When combined with `auto&&`, generic lambdas become even more flexible, enabling perfect forwarding and allowing the lambda to handle both lvalues and rvalues efficiently.

### Generic Lambdas

**A generic lambda is a lambda that can accept arguments of any type. This is achieved by using the `auto` specifier in the parameter list.** Here's a simple example:

```cpp
auto add = [](auto a, auto b) {
    return a + b;
};
```

In this example, the lambda `add` can accept arguments of any type that supports the `+` operator. This makes the lambda highly reusable and flexible.

### Type Safety

Generic lambdas are type-safe because the type of the arguments is deduced at compile time. This means that any type mismatches or invalid operations will result in compile-time errors, rather than runtime errors.

For example, consider the following lambda:

```cpp
auto print = [](auto value) {
    std::cout << value << std::endl;
};
```

If you pass an argument to `print` that cannot be streamed to `std::cout`, you will get a compile-time error. This ensures that the lambda is used correctly and helps catch potential bugs early in the development process.

### `auto&&` and Perfect Forwarding

Using `auto&&` in the parameter list of a generic lambda enables perfect forwarding. Perfect forwarding is a technique that allows the lambda to preserve the value category (lvalue or rvalue) of the arguments passed to it. This is important for several reasons:

1. **Efficiency**: Perfect forwarding allows the lambda to avoid unnecessary copies or moves, which can improve performance.
2. **Flexibility**: Perfect forwarding allows the lambda to accept both lvalues and rvalues, making it more versatile.
3. **Compatibility**: Perfect forwarding allows the lambda to work with functions that require specific value categories, such as constructors that accept rvalue references.

Here's an example that demonstrates perfect forwarding with a generic lambda:

```cpp
auto wrap = [](auto&& value) {
    return [value = std::forward<decltype(value)>(value)]() mutable {
        return value;
    };
};
```

In this example, the lambda `wrap` accepts an argument of any type and returns a new lambda that captures the argument by value. The `std::forward` function is used to preserve the value category of the argument.

### Use Cases

Generic lambdas with `auto&&` have many use cases. Here are a few examples:

1. **Utility Functions**: Generic lambdas can be used to create utility functions that work with any type. For example, a lambda that applies a function to a range of values can be implemented as a generic lambda.

```cpp
auto apply = [](auto&& func, auto&& range) {
    for (auto&& value : range) {
        func(value);
    }
};
```

2. **Visitors**: Generic lambdas can be used to implement the visitor pattern, which allows different behavior based on the type of the argument. The example you provided earlier demonstrates this use case.

3. **Wrappers**: Generic lambdas can be used to create wrappers around other functions or lambdas, adding additional behavior or modifying the input or output. The `wrap` example above demonstrates this use case.

4. **Callbacks**: Generic lambdas can be used as callbacks in algorithms or asynchronous operations, allowing the callback to handle any type of input.

```cpp
auto async_operation = [](auto&& callback) {
    // Perform some asynchronous operation
    auto result = perform_operation();
    callback(result);
};
```

In conclusion, generic lambdas with `auto&&` are a powerful feature in modern C++ that provide type safety, efficiency, and flexibility. They have many use cases and can help make code more reusable, maintainable, and expressive.

# Generalized Lambda Capture (Also known as init captures)

Generalized lambda captures, introduced in C++14, enhance the flexibility and expressiveness of lambda expressions by allowing more versatile initialization of the variables captured by a lambda. Prior to C++14, lambda captures were limited in that they could only capture variables by copy or by reference, without the ability to perform arbitrary initialization or transformation during capture. Generalized captures address these limitations, enabling more powerful and concise lambda expressions.

## Table of Contents

- [Generalized Lambda Capture (Also known as init captures)](#generalized-lambda-capture-also-known-as-init-captures)
  - [Table of Contents](#table-of-contents)
  - [Lambda Expressions in C++](#lambda-expressions-in-c)
  - [Limitations Before C++14](#limitations-before-c14)
  - [What Are Generalized Lambda Captures?](#what-are-generalized-lambda-captures)
  - [Syntax of Generalized Lambda Captures](#syntax-of-generalized-lambda-captures)
  - [Use Cases and Examples](#use-cases-and-examples)
    - [Capturing by Move](#capturing-by-move)
    - [Capturing with Initialization](#capturing-with-initialization)
    - [Capturing in Parallel Algorithms](#capturing-in-parallel-algorithms)
    - [Capturing Temporary Objects](#capturing-temporary-objects)
  - [Benefits of Generalized Lambda Captures](#benefits-of-generalized-lambda-captures)
  - [Comparisons with Previous C++ Standards](#comparisons-with-previous-c-standards)
  - [Conclusion](#conclusion)

---

## Lambda Expressions in C++

Lambda expressions are anonymous function objects that can be defined inline. They are particularly useful for short snippets of code that are passed as arguments to algorithms or used as callbacks.

**Basic Syntax:**

```cpp
[capture](parameters) -> return_type {
    // function body
};
```

**Example:**

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    int multiplier = 2;

    std::for_each(numbers.begin(), numbers.end(), [multiplier](int &n) {
        n *= multiplier;
    });

    for(auto n : numbers) {
        std::cout << n << ' '; // Outputs: 2 4 6 8 10
    }
}
```

In this example, the lambda captures the `multiplier` by copy and uses it to modify each element in the vector.

## Limitations Before C++14

Before C++14, lambda captures were limited to:

1. **Capturing Existing Variables:**
   - By value: `[x]`
   - By reference: `[&x]`

2. **No Initialization or Transformation:**
   - You couldn't perform operations or initialize new members during capture.

3. **No Moves:**
   - Capturing by moving a variable (especially useful for unique resources like `std::unique_ptr`) wasn't straightforward.

**Example of Limitation:**

Suppose you want to capture a transformed version of a variable (e.g., initializing a member with a modified value). Prior to C++14, you'd need to capture the original variable and then perform the transformation inside the lambda body, which could be less efficient or less elegant.

## What Are Generalized Lambda Captures?

Generalized lambda captures extend the capture clause to allow initializing new variables directly within the lambda’s capture list. This means you can create new variables with custom initialization logic, including moving variables, constructing new objects, or computing expressions on the fly.

Introduced in C++14, generalized lambda captures expand the capture clause from simply capturing existing variables to allowing any arbitrary expression to be evaluated and stored as a member of the lambda’s closure type.

## Syntax of Generalized Lambda Captures

Generalized captures expand the `capture` list to allow initialization using the following syntax:

- **Explicit Initialization:**
  ```cpp
  [new_var = expression]
  ```
- **Implicit Initialization:**
  - Similar to pre-C++14, but generalized to include new variables.

**Examples:**

- ` [x = std::move(y)]`
- ` [z = x + y]`
- ` [ptr = std::make_unique<int>(42)]`

These expressions initialize `new_var` within the closure by evaluating `expression` at the point of lambda creation.

## Use Cases and Examples

### Capturing by Move

One common use case is capturing a `std::unique_ptr` or other move-only types by moving them into the lambda.

**Example:**

```cpp
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

int main() {
    std::vector<std::unique_ptr<int>> vec;
    vec.emplace_back(std::make_unique<int>(1));
    vec.emplace_back(std::make_unique<int>(2));

    // Move unique_ptrs into a new vector using a lambda
    std::vector<std::unique_ptr<int>> moved_vec;
    std::for_each(std::make_move_iterator(vec.begin()), std::make_move_iterator(vec.end()),
        [&](std::unique_ptr<int> &&ptr) {
            moved_vec.emplace_back(std::move(ptr));
        }
    );

    // Using a lambda with generalized lambda capture
    auto print = [ptr = std::make_unique<int>(10)]() {
        std::cout << *ptr << '\n';
    };

    print(); // Outputs: 10
}
```

In the first part, `std::make_move_iterator` is used to move the `unique_ptr`s from `vec` to `moved_vec`. In the second part, a lambda captures a `unique_ptr` by moving `std::make_unique<int>(10)` directly into the lambda.

### Capturing with Initialization

You can initialize new variables inside the lambda, thus avoiding the need to separately define and capture them.

**Example:**

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5};

    // Capture the sum of all elements
    int total = 0;
    auto sum_lambda = [&total](int n) { total += n; };
    std::for_each(nums.begin(), nums.end(), sum_lambda);
    std::cout << "Total: " << total << '\n'; // Total: 15

    // Using generalized lambda capture to compute total within lambda
    auto print_total = [total = std::accumulate(nums.begin(), nums.end(), 0)]() {
        std::cout << "Total: " << total << '\n';
    };
    print_total(); // Total: 15
}
```

Here, instead of capturing `total` by reference and updating it in the lambda, the lambda directly captures a computed `total` value using `std::accumulate`.

### Capturing in Parallel Algorithms

When using parallel algorithms or asynchronous operations, capturing necessary state efficiently is crucial. Generalized captures can help encapsulate the state needed for each operation.

**Example:**

```cpp
#include <iostream>
#include <vector>
#include <future>

int main() {
    std::vector<int> data = {1, 2, 3, 4, 5};

    // Capture a computed factor within the lambda
    auto factor = 10;
    auto futures = std::vector<std::future<int>>{};
    
    for(auto x : data) {
        futures.emplace_back(std::async(std::launch::async, [scaled = x * factor]() -> int {
            return scaled;
        }));
    }

    for(auto &fut : futures) {
        std::cout << fut.get() << ' '; // Outputs: 10 20 30 40 50
    }
}
```

In this example, each lambda captures `scaled`, which is `x * factor`, directly within the capture list.

### Capturing Temporary Objects

Generalized captures allow capturing temporary objects that wouldn’t be possible to capture otherwise.

**Example:**

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main() {
    std::vector<std::string> words = {"apple", "banana", "cherry"};

    // Capture a transformed version of each word (uppercase)
    std::vector<std::string> uppercased;
    std::transform(words.begin(), words.end(), std::back_inserter(uppercased),
        [captured = std::string{}](const std::string &word) mutable -> std::string {
            captured = word;
            for(auto &c : captured) c = toupper(c);
            return captured;
        }
    );

    for(const auto &word : uppercased) {
        std::cout << word << ' '; // Outputs: APPLE BANANA CHERRY
    }
}
```

Here, `captured` is a temporary string that is modified within the lambda.

## Benefits of Generalized Lambda Captures

1. **Increased Flexibility:** Ability to initialize new variables within the capture list, enabling more complex initializations and transformations.
2. **Move Semantics:** Easily move objects into the lambda, which is essential for managing resources like `std::unique_ptr`.
3. **Atomic Capture:** Capture expressions are evaluated exactly once at the lambda’s creation, avoiding potential multiple evaluations.
4. **Cleaner Code:** Reduces boilerplate by allowing initialization directly within the capture, leading to more concise and readable code.

## Comparisons with Previous C++ Standards

**C++11 vs. C++14:**

- **C++11:**
  - Simple captures by value or reference.
  - Cannot directly move objects or initialize new captures within the lambda.

- **C++14:**
  - Introduces generalized lambda captures.
  - Allows initialization of new variables in the capture list using arbitrary expressions.

**Example Comparison:**

C++11:

```cpp
auto ptr = std::make_unique<int>(42);
auto lambda = [ptr = std::move(ptr)]() { // Error in C++11
    // ...
};
```

C++14:

```cpp
auto ptr = std::make_unique<int>(42);
auto lambda = [ptr = std::move(ptr)]() {
    // Now ptr is owned by the lambda
};
```

In C++11, the above code would not compile because lambda captures couldn’t include initialization expressions like `ptr = std::move(ptr)`. C++14 allows this syntax, facilitating the capture and move of `std::unique_ptr`.

## Conclusion

Generalized lambda captures significantly enhance the power and expressiveness of lambda expressions in C++. By allowing arbitrary initialization expressions within the capture list, developers can write more concise, efficient, and readable code. This feature is especially beneficial when working with move-only types, complex initializations, and scenarios requiring encapsulation of temporary or computed state within lambdas.

Understanding and utilizing generalized lambda captures can lead to better resource management, cleaner code architecture, and more effective use of modern C++ features in your applications.