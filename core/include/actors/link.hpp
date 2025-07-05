#pragma once
#include "actors/fighter.hpp"

namespace lulu
{

class Link final : public Fighter
{
    inline static animations _down = {"core/assets/link/link front 1.png", "core/assets/link/link front 2.png"};
    inline static animations _up = {"core/assets/link/link up 1.png", "core/assets/link/link up 2.png"};
    inline static animations _left = {"core/assets/link/link left 1.png", "core/assets/link/link left 2.png"};
    inline static animations _right = {"core/assets/link/link right 1.png", "core/assets/link/link right 2.png"};
    bool _animationCounter;
  public:
    Link(const pair position, const pair size, const float speed, const float hp, const float damage, Arena *arena = nullptr)
        : Fighter(position, size, pair{speed, speed}, hp, damage, arena, _down.first), _animationCounter(false) {};

    void move() override;
    void handleCollisions(const std::vector<collision> &collisions) override;
};
} // namespace lulu
