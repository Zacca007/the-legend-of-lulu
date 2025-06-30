#pragma once
#include "actor.hpp"
#include "movable.hpp"
#include "types.hpp"

namespace lulu
{
class Fighter : public Actor, public Movable
{
  protected:
    float _hp;
    float _damage;

    Fighter(const pair position, const pair size, const pair speed, const float hp, const float damage, Arena *arena = nullptr, const std::string &sprite = "")
        : Actor(position, size, arena, sprite), Movable(speed), _hp(hp), _damage(damage) {}

  public:
    void takeDamage(float damage);
    void attack(Fighter &fighter) const;
    [[nodiscard]] bool isAlive() const;
};
} // namespace lulu
