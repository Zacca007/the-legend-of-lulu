#include "actor.hpp"
#include "arena.hpp"
#include <algorithm>
#include <utility>

namespace lulu
{
Actor::Actor(const Vec2<int> pos, const Vec2<int> size, const std::string &sprite, Arena *room)
    : pos_(pos), size_(size), sprite_(sprite), arena_(room)
{
}

const Vec2<int> &Actor::pos() const
{
    return pos_;
}

const Vec2<int> &Actor::size() const
{
    return size_;
}

const std::string &Actor::sprite() const
{
    return sprite_;
}

Arena *Actor::arena() const
{
    return arena_;
}

//TODO: SOLVE THIS SHIT, THIS ACTOR NEEDS TO BE A UNIQUE PTR OF HIMSELF
void Actor::setArena(Arena* arena)
{
    if (arena_ == arena) return;

    // Lascia la vecchia arena (senza ownership)
    arena_ = arena;
}


void Actor::keepInsideArena()
{
    if (!arena_)
        return;

    const auto roomPos = arena_->pos();
    const auto roomSize = arena_->size();
    const auto [x, y] = roomPos + roomSize;

    pos_.x = std::clamp(pos_.x, roomPos.x, x - size_.x);
    pos_.y = std::clamp(pos_.y, roomPos.y, y - size_.y);
}
} // namespace lulu