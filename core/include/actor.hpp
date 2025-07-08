#pragma once
#include "types.hpp"
#include <string>
namespace lulu
{
class Arena;

class Actor
{
  protected:
    pair _pos;
    pair _size;
    Arena *_arena;
    std::string _sprite;

  public:
    Actor(pair position, pair size, Arena *arena = nullptr, const std::string &sprite = "");

    virtual ~Actor() = default;

    [[nodiscard]] const pair &pos() const
    {
        return _pos;
    }

    [[nodiscard]] const pair &size() const
    {
        return _size;
    }

    [[nodiscard]] const Arena *arena() const
    {
        return _arena;
    }

    [[nodiscard]] const std::string &sprite() const
    {
        return _sprite;
    }

    void setArena(Arena *arena);

    collisionType checkCollision(const Actor *other) const;
};
} // namespace lulu
