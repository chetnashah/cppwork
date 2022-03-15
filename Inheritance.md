
**Inheritance of classes is private by default, you should make it public**

https://www.youtube.com/watch?v=77eueMbWI0Y

### Classic example


Base instantiation happens before derived constructor instantiation.

C++ constructs derived classes in phases, starting with the most-base class (at the top of the inheritance tree) and finishing with the most-child class (at the bottom of the inheritance tree). As each class is constructed, the appropriate constructor from that class is called to initialize that part of the class.



```cpp

#include <iostream>

class Base
{
public:
    int m_id {};

    Base(int id=0)
        : m_id(id)
    {
        std::cout << "Base\n";
    }

    int getId() const { return m_id; }
};

class Derived: public Base
{
public:
    double m_cost {};
    Derived(double cost=0.0)
        : m_cost(cost)
    {
        std::cout << "Derived\n";
    }
    double getCost() const { return m_cost; }
};

int main()
{
    std::cout << "Instantiating Base\n";
    Base base;

    std::cout << "Instantiating Derived\n";
    Derived derived;

    return 0;
}
```

### init problems

C++ prevents classes from initializing inherited member variables in the member initializer list of a constructor. 

In other words, the value of a member variable can only be set in a member initializer list of a constructor belonging to the same class as the variable.
e.g. in above example, you cannot init `m_id` in derived class code.

another possibility:
inherited variables can still have their values changed in the body of the constructor using an assignment.

problem with it:
it wouldn’t work if `m_id` were a const or a reference (because const values and references have to be initialized in the member initializer list of the constructor). 

It’s also inefficient because `m_id` gets assigned a value twice: once in the member initializer list of the Base class constructor, and then again in the body of the Derived class constructor. 

And finally, what if the Base class needed access to this value during construction? It has no way to access it, since it’s not set until the Derived constructor is executed (which pretty much happens last).

Correct way:
```cpp
class Derived: public Base
{
public:
    double m_cost {};

    Derived(double cost=0.0, int id=0)
        : Base{ id } // Call Base(int) constructor with value id!
        , m_cost{ cost }
    {
    }

    double getCost() const { return m_cost; }
};
```

### Re-using base functions

```cpp
class Base
{
protected:
    int m_value {};

public:
    Base(int value)
        : m_value { value }
    {
    }

    void identify() const { std::cout << "I am a Base\n"; }
};

class Derived: public Base
{
public:
    Derived(int value)
        : Base { value }
    {
    }

    int getValue() const  { return m_value; }

    void identify() const
    {
        Base::identify(); // call Base::identify() first
        std::cout << "I am a Derived\n"; // then identify ourselves
    }
};

int main()
{
    Base base { 5 };
    base.identify();

    Derived derived { 7 };
    derived.identify();

    return 0;
}
```


### Virtual functions

