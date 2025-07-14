#include "actors/fighter.hpp"
#include "arena.hpp"
#include <algorithm>

using namespace lulu;

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

/**
 * @brief Get all colliding fighters from current collision map
 */
std::vector<Fighter *> Fighter::getCollidingFighters() const
{
    std::vector<Fighter *> collidingFighters;

    if (!_arena)
        return collidingFighters;

    const auto &collisionMap = _arena->collisions();
    auto it = collisionMap.find(std::map<Actor *, std::vector<collision>>::key_type(this));

    if (it != collisionMap.end())
    {
        for (const auto &[target, type] : it->second)
        {
            if (auto *fighter = dynamic_cast<Fighter *>(target))
            {
                collidingFighters.push_back(fighter);
            }
        }
    }

    return collidingFighters;
}

/**
 * @brief Check if actor is colliding with any static objects
 */
bool Fighter::isCollidingWithStatic() const
{
    if (!_arena)
        return false;

    const auto &collisionMap = _arena->collisions();
    auto it = collisionMap.find(std::map<Actor *, std::vector<collision>>::key_type(this));

    if (it != collisionMap.end())
    {
        for (const auto &[target, type] : it->second)
        {
            // Check if target is not a Movable (i.e., static)
            if (!dynamic_cast<Movable *>(target))
            {
                return true;
            }
        }
    }

    return false;
}

/**
 * @brief Deal damage to all colliding fighters
 */
void Fighter::damageCollidingFighters() const
{
    for (const auto collidingFighters = getCollidingFighters(); auto *fighter : collidingFighters)
    {
        attack(*fighter);
    }
}