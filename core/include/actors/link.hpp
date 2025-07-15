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
    void performAttack() override;

    /**
     * @brief End attack sequence and restore normal state
     */
    void endAttack() override;

    /**
     * @brief Handle sprite size changes during attack animations
     * @param sizeDiff Difference in size between current and new sprite
     */
    void adjustPositionForAttack(const pair &sizeDiff);

    // INPUT HANDLING METHODS - specific to player-controlled entities

    /**
     * @brief Check if input keys contain a specific key
     * @param keys Vector of keys to check
     * @param key Key to look for
     * @return True if key is found
     */
    static bool hasKey(const std::vector<Key> &keys, Key key);

    /**
     * @brief Check if a key was just pressed (not held from previous frame)
     * @param key Key to check
     * @return True if key was just pressed
     */
    [[nodiscard]] bool isKeyJustPressed(Key key) const;

    /**
     * @brief Get current directional input from arena
     * @return Direction based on current key presses
     */
    [[nodiscard]] direction getCurrentDirection() const;

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