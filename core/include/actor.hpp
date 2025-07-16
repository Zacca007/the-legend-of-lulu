#pragma once
#include "types.hpp"
#include <string>
namespace lulu
{
class Arena;

class Actor
{
  protected:
    pair pos_;           // Position in world coordinates
    pair size_;          // Size (width, height) of the actor
    Arena *arena_;       // Pointer to the arena this actor belongs to
    std::string sprite_; // Path to the sprite image file

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