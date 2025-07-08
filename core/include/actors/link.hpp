#pragma once
#include "actors/fighter.hpp"

namespace lulu
{
class Link final : public Fighter
{
private:
    void setupAnimations();
    direction parseInput() const;
    pair calculateMovement(direction dir) const;

public:
    Link(const pair position, const pair size, float speed, float hp, float damage, Arena *arena = nullptr);

    void move() override;
    void handleCollisions(const std::vector<collision> &collisions) override;
};
} // namespace lulu