#include "actors/fighter.hpp"
using lulu::Fighter;

void Fighter::takeDamage(const float damage)
{
    _hp -= damage;
};
void Fighter::attack(Fighter &fighter) const
{
    fighter.takeDamage(_damage);
}
bool Fighter::isAlive() const
{
    return _hp > 0;
}
