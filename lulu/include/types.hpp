#pragma once

namespace lulu
{
template <typename T>
struct Vec2
{
    T x, y;
    Vec2 operator+(const Vec2 & other) const
    {
        return { x + other.x, y + other.y };
    };
};

enum Key
{
    K_SPACE = 32,
    K_A = 65,
    K_D = 68,
    K_S = 83,
    K_W = 87,
    K_ENTER = 257,
    K_RIGHT = 262,
    K_LEFT = 263,
    K_DOWN = 264,
    K_UP = 265
};
}