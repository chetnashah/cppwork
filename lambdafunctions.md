
lambdas are anonymous functions

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
