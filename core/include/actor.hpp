#pragma once
#include "types.hpp"
#include <string>
namespace lulu
{
class Arena;

/**
 * @brief Base class for all game entities
 *
 * Actor represents any object in the game world that has a position,
 * size, and can be rendered with a sprite. It provides basic collision
 * detection and arena management.
 */
class Actor
{
  protected:
    pair _pos;           // Position in world coordinates
    pair _size;          // Size (width, height) of the actor
    Arena *_arena;       // Pointer to the arena this actor belongs to
    std::string _sprite; // Path to the sprite image file

  public:
    /**
     * @brief Construct a new Actor
     * @param position Initial position in world coordinates
     * @param size Size of the actor (width, height)
     * @param arena Pointer to the arena (optional)
     * @param sprite Path to sprite file (optional)
     */
    Actor(pair position, pair size, Arena *arena = nullptr, std::string sprite = "");

    virtual ~Actor() = default;

    // Getters for actor properties
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

    /**
     * @brief Set the arena this actor belongs to
     * @param arena Pointer to the arena
     */
    void setArena(Arena *arena);

    /**
     * @brief Check collision with another actor
     * @param other The other actor to check collision with
     * @return Type of collision (C_NONE if no collision)
     */
    collisionType checkCollision(const Actor *other) const;
};
} // namespace lulu