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
    bool _isAttacking;             // Whether currently performing attack
    std::uint8_t _attackFrame;     // Current frame of attack animation
    std::uint8_t _previousFrame{}; // Previous animation frame (for restoration)
    long long int _animationSwitch;

    /**
     * @brief Initialize all animation sequences for Link
     */
    void setupAnimations();

    /**
     * @brief Determine current animation state based on input
     * @return Current state (still, movement, or attack)
     */
    [[nodiscard]] state updateState() const;

    /**
     * @brief Initialize attack sequence
     */
    void setupAttack();

    /**
     * @brief Execute attack frame and handle damage
     */
    void performAttack();

    /**
     * @brief End attack sequence and restore normal state
     */
    void endAttack();

    /**
     * @brief Handle sprite size changes during attack animations
     * @param sizeDiff Difference in size between current and new sprite
     */
    void adjustPositionForAttack(const pair &sizeDiff);

  public:
    /**
     * @brief Construct Link character
     * @param position Initial position
     * @param size Character size
     * @param speed Movement speed
     * @param hp Initial health points
     * @param damage Attack damage
     * @param arena Pointer to arena
     */
    Link(pair position, pair size, float speed, float hp, float damage, Arena *arena = nullptr);

    /**
     * @brief Update Link's position and animation
     * Override from Movable base class
     */
    void move() override;

    /**
     * @brief Handle collision responses
     * @param collisions Vector of collisions to resolve
     */
    void handleCollisions(const std::vector<collision> &collisions) override;

    /**
     * @brief Check if Link is currently attacking
     * @return True if in attack state
     */
    [[nodiscard]] bool isAttacking() const
    {
        return _isAttacking;
    }
};
} // namespace lulu