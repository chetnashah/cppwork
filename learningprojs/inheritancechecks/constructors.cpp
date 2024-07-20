#include <iostream>
#include <string>

class EntityBase
{
public:
    int m_id;
    EntityBase()
    {
        std::cout << "EntityBase empty args Constructor\n";
    }
    EntityBase(int id) : m_id(id)
    {
        std::cout << "EntityBase single arg Constructor id = " << id << "\n";
    }
    int getId() const { return m_id; }
};

class Monster : public EntityBase
{
public:
    double m_health;
    Monster()
    {
        std::cout << "Monster empty args Constructor\n";
    }
    Monster(double health) : EntityBase(12), m_health(health)
    {
        std::cout << "Monster single arg Constructor health = " << health << "\n";
    }
    double getHealth() const { return m_health; }
};

int main()
{
    std::cout << "Instantiating EntityBase\n";
    EntityBase entityBase; // "EntityBase empty args Constructor" is printed

    std::cout << "Instantiating Monster\n";
    Monster monster{123}; // "EntityBase empty args Constructor" is printed first, then "Monster empty args Constructor"

    return 0;
}