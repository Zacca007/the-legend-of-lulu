#include "movable.hpp"

using namespace lulu;

/**
 * @brief Calculate movement vector for given direction and speed
 */
pair Movable::calculateMovement(const direction dir, const float speedMultiplier) const
{
    const pair effectiveSpeed = _speed * speedMultiplier;

    switch (dir)
    {
    case D_UP:
        return {0, -effectiveSpeed.y};
    case D_DOWN:
        return {0, effectiveSpeed.y};
    case D_LEFT:
        return {-effectiveSpeed.x, 0};
    case D_RIGHT:
        return {effectiveSpeed.x, 0};
    case D_UPLEFT:
    case D_UPRIGHT:
    case D_DOWNLEFT:
    case D_DOWNRIGHT: {
        // Calculate diagonal movement
        if (auto diagonal = effectiveSpeed.diagonal(); diagonal.has_value())
        {
            const auto [x, y] = diagonal.value();
            switch (dir)
            {
            case D_UPLEFT:
                return {-x, -y};
            case D_UPRIGHT:
                return {x, -y};
            case D_DOWNLEFT:
                return {-x, y};
            case D_DOWNRIGHT:
                return {x, y};
            default:
                break;
            }
        }
    }
    break;
    case D_STILL:
    default:
        break;
    }
    return {0, 0};
}