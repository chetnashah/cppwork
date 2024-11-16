#include <iostream>
const std::string &getConstRef(); // some function that returns a reference to const

void autoDoesNotDropPointers()
{
    const int *p = nullptr;
    auto a = p; // a is const int*
}

void autodroppingconst()
{
    const int k = 1;
    auto a = k; // a is int (const dropped), copied from k
    a = 11;
    std::cout << "a = " << a << std::endl;
    std::cout << "k = " << k << std::endl;
}

void autodroppingreference()
{
    int x = 1;
    int &y = x;
    auto &a = y; // a is int (reference dropped), copied from y

    if (std::is_reference_v<decltype(a)>)
        std::cout << "a is a reference" << std::endl;
    else
        std::cout << "a is not a reference" << std::endl;
}

void autodroppingconstandref()
{
    const int k = 1;
    const int &kr = k;
    auto a = kr;                                                          // a is int (const and reference dropped), copied from kr
    auto isconst = std::is_const_v<std::remove_reference_t<decltype(a)>>; // false
    std::cout << "a is const? = " << std::boolalpha << isconst << std::endl;

    auto isref = std::is_reference_v<std::remove_const<decltype(a)>>; // false
    std::cout << "a is a reference? = " << std::boolalpha << isref << std::endl;
}

int main()
{
    autodroppingconst();
    autodroppingreference();
    autodroppingconstandref();
    const int val = 5;
    const int *ptr = &val;

    auto p1 = ptr;  // 'const int*'
    auto p2 = &ptr; // 'const int**'
    auto &p3 = ptr; // 'const int*&'

    return 0;
}
