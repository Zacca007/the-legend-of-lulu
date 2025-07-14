#pragma once
#include "types.hpp"
#include <map>
#include <vector>

namespace lulu
{
class Actor;
class Movable;

/**
 * @brief Game world container that manages actors and game logic
 *
 * Arena handles actor lifecycle, collision detection, input processing,
 * and game state updates. It serves as the main game world container.
 */
class Arena
{
    pair _pos;                                             // Arena position
    pair _size;                                            // Arena size
    std::vector<Key> _currKeys;                            // Current frame input
    std::vector<Key> _prevKeys;                            // Previous frame input
    std::vector<Actor *> _actors;                          // All actors in the arena
    std::map<Actor *, std::vector<collision>> _collisions; // Collision data per actor

    /**
     * @brief Detect collisions for a specific actor
     * @param actor The actor to check collisions for
     */
    void detectCollisionsFor(Actor *actor);

    /**
     * @brief Detect collisions for all actors
     */
    void detectCollisions();

  public:
    /**
     * @brief Construct a new Arena
     * @param position Arena position in world coordinates
     * @param size Arena size (width, height)
     */
    Arena(const pair position, const pair size) : _pos(position), _size(size)
    {
    }

    // Getters for arena properties
    [[nodiscard]] const pair &pos() const
    {
        return _pos;
    }
    [[nodiscard]] const pair &size() const
    {
        return _size;
    }
    [[nodiscard]] const std::vector<Key> &currKeys() const
    {
        return _currKeys;
    }
    [[nodiscard]] const std::vector<Key> &prevKeys() const
    {
        return _prevKeys;
    }
    [[nodiscard]] const std::vector<Actor *> &actors() const
    {
        return _actors;
    }
    [[nodiscard]] const std::map<Actor *, std::vector<collision>> &collisions() const
    {
        return _collisions;
    }

    /**
     * @brief Add an actor to the arena
     * @param actor Pointer to the actor to add
     */
    void spawn(Actor *actor);

    /**
     * @brief Remove an actor from the arena
     * @param actor Pointer to the actor to remove
     */
    void kill(Actor *actor);

    /**
     * @brief Update arena state for one game tick
     * @param keys Input keys for this frame
     */
    void tick(const std::vector<Key> &keys = {});
};
} // namespace lulu