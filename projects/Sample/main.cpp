#include <cstdio>
class Person
{
public:
    Person() = default;
    Person(const Person &pp)
    {
        printf("copy constructor invoked");
        m = pp.m + 2;
    }

    Person operator=(const Person &pp)
    {
        printf("copy assignment invokd");
        m = pp.m + 100;
    }
    int m = 0;
};

int main()
{
    Person p;
    Person p2(p);
    Person p3;
    p3 = p2;
    auto p4 = p3;
    printf("p = %d, p2 = %d, p3 = %d, p4 = %d", p.m, p2.m, p3.m, p4.m);
}
