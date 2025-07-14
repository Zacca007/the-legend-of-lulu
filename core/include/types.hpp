#pragma once
#include <optional>

namespace lulu
{
class Actor;

/**
 * @brief 2D coordinate/vector structure with arithmetic operations
 */
struct pair
{
    float x = 0, y = 0;

    // Arithmetic operations with another pair
    pair operator+(const pair &other) const;
    pair operator-(const pair &other) const;
    pair operator*(const pair &other) const;
    pair operator/(const pair &other) const;

    // Arithmetic operations with scalar values
    pair operator+(float offset) const;
    pair operator-(float offset) const;
    pair operator*(float offset) const;
    pair operator/(float offset) const;

    // Compound assignment operations
    pair &operator+=(const pair &other);
    pair &operator-=(const pair &other);

    /**
     * @brief Calculate diagonal components for movement (when x == y)
     * @return Optional pair with diagonal values, nullopt if x != y
     */
    [[nodiscard]] std::optional<pair> diagonal() const;
};

/**
 * @brief Keyboard input enumeration
 */
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

/**
 * @brief Movement direction enumeration
 */
enum direction
{
    D_STILL,    // No movement
    D_UP,       // Up
    D_DOWN,     // Down
    D_LEFT,     // Left
    D_RIGHT,    // Right
    D_UPLEFT,   // Diagonal up-left
    D_UPRIGHT,  // Diagonal up-right
    D_DOWNLEFT, // Diagonal down-left
    D_DOWNRIGHT // Diagonal down-right
};

/**
 * @brief Actor state enumeration for animations
 */
enum state
{
    S_STILL,    // Standing still
    S_MOVEMENT, // Moving
    S_ATTACK,   // Attacking
    D_HURT      // Taking damage
};

/**
 * @brief Collision type enumeration indicating collision side
 */
enum collisionType
{
    C_RIGHT,  // Collision on the right side
    C_LEFT,   // Collision on the left side
    C_TOP,    // Collision on the top side
    C_BOTTOM, // Collision on the bottom side
    C_NONE    // No collision
};

/**
 * @brief Collision data structure
 */
struct collision
{
    Actor *target;      // The actor we collided with
    collisionType type; // Type of collision
};
} // namespace lulu