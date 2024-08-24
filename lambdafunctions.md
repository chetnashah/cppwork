
lambdas are anonymous functions

https://learn.microsoft.com/en-us/cpp/cpp/lambda-expressions-in-cpp?view=msvc-170&viewFallbackFrom=vs-2019

 a function object is an instance of a class, for which the call operator ( operator`()` ) is overloaded. This means that a function object is an object that behaves like a function. The main difference between a function and a function object is: a function object is an object and can, therefore, have stated.

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