#include "actor.hpp"

#include <algorithm>
#include <utility>

#include "arena.hpp"
namespace lulu
{
Actor::Actor(const pair position, const pair size, Arena *arena, std::string sprite)
    : _pos(position), _size(size), _arena(nullptr), _sprite(std::move(sprite))
{
    setArena(arena);
}
// Getters for actor properties
const pair &Actor::pos() const
{
    return _pos;
}
const pair &Actor::size() const
{
    return _size;
}
const Arena *Actor::arena() const
{
    return _arena;
}
const std::string &Actor::sprite() const
{
    return _sprite;
}

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

void Actor::clampToArena()
{
    if (!_arena)
        return;

    const auto roomPos = _arena->pos();
    const auto roomSize = _arena->size();
    const auto roomEnd = roomPos + roomSize;

    _pos.x = std::clamp(_pos.x, roomPos.x, roomEnd.x - _size.x);
    _pos.y = std::clamp(_pos.y, roomPos.y, roomEnd.y - _size.y);
}

} // namespace lulu
