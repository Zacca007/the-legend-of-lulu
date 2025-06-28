#include "actors/fighter.hpp"
using lulu::Fighter;

void Fighter::takeDamage(float damage)
{
    _HP -= damage;
};

void Fighter::attack(Fighter &fighter) const
{
    fighter.takeDamage(_damage);
}

bool Fighter::isAlive() const
{
    return _HP > 0;
}