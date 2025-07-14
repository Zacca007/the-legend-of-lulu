#include "arena.hpp"
#include "actor.hpp"
#include "movable.hpp"
#include <algorithm>
#include <ranges>
#include <utility>

using namespace lulu;

/**
 * @brief Add an actor to the arena
 * Handles actor registration and collision map initialization
 */
void Arena::spawn(Actor *actor)
{
    if (!actor)
        return;

    // Set this arena as the actor's arena
    actor->setArena(this);

    // Prevent duplicate additions
    if (std::ranges::find(_actors, actor) != _actors.end())
        return;

    // Add to actor list and initialize collision map
    _actors.emplace_back(actor);
    _collisions[actor] = {};
}

/**
 * @brief Remove an actor from the arena
 * Handles cleanup of actor references and collision data
 */
void Arena::kill(Actor *actor)
{
    if (!actor)
        return;

    // Clean up collision data and remove from actor list
    _collisions.erase(actor);
    std::erase(_actors, actor);
    actor->setArena(nullptr);
}

/**
 * @brief Update arena state for one game tick
 * Processes input, updates movable actors, and handles collisions
 */
void Arena::tick(const std::vector<Key> &keys)
{
    // Store previous keys and update current keys
    _prevKeys = std::exchange(_currKeys, keys);

    // Process all actors - only movable actors get updated
    for (auto *actor : _actors)
    {
        if (auto *movable = dynamic_cast<Movable *>(actor))
        {
            movable->move();                               // Update position and animation
            detectCollisionsFor(actor);                    // Check collisions for this actor
            movable->handleCollisions(_collisions[actor]); // Handle collision responses
        }
    }
}

/**
 * @brief Detect collisions for a specific actor
 * More efficient than checking all actors - only called for movable actors
 */
void Arena::detectCollisionsFor(Actor *actor)
{
    auto &collisions = _collisions[actor];
    collisions.clear();

    // Check collision with all other actors
    for (auto *other : _actors)
    {
        if (actor == other)
            continue;

        // Check collision and add to collision list if found
        if (const auto coll = actor->checkCollision(other); coll != C_NONE)
        {
            collisions.emplace_back(other, coll);
        }
    }
}

/**
 * @brief Detect collisions for all actors
 * Currently unused - detectCollisionsFor is more efficient for movable actors only
 */
void Arena::detectCollisions()
{
    for (const auto &actor : _collisions | std::views::keys)
    {
        detectCollisionsFor(actor);
    }
}