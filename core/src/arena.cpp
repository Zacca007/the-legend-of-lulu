#include "arena.hpp"
#include "actor.hpp"
#include "movable.hpp"
#include <ranges>
#include <utility>

namespace lulu
{
// constructors
Arena::Arena(pair position, pair size) : pos_(position), size_(size)
{
}

// getters
[[nodiscard]] const pair &Arena::pos() const
{
    return pos_;
}
[[nodiscard]] const pair &Arena::size() const
{
    return size_;
}
[[nodiscard]] const std::vector<Key> &Arena::currKeys() const
{
    return currKeys_;
}
[[nodiscard]] const std::vector<Key> &Arena::prevKeys() const
{
    return prevKeys_;
}
[[nodiscard]] const std::vector<Actor *> &Arena::actors() const
{
    return actors_;
}
[[nodiscard]] const std::map<Actor *, std::vector<collision>> &Arena::collisions() const
{
    return collisions_;
}

void Arena::spawn(Actor *actor)
{
    if (!actor)
        return;

    // Set this arena as the actor's arena
    actor->setArena(this);

    // Prevent duplicate additions
    if (std::ranges::find(actors_, actor) != actors_.end())
        return;

    // Add to actor list and initialize collision map
    actors_.emplace_back(actor);
    collisions_[actor] = {};
}

void Arena::kill(Actor *actor)
{
    if (!actor)
        return;

    // Clean up collision data and remove from actor list
    collisions_.erase(actor);
    std::erase(actors_, actor);
    actor->setArena(nullptr);
}

void Arena::tick(const std::vector<Key> &keys)
{
    // Store previous keys and update current keys
    prevKeys_ = std::exchange(currKeys_, keys);

    // Process all actors - only movable actors get updated
    for (auto *actor : actors_)
    {
        if (auto *movable = dynamic_cast<Movable *>(actor))
        {
            movable->move();                               // Update position and animation
            detectCollisionsFor(actor);                    // Check collisions for this actor
            movable->handleCollisions(collisions_[actor]); // Handle collision responses
        }
    }
}

void Arena::detectCollisionsFor(Actor *actor)
{
    auto &collisions = collisions_[actor];
    collisions.clear();

    // Check collision with all other actors
    for (auto *other : actors_)
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
    for (const auto &actor : collisions_ | std::views::keys)
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

    const auto &currKeys = currKeys_;
    const auto &prevKeys = prevKeys_;

    return hasKey(currKeys, key) && !hasKey(prevKeys, key);
}
} // namespace lulu
