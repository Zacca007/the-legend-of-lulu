#pragma once
#include "actors/fighter.hpp"

namespace lulu
{
/**
 * @brief Main player character class
 * Link is the player-controlled character with movement, attack mechanics,
 * and complex animation handling. Inherits combat abilities from Fighter.
 */
class Link final : public Fighter
{
    std::uint8_t attackFrame_;     // Current frame of attack animation
    std::uint8_t previousFrame_; // Previous animation frame (for restoration)
    long long int animationSwitch_;

    void setupAnimation() override;
    state updatedState() const override;
    direction updatedDirection() const override;
    pair calculateMovement(direction dir) const override;

    void setupAttack() override;
    void performAttack() override;
    void endAttack() override;
    void adjustPositionForAttack(const pair &sizeDiff);


  public:
    Link(pair position, pair size, float speed, float hp, float damage, Arena *arena = nullptr);

    void move() override;
    void handleCollisions(const std::vector<collision> &collisions) override;
};
} // namespace lulu