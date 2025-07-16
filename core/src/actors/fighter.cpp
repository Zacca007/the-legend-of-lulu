#include "actors/fighter.hpp"
namespace lulu
{
Fighter::Fighter(const pair position, const pair size, const pair speed, const float hp, const float damage,
                 Arena *arena, const std::string &sprite)
    : Actor(position, size, arena, sprite), Movable(speed, true), _hp(hp), _damage(damage)
{
}

void Fighter::takeDamage(const float damage)
{
    _hp -= damage;
}

void Fighter::attack(Fighter &fighter) const
{
    fighter.takeDamage(_damage);
}

bool Fighter::isAlive() const
{
    return _hp > 0;
}

float Fighter::hp() const
{
    return _hp;
}

float Fighter::damage() const
{
    return _damage;
}

} // namespace lulu