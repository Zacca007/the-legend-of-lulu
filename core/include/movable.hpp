#pragma once
#include "animationHandler.hpp"
#include "types.hpp"
#include <vector>
namespace lulu
{
class Movable
{
  protected:
    pair _speed;
    AnimationHandler _animation;

    explicit Movable(const pair speed, bool enableAnimation = false) : _speed(speed)
    {
        if (enableAnimation)
            _animation.enable();
    }

  public:
    virtual ~Movable() = default;

    [[nodiscard]] const pair &speed() const
    {
        return _speed;
    }

    virtual void move() = 0;
    virtual void handleCollisions(const std::vector<collision> &collisions) = 0;
};
} // namespace lulu
