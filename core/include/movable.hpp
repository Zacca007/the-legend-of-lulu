#pragma once
#include "animationHandler.hpp"
#include "types.hpp"

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
    // attributes
    pair speed_;                 // Movement speed (x, y components)
    AnimationHandler movement_; // Animation handler for sprite management

    // constructors
    explicit Movable(pair speed, bool enableAnimation = false);

    // internal movement methods
    virtual void setupAnimation() = 0;
    [[nodiscard]] virtual state updatedState() const = 0;
    [[nodiscard]] virtual direction updatedDirection() const = 0;
    [[nodiscard]] virtual pair calculateMovement(direction dir) const = 0;

  public:
    virtual ~Movable() = default;

    // getter
    [[nodiscard]] const pair &speed() const;

    // movement methods (used in arena)
    virtual void move() = 0;
    virtual void handleCollisions(const std::vector<collision> &collisions) = 0;
};
} // namespace lulu