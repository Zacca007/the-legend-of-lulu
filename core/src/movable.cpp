#include "movable.hpp"

namespace lulu
{
Movable::Movable(const pair speed, bool enableAnimation) : speed_(speed)
{
    if (enableAnimation)
        movement_.enabled_ = true;
}

const pair &Movable::speed() const
{
    return speed_;
}
} // namespace lulu
