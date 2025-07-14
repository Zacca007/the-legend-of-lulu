#include "actor.hpp"
#include "arena.hpp"
#include <utility>

using namespace lulu;

/**
 * @brief Construct a new Actor
 * Initializes position, size, and optionally sets arena and sprite
 */
Actor::Actor(const pair position, const pair size, Arena *arena, std::string sprite)
    : _pos(position), _size(size), _arena(nullptr), _sprite(std::move(sprite))
{
    setArena(arena);
}

/**
 * @brief Set the arena this actor belongs to
 * Handles proper cleanup from old arena and registration with new one
 */
void Actor::setArena(Arena *arena)
{
    // Early return if arena is the same
    if (_arena == arena)
        return;

    // Clean up from old arena
    if (_arena)
    {
        auto *old = std::exchange(_arena, nullptr);
        old->kill(this);
    }

    // Set new arena
    _arena = arena;

    // Register with new arena
    if (_arena)
    {
        _arena->spawn(this);
    }
}

/**
 * @brief Check collision with another actor using AABB collision detection
 * @param other The other actor to check collision with
 * @return Type of collision based on penetration depth
 */
collisionType Actor::checkCollision(const Actor *other) const
{
    // Null check and arena validation
    if (!other || !other->arena())
        return C_NONE;

    // Get other actor's bounds
    const auto &[ox, oy] = other->pos();
    const auto &[ow, oh] = other->size();
 // Calculate bounding box corners
    const pair thisMax = _pos + _size;
    const pair otherMax = {ox + ow, oy + oh};

    // AABB collision detection - check if boxes don't overlap
    if (thisMax.x < ox || otherMax.x < _pos.x || thisMax.y < oy || otherMax.y < _pos.y)
    {
        return C_NONE;
    }

    // Calculate penetration depths for each side
    const float leftDist = std::abs(thisMax.x - ox);        // Distance from our right to their left
    const float rightDist = std::abs(otherMax.x - _pos.x);  // Distance from our left to their right
    const float topDist = std::abs(thisMax.y - oy);         // Distance from our bottom to their top
    const float bottomDist = std::abs(otherMax.y - _pos.y); // Distance from our top to their bottom

    // Find minimum penetration distances
    const float minH = std::min(leftDist, rightDist); // Minimum horizontal penetration
    const float minV = std::min(topDist, bottomDist); // Minimum vertical penetration

    // Calculate centers to determine collision direction
    const pair thisCenter = _pos + _size / 2.0f;
    const pair otherCenter = other->pos() + other->size() / 2.0f;

    // Return collision type based on minimum penetration
    if (minH <= minV)
        return thisCenter.x <= otherCenter.x ? C_RIGHT : C_LEFT;

    return thisCenter.y <= otherCenter.y ? C_BOTTOM : C_TOP;
}