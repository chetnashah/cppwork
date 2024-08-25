#include <iostream>

struct MyStruct
{
    void Function()
    {
        auto f = [this]()
        {
            auto &obj = *this; // this is a pointer
            obj.counter++;
            std::cout << " counter inside lambda " << obj.counter << std::endl;
        };
        f();
        std::cout << " counter outside lambda " << counter << std::endl;
    }
    int counter{0};
};

int main()
{
    MyStruct instance;
    instance.Function();
    instance.Function();
    instance.Function();
    return 0;
}