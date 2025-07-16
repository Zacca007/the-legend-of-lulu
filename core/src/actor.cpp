#include "actor.hpp"

#include <algorithm>
#include <utility>

#include "arena.hpp"
namespace lulu
{
Actor::Actor(const pair position, const pair size, Arena *arena, std::string sprite)
    : pos_(position), size_(size), arena_(nullptr), sprite_(std::move(sprite))
{
    setArena(arena);
}
// Getters for actor properties
const pair &Actor::pos() const
{
    return pos_;
}
const pair &Actor::size() const
{
    return size_;
}
const Arena *Actor::arena() const
{
    return arena_;
}
const std::string &Actor::sprite() const
{
    return sprite_;
}

void Actor::setArena(Arena *arena)
{
    // Early return if arena is the same
    if (arena_ == arena)
        return;

    // Clean up from old arena
    if (arena_)
    {
        auto *old = std::exchange(arena_, nullptr);
        old->kill(this);
    }

    // Set new arena
    arena_ = arena;

    // Register with new arena
    if (arena_)
    {
        arena_->spawn(this);
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
    const pair thisMax = pos_ + size_;
    const pair otherMax = {ox + ow, oy + oh};

    // AABB collision detection - check if boxes don't overlap
    if (thisMax.x < ox || otherMax.x < pos_.x || thisMax.y < oy || otherMax.y < pos_.y)
    {
        return C_NONE;
    }

    // Calculate penetration depths for each side
    const float leftDist = std::abs(thisMax.x - ox);        // Distance from our right to their left
    const float rightDist = std::abs(otherMax.x - pos_.x);  // Distance from our left to their right
    const float topDist = std::abs(thisMax.y - oy);         // Distance from our bottom to their top
    const float bottomDist = std::abs(otherMax.y - pos_.y); // Distance from our top to their bottom

    // Find minimum penetration distances
    const float minH = std::min(leftDist, rightDist); // Minimum horizontal penetration
    const float minV = std::min(topDist, bottomDist); // Minimum vertical penetration

    // Calculate centers to determine collision direction
    const pair thisCenter = pos_ + size_ / 2.0f;
    const pair otherCenter = other->pos() + other->size() / 2.0f;

    // Return collision type based on minimum penetration
    if (minH <= minV)
        return thisCenter.x <= otherCenter.x ? C_RIGHT : C_LEFT;

    return thisCenter.y <= otherCenter.y ? C_BOTTOM : C_TOP;
}

void Actor::clampToArena()
{
    if (!arena_)
        return;

    const auto roomPos = arena_->pos();
    const auto roomSize = arena_->size();
    const auto roomEnd = roomPos + roomSize;

    pos_.x = std::clamp(pos_.x, roomPos.x, roomEnd.x - size_.x);
    pos_.y = std::clamp(pos_.y, roomPos.y, roomEnd.y - size_.y);
}

} // namespace lulu
