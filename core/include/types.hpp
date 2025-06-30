#pragma once
#include <optional>

namespace lulu {
class Actor;
    struct pair {
        float x = 0, y = 0;

        pair operator+(const pair &other) const { return {x + other.x, y + other.y}; }
        pair operator-(const pair &other) const { return {x - other.x, y - other.y}; }
        pair operator*(const pair &other) const { return {x * other.x, y * other.y}; }
        pair operator/(const pair &other) const { return {x / other.x, y / other.y}; }

        pair operator+(const float offset) const { return {x + offset, y + offset}; }
        pair operator-(const float offset) const { return {x - offset, y - offset}; }
        pair operator*(const float offset) const { return {x * offset, y * offset}; }
        pair operator/(const float offset) const { return {x / offset, y / offset}; }

        pair &operator+=(const pair &other);

        pair &operator-=(const pair &other);

        [[nodiscard]] std::optional<pair> diagonal() const;
    };

    enum Key {
        K_A = 65,
        K_D = 68,
        K_S = 83,
        K_W = 87,
    };

    enum collisionType {
        C_RIGHT,
        C_LEFT,
        C_TOP,
        C_BOTTOM,
        C_NONE
    };

    struct collision
    {
        Actor* target;
        collisionType type;
    };
}
