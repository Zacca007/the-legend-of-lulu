#pragma once
#include <cmath>

namespace lulu
{
    struct pair
    {
        float x = 0, y = 0;

        pair operator+(const pair &other) const { return {x + other.x, y + other.y}; }
        pair operator+(float offset) const { return {x + offset, y + offset}; }
        pair operator-(const pair &other) const { return {x - other.x, y - other.y}; }
        pair operator*(float scalar) const { return {x * scalar, y * scalar}; }

        float length() const { return std::sqrt(x * x + y * y); }
    };

    enum Key
    {
        K_RIGHT = 262,
        K_LEFT = 263,
        K_DOWN = 264,
        K_UP = 265
    };

    enum collision
    {
        C_RIGHT,
        C_LEFT,
        C_TOP,
        C_BOTTOM
    };
}
