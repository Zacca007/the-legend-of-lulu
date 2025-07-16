#include "arena.hpp"
#include "actor.hpp"
#include "movable.hpp"
#include <ranges>
#include <utility>

namespace lulu
{
// constructors
Arena::Arena(pair position, pair size) : _pos(position), _size(size)
{
}

// getters
[[nodiscard]] const pair &Arena::pos() const
{
    return _pos;
}
[[nodiscard]] const pair &Arena::size() const
{
    return _size;
}
[[nodiscard]] const std::vector<Key> &Arena::currKeys() const
{
    return _currKeys;
}
[[nodiscard]] const std::vector<Key> &Arena::prevKeys() const
{
    return _prevKeys;
}
[[nodiscard]] const std::vector<Actor *> &Arena::actors() const
{
    return _actors;
}
[[nodiscard]] const std::map<Actor *, std::vector<collision>> &Arena::collisions() const
{
    return _collisions;
}

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

void Arena::kill(Actor *actor)
{
    if (!actor)
        return;

    // Clean up collision data and remove from actor list
    _collisions.erase(actor);
    std::erase(_actors, actor);
    actor->setArena(nullptr);
}

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

void Arena::detectCollisions()
{
    for (const auto &actor : _collisions | std::views::keys)
    {
        detectCollisionsFor(actor);
    }
}

bool Arena::hasKey(const std::vector<Key> &keys, Key key)
{
    return std::ranges::find(keys, key) != keys.end();
}

bool Arena::isKeyJustPressed(Key key) const
{

    const auto &currKeys = _currKeys;
    const auto &prevKeys = _prevKeys;

    return hasKey(currKeys, key) && !hasKey(prevKeys, key);
}
} // namespace lulu
