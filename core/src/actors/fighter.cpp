#include "actors/fighter.hpp"
namespace lulu
{
Fighter::Fighter(const pair position, const pair size, const pair speed, const float hp, const float damage,
                 Arena *arena, const std::string &sprite)
    : Actor(position, size, arena, sprite), Movable(speed, true), hp_(hp), damage_(damage)
{
}

void Fighter::takeDamage(const float damage)
{
    hp_ -= damage;
}

void Fighter::attack(Fighter &fighter) const
{
    fighter.takeDamage(damage_);
}

bool Fighter::isAlive() const
{
    return hp_ > 0;
}

float Fighter::hp() const
{
    return hp_;
}

float Fighter::damage() const
{
    return damage_;
}

} // namespace lulu