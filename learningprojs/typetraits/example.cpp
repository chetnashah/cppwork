#include <iostream>
#include <type_traits>

template <typename T>
void checkIntegral(T t)
{
    if (std::is_integral<T>::value)
    {
        std::cout << "T is an integral type" << std::endl;
    }
    else
    {
        std::cout << "T is not an integral type" << std::endl;
    }
}

template <typename T>
void checkIfPointer(T t)
{
    if (std::is_pointer<T>::value)
    {
        std::cout << "T is a pointer type" << std::endl;
    }
    else
    {
        std::cout << "T is not a pointer type" << std::endl;
    }
}

int main()
{
    checkIntegral(5);
    checkIntegral(5.0);
    checkIfPointer(5);
    checkIfPointer(nullptr);
    checkIfPointer("Hello");
    checkIfPointer(new int(5));
    return 0;
}