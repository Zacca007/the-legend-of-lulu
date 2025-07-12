#pragma once
#include <optional>
#include <string>
#include <vector>

namespace lulu
{
class Actor;

struct pair
{
    float x = 0, y = 0;

    pair operator+(const pair &other) const;
    pair operator-(const pair &other) const;
    pair operator*(const pair &other) const;
    pair operator/(const pair &other) const;

    pair operator+(float offset) const;
    pair operator-(float offset) const;
    pair operator*(float offset) const;
    pair operator/(float offset) const;

    pair &operator+=(const pair &other);
    pair &operator-=(const pair &other);

    [[nodiscard]] std::optional<pair> diagonal() const;
};

enum Key
{
    K_SPACE = 32,
    K_A = 65,
    K_D = 68,
    K_S = 83,
    K_W = 87,
    K_RIGHT = 262,
    K_LEFT = 263,
    K_DOWN = 264,
    K_UP = 265
};

enum direction
{
    D_STILL,
    D_UP,
    D_DOWN,
    D_LEFT,
    D_RIGHT,
    D_UPLEFT,
    D_UPRIGHT,
    D_DOWNLEFT,
    D_DOWNRIGHT
};

enum state
{
    S_STILL,
    S_MOVEMENT,
    S_ATTACK,
    D_HURT
};

enum collisionType
{
    C_RIGHT,
    C_LEFT,
    C_TOP,
    C_BOTTOM,
    C_NONE
};

struct collision
{
    Actor *target;
    collisionType type;
};
} // namespace lulu
