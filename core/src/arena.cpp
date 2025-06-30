#include "arena.hpp"
#include "actor.hpp"
#include "movable.hpp"
#include <algorithm>
#include <set>
using lulu::Arena;

void Arena::spawn(Actor *actor)
{
    if (!actor)
        return;

    actor->setArena(this);

    // Evita di aggiungere lo stesso attore due volte
    for (const Actor *currentActor : _actors)
    {
        if (currentActor == actor)
            return;
    }
    _actors.push_back(actor);
    _collisions.insert({actor, std::vector<Actor *>()});
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
    detectCollisions();

    // Aggiorna i tasti
    _prevKeys = _currKeys;
    _currKeys = keys;

    // Muove tutti gli attori
    for (Actor *actor : _actors)
    {
        Movable *movable = dynamic_cast<Movable *>(actor);
        if (movable)
        {
            movable->move();
        }
    }

    _count++;
}

void Arena::detectCollisions()
{
    _collisions.clear();

    for (Actor *act : _actors)
    {
        std::vector<Actor *> collisions;
        for (Actor *other : _actors)
        {
            if (act == other)
                continue;
            else if (act->checkCollision(other) != C_NONE)
                collisions.push_back(other);
        }
        _collisions.insert({act, collisions});
    }
}