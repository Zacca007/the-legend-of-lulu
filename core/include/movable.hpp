#pragma once
#include "animationHandler.hpp"
#include "types.hpp"
#include <vector>

namespace lulu
{
/**
 * @brief Interface for movable actors
 *
 * Movable is an abstract class that provides movement capabilities
 * and animation handling for actors that can move around the game world.
 */
class Movable
{
  protected:
    pair _speed;                 // Movement speed (x, y components)
    AnimationHandler _animation; // Animation handler for sprite management

    /**
     * @brief Construct a new Movable
     * @param speed Movement speed
     * @param enableAnimation Whether to enable animation system
     */
    explicit Movable(const pair speed, const bool enableAnimation = false) : _speed(speed)
    {
        if (enableAnimation)
            _animation.enable();
    }

    /**
     * @brief Calculate movement vector for given direction and speed
     * @param dir Direction to move
     * @param speedMultiplier Speed multiplier (defaults to 1.0f)
     * @return Movement vector (delta position)
     */
    [[nodiscard]] pair calculateMovement(direction dir, float speedMultiplier = 1.0f) const;

  public:
    virtual ~Movable() = default;

    [[nodiscard]] const pair &speed() const
    {
        return _speed;
    }

    /**
     * @brief Update actor position and state
     * Pure virtual function that must be implemented by derived classes
     */
    virtual void move() = 0;

    /**
     * @brief Handle collision responses
     * @param collisions Vector of collisions to handle
     */
    virtual void handleCollisions(const std::vector<collision> &collisions) = 0;
};
}