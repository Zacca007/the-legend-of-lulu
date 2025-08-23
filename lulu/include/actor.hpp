#pragma once
#include "types.hpp"
#include <string>

namespace lulu
{
class Arena;
class Actor
{
  protected:
    Vec2<int> pos_, size_;
    std::string sprite_;
    Arena *arena_;

    void keepInsideArena();

  public:
    Actor(Vec2<int> pos, Vec2<int> size, const std::string &sprite = "", Arena *room = nullptr);
    virtual ~Actor() = default;

    [[nodiscard]] const Vec2<int> &pos() const;
    [[nodiscard]] const Vec2<int> &size() const;
    [[nodiscard]] const std::string &sprite() const;
    [[nodiscard]] Arena *arena() const;

    void setArena(Arena *arena);
};
} // namespace lulu
