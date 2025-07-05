#pragma once
#include "types.hpp"
#include <vector>
namespace lulu
{
class Movable
{
  protected:
    using animations = std::pair<std::string, std::string>;
    pair _speed;
    explicit Movable(const pair speed) : _speed(speed)
    {
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
