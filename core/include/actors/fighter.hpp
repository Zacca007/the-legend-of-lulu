#pragma once
#include "actor.hpp"
#include "movable.hpp"
#include "types.hpp"

namespace lulu
{
class Fighter : public Actor, public Movable
{
  protected:
    float hp_;     // Current health points
    float damage_; // Damage dealt per attack

    virtual void setupAttack() = 0;
    virtual void performAttack() = 0;
    virtual void endAttack() = 0;

    Fighter(pair position, pair size, pair speed, float hp, float damage,
            Arena *arena = nullptr, const std::string &sprite = "");


  public:
    [[nodiscard]] bool isAlive() const;
    [[nodiscard]] float hp() const;
    [[nodiscard]] float damage() const;

    void takeDamage(float damage);
    void attack(Fighter &fighter) const;
};
}