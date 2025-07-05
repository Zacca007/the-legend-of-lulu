#pragma once
#include "actors/fighter.hpp"

#include <bits/ios_base.h>
#include <ios>

namespace lulu
{

class Link final : public Fighter
{

  public:
    Link(const pair position, const pair size, const float speed, const float hp, const float damage, Arena *arena = nullptr)
        : Fighter(position, size, pair{speed, speed}, hp, damage, arena)
    {
        std::vector<std::string> up = {"core/assets/link/link up 1.png", "core/assets/link/link up 2.png"};
        std::vector<std::string> down = {"core/assets/link/link front 1.png", "core/assets/link/link front 2.png"};
        std::vector<std::string> left = {"core/assets/link/link left 1.png", "core/assets/link/link left 2.png"};
        std::vector<std::string> right = {"core/assets/link/link right 1.png", "core/assets/link/link right 2.png"};

        _animation.addAnimation(S_MOVEMENT, D_UP, up);
        _animation.addAnimation(S_MOVEMENT, D_UPLEFT, up);
        _animation.addAnimation(S_MOVEMENT, D_UPRIGHT, up);

        _animation.addAnimation(S_MOVEMENT, D_DOWN, down);
        _animation.addAnimation(S_MOVEMENT, D_DOWNLEFT, down);
        _animation.addAnimation(S_MOVEMENT, D_DOWNRIGHT, down);

        _animation.addAnimation(S_MOVEMENT, D_LEFT, left);
        _animation.addAnimation(S_MOVEMENT, D_RIGHT, right);

        _animation.set(S_MOVEMENT, D_UP);
        _sprite = _animation.nextSprite();
    }

    void move() override;
    void handleCollisions(const std::vector<collision> &collisions) override;
};

} // namespace lulu