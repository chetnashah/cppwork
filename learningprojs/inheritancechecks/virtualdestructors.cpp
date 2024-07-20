#include <iostream>

class Base
{
public:
    Base() { std::cout << "Base class constructor" << std::endl; }
    // useful to declare virtual as base reference/pointer going out of scope
    // should be able to delete derived object via derived destructor
    virtual ~Base() { std::cout << "Base class destructor" << std::endl; }
};

class Derived : public Base
{
public:
    Derived() { std::cout << "Derived class constructor" << std::endl; }
    ~Derived() { std::cout << "Derived class destructor" << std::endl; }
};

void doSomething()
{
    std::unique_ptr<Base> instance = std::make_unique<Derived>();
}

int main()
{
    doSomething();
    return 0;
}