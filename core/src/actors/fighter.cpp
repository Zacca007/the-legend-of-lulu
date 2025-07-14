#include "actors/fighter.hpp"
using lulu::Fighter;

/**
 * @brief Reduce health points by damage amount
 */
void Fighter::takeDamage(const float damage)
{
    _hp -= damage;
}

/**
 * @brief Attack another fighter, dealing damage
 */
void Fighter::attack(Fighter &fighter) const
{
    fighter.takeDamage(_damage);
}

/**
 * @brief Check if fighter is still alive
 */
bool Fighter::isAlive() const
{
    return _hp > 0;
}