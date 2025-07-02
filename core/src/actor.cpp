#include "actor.hpp"
#include "arena.hpp"

#include <utility>

using namespace lulu;

Actor::Actor(const pair position, const pair size, Arena *arena, const std::string &sprite)
    : _pos(position), _size(size), _arena(nullptr), _sprite(sprite)
{
    setArena(arena);
}

void Actor::setArena(Arena *arena)
{
    if (_arena == arena)
        return;

    if (_arena)
    {
        auto *old = std::exchange(_arena, nullptr);
        old->kill(this);
    }

    _arena = arena;

    if (_arena)
    {
        _arena->spawn(this);
    }
}

collisionType Actor::checkCollision(const Actor *other) const
{
    if (!other || !other->arena())
        return C_NONE;

    const auto &[ox, oy] = other->pos();
    const auto &[ow, oh] = other->size();
    const pair thisMax = _pos + _size;
    const pair otherMax = {ox + ow, oy + oh};

    if (thisMax.x < ox || otherMax.x < _pos.x || thisMax.y < oy || otherMax.y < _pos.y)

    {
        return C_NONE;
    }

    const float leftDist = std::abs(thisMax.x - ox);
    const float rightDist = std::abs(otherMax.x - _pos.x);
    const float topDist = std::abs(thisMax.y - oy);
    const float bottomDist = std::abs(otherMax.y - _pos.y);

    const float minH = std::min(leftDist, rightDist);
    const float minV = std::min(topDist, bottomDist);

    const pair thisCenter = _pos + _size / 2.0f;
    const pair otherCenter = other->pos() + other->size() / 2.0f;

    if (minH <= minV)
        return thisCenter.x <= otherCenter.x ? C_RIGHT : C_LEFT;

    return thisCenter.y <= otherCenter.y ? C_BOTTOM : C_TOP;
}
