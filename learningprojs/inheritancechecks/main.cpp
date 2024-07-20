#include <iostream>

class Base
{
public:
    int m_id{};

    Base(int id = 0)
        : m_id(id)
    {
        std::cout << "Base Constructor\n";
    }
    int getId() const { return m_id; }
};

class Derived : public Base
{
public:
    double m_cost{};
    // Implicit call is made to Base Constructor,
    // because it has a no-arg constructor/default
    // constructor/constructor with default values
    Derived(double cost = 0.0)
        : m_cost(cost)
    {
        std::cout << "Derived Constructor\n";
    }
    double getCost() const { return m_cost; }
};

int main()
{
    std::cout << "Instantiating Base\n";
    Base base; // "Base Constructor" is printed

    std::cout << "Instantiating Derived\n";
    Derived derived; // "Base Constructor" is printed first, then "Derived Constructor

    return 0;
}