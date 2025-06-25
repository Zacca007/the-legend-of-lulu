#pragma once
#include <cmath>

namespace lulu
{
    struct pair
    {
        float x=0, y=0;

        pair operator+(const pair &other) const { return {x + other.x, y + other.y}; }
        pair operator-(const pair &other) const { return {x - other.x, y - other.y}; }
        pair operator*(float scalar) const { return {x * scalar, y * scalar}; }

        float length() const { return std::sqrt(x * x + y * y); }
    };

    enum Key
    {
        RIGHT = 262,
        LEFT = 263,
        DOWN = 264,
        UP = 265
    };
}
