#include "arena.hpp"
#include "actor.hpp"
#include "movable.hpp"
#include <algorithm>
#include <ranges>
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
    // detectCollisions(); disabilitato: calcoliamo collisioni solo per gli attori mobili (più efficiente)
    _prevKeys = std::exchange(_currKeys, keys);

    for (auto *actor : _actors)
    {
        if (auto *movable = dynamic_cast<Movable *>(actor))
        {
            movable->move();
            detectCollisionsFor(actor);
            movable->handleCollisions(_collisions[actor]);
        }
    }
}

void Arena::detectCollisionsFor(Actor *actor)
{
    auto &collisions = _collisions[actor];
    collisions.clear();
    for (auto *other : _actors)
    {
        if (actor == other)
            continue;

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