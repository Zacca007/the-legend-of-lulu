#pragma once
#include "types.hpp"
#include "actor.hpp"
#include "movable.hpp"

namespace lulu
{

    class Fighter : public Actor, public Movable
    {
    protected:
        float _HP;
        float _damage;

        Fighter(pair position, pair size, pair speed, Arena *arena = nullptr, const std::string &sprite = "")
            : Actor(position, size, arena, sprite), Movable(speed) {}

    public:
        void takeDamage(float damage);
        void attack(Fighter &fighter) const;
        bool isAlive() const;
    };
}
