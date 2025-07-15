#pragma once
#include "actors/fighter.hpp"

#include <limits>

namespace lulu
{
class TrapBlade final : public Fighter
{
    protected:
    void performAttack() override;
    void endAttack() override;
  public:
    TrapBlade(const pair position, const pair size, const float speed, const float damage, Arena *arena = nullptr,
              const std::string &sprite = "core/assets/enemies/spines.png")
        : Fighter(position, size, {speed, speed}, std::numeric_limits<float>::infinity(), damage, arena, sprite) {}

    void move() override;
    void handleCollisions(const std::vector<collision> &collisions) override;
};
} // namespace lulu