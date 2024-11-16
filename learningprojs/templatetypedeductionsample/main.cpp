
#include <iostream>
#include <type_traits>

template <typename T>
class TD; // Type Displayer

template <typename T>
void f2(T &param)
{
    TD<T> tType;                   // shows me T
    TD<decltype(param)> paramType; // shows me the type of param
}

template <typename T>
void f(T param)
{
    bool isRef = std::is_reference_v<T>;
    std::cout << "isRef = " << isRef << std::endl;
    bool isConst = std::is_const_v<std::remove_reference_t<T>>;
    std::cout << "isConst = " << isConst << std::endl;
    // param is a copy of the argument passed in
}

int main()
{
    int x = 27;
    const int cx = x;
    const int &rx = x;

    // f2(x);  // T is int, param's type is int
    f2(cx); // T is int, param's type is int
    // f2(rx);
    return 0;
}