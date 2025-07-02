#pragma once
#include "actors/fighter.hpp"

namespace lulu
{
class Link final : public Fighter
{
  public:
    Link(const pair position, const pair size, const float speed, const float hp, const float damage, Arena *arena = nullptr, const std::string &sprite = "")
        : Fighter(position, size, pair{speed, speed}, hp, damage, arena, sprite) {};

    void move() override;
    void handleCollisions(const std::vector<collision> &collisions) override;
};
} // namespace lulu
