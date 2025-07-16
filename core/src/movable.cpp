#include "movable.hpp"

namespace lulu
{
Movable::Movable(const pair speed, bool enableAnimation) : _speed(speed)
{
    if (enableAnimation)
        _animation._enabled = true;
}

const pair &Movable::speed() const
{
    return _speed;
}
} // namespace lulu
