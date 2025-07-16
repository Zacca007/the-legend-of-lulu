#pragma once
#include "types.hpp"
#include <string>
namespace lulu
{
class Arena;

class Actor
{
  protected:
    pair _pos;           // Position in world coordinates
    pair _size;          // Size (width, height) of the actor
    Arena *_arena;       // Pointer to the arena this actor belongs to
    std::string _sprite; // Path to the sprite image file

    void clampToArena();

  public:
    Actor(pair position, pair size, Arena *arena = nullptr, std::string sprite = "");

    virtual ~Actor() = default;

    // Getters for actor properties
    [[nodiscard]] const pair &pos() const;
    [[nodiscard]] const pair &size() const;
    [[nodiscard]] const Arena *arena() const;
    [[nodiscard]] const std::string &sprite() const;

    void setArena(Arena *arena);
    collisionType checkCollision(const Actor *other) const;
};
} // namespace lulu