
## Functions can be top level in c++

Functions can be top-level in C++ and their scoping/linkage is managed through several mechanisms:

**Top-level functions have external linkage by default, and hence go in global namespace by default**
**Top-level functions are generally a bad idea due to conflicts possible in global namespace**

The global namespace can be explicitly referred to using the scope resolution operator :: with nothing before it. This is particularly useful when you need to disambiguate between global and local/namespaced functions.

```cpp
void foo() { }                  // Global namespace

namespace Bar {
    void foo() { }             // Bar namespace
}

::foo();                       // Calls global foo
Bar::foo();                    // Calls Bar's foo
```

1. Default linkage: 
```cpp
void func() { }  // External linkage by default
```

2. Explicit linkage specifications:
```cpp
static void func() { }      // Internal linkage
extern "C" void func() { }  // C linkage
```

3. Anonymous namespace (makes symbols internal):
```cpp
namespace {
    void func() { }  // Internal linkage
}
```

4. Named namespaces:
```cpp
namespace MyNamespace {
    void func() { }  // External linkage, scoped to namespace
}
```

The main rules are:
- Top-level functions have external linkage by default
- `static` makes them internal (visible only in current translation unit)
- Functions in anonymous namespaces have internal linkage
- Functions can be placed in namespaces to manage scope
- `extern` can be used to declare functions defined elsewhere


### Virtual functions

The virtual specifier specifies that a non-static member function is virtual and supports dynamic dispatch

Virtual functions are member functions whose behavior can be overridden in derived classes. As opposed to non-virtual functions, the overriding behavior is preserved even if there is no compile-time information about the actual type of the class. That is to say, if a derived class is handled using pointer or reference to the base class, a call to an overridden virtual function would invoke the behavior defined in the derived class. Such a function call is known as virtual function call or virtual call.

```cpp
#include <iostream>
 
struct Base
{
    virtual void f()
    {
        std::cout << "base\n";
    }
};
 
struct Derived : Base
{
    void f() override // 'override' is optional
    {
        std::cout << "derived\n";
    }
};
 
int main()
{
    Base b;
    Derived d;
 
    // virtual function call through reference
    Base& br = b; // the type of br is Base&
    Base& dr = d; // the type of dr is Base& as well
    br.f(); // prints "base"
    dr.f(); // prints "derived"
 
    // virtual function call through pointer
    Base* bp = &b; // the type of bp is Base*
    Base* dp = &d; // the type of dp is Base* as well
    bp->f(); // prints "base"
    dp->f(); // prints "derived"
 
    // non-virtual function call
    br.Base::f(); // prints "base"
    dr.Base::f(); // prints "base"
}
```