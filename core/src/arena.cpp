#include "arena.hpp"
#include "actor.hpp"
#include "movable.hpp"
#include <algorithm>
#include <utility>

using namespace lulu;

void Arena::spawn(Actor *actor)
{
    if (!actor)
        return;

    actor->setArena(this);

    if (std::ranges::find(_actors, actor) != _actors.end())
        return;

    _actors.emplace_back(actor);
    _collisions[actor] = {};
}

void Arena::kill(Actor *actor)
{
    if (!actor)
        return;

    _collisions.erase(actor);
    std::erase(_actors, actor);
    actor->setArena(nullptr);
}

void Arena::tick(const std::vector<Key> &keys)
{
    _prevKeys = std::exchange(_currKeys, keys);

    for (auto *actor : _actors)
    {
        if (auto *movable = dynamic_cast<Movable *>(actor))
        {
            movable->move();
            detectCollisions();
            movable->handleCollisions(_collisions[actor]);
        }
    }
}

void Arena::detectCollisions()
{
    for (auto &[actor, collisions] : _collisions)
    {
        collisions.clear();

        for (auto *other : _actors)
        {
            if (actor == other)
                continue;

            const auto coll = actor->checkCollision(other);
            if (coll != C_NONE)
            {
                collisions.emplace_back(other, coll);
            }
        }
    }
}
