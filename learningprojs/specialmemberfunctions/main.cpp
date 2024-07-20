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

class MovableClass
{
public:
    MovableClass(std::unique_ptr<int> arg) : ptr(std::move(arg)){};
    // MovableClass(const MovableClass &) = delete;
    // MovableClass &operator=(const MovableClass &) = delete;

private:
    std::unique_ptr<int> ptr;
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

    // B b{};
    // B b2 = b;

    std::unique_ptr<int> ptr = std::make_unique<int>(10);
    MovableClass mc(std::move(ptr));

    MovableClass mc2 = ptr;

    return 0;
    // NoDefaultCtor ndc; // Error: no default constructor
}
