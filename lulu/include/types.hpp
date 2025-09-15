#pragma once
#include <optional>
#include <cmath>
#include <type_traits>

namespace lulu
{
class Actor;
template <typename T>
struct Vec2
{
    static_assert(std::is_arithmetic_v<T>, "Vec2<T> requires a numeric type");

    T x, y;

    // Operatori tra Vec2
    Vec2 operator+(const Vec2 &other) const { return {x + other.x, y + other.y}; }
    Vec2 operator-(const Vec2 &other) const { return {x - other.x, y - other.y}; }
    Vec2 operator*(const Vec2 &other) const { return {x * other.x, y * other.y}; }
    Vec2 operator/(const Vec2 &other) const { return {x / other.x, y / other.y}; }

    // Operatori con scalare
    Vec2 operator+(T other) const { return {x + other, y + other}; }
    Vec2 operator-(T other) const { return {x - other, y - other}; }
    Vec2 operator*(T other) const { return {x * other, y * other}; }
    Vec2 operator/(T other) const { return {x / other, y / other}; }

    // Operatori composti
    Vec2 &operator+=(const Vec2 &other) { *this = *this + other; return *this; }
    Vec2 &operator-=(const Vec2 &other) { *this = *this - other; return *this; }
    Vec2 &operator*=(const Vec2 &other) { *this = *this * other; return *this; }
    Vec2 &operator/=(const Vec2 &other) { *this = *this / other; return *this; }

    Vec2 &operator+=(T other) { *this = *this + other; return *this; }
    Vec2 &operator-=(T other) { *this = *this - other; return *this; }
    Vec2 &operator*=(T other) { *this = *this * other; return *this; }
    Vec2 &operator/=(T other) { *this = *this / other; return *this; }

    //conversions
    template <typename C> Vec2<C> convert() const { return Vec2<C>{static_cast<C>(x), static_cast<C>(y)}; }

    // Calcola la diagonale (se quadrato)
    [[nodiscard]] std::optional<Vec2> diagonal() const
    {
        if (x == y)
        {
            auto l = static_cast<T>(x / std::sqrt(2.0));
            return Vec2{l, l};
        }
        return std::nullopt;
    }
};

// Enum dei tasti
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

enum State
{
    S_STILL,
    S_MOVING,
    S_ATTACK,
    S_HURT
};

enum Direction
{
    D_NONE,    // No movement
    D_UP,       // Up
    D_DOWN,     // Down
    D_LEFT,     // Left
    D_RIGHT,    // Right
    D_UPLEFT,   // Diagonal up-left
    D_UPRIGHT,  // Diagonal up-right
    D_DOWNLEFT, // Diagonal down-left
    D_DOWNRIGHT // Diagonal down-right
};

struct Collision
{
    Actor *target;      // The actor we collided with
    Direction collisionDirection; // Type of collision
};

} // namespace lulu
