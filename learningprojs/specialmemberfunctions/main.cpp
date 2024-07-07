#include <string>
#include <iostream>

class NoDefaultCtor
{
public:
    NoDefaultCtor(int i) : i(i) {}

    void printI() { std::cout << i << std::endl; }

private:
    int i;
};

class NotCopyable
{
public:
    NotCopyable() = default;
    NotCopyable(const NotCopyable &) = delete;
    NotCopyable &operator=(const NotCopyable &) = delete;
};

class B
{
public:
    B() = default;
    NotCopyable nc; // as this is not copyable, B is not copyable (implictly deleted copy ops)
};

class Regular
{
public:
    // Regular() {}
    int i;
    std::string s;
};

class Widget
{
public:
private:
    NoDefaultCtor ndc;
    Regular r;
};

int main()
{
    // Widget w;

    Regular r{};
    std::cout << "" << r.i << std::endl;
    std::cout << "" << r.s << std::endl;

    B b{};
    B b2 = b;

    return 0;
    // NoDefaultCtor ndc; // Error: no default constructor
}
