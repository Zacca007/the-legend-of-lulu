#include "arena.hpp"
#include "actor.hpp"
#include "movable.hpp"
using lulu::Arena;

void Arena::spawn(Actor *actor)
{
    actor->setArena(this);

    // cycle is preventing the recursion to add the actor two times
    for (Actor *currentActor : _actors)
    {
        if (currentActor == actor)
            return;
    }
    _actors.push_back(actor);
}

void Arena::kill(Actor *actor)
{
    std::erase(_actors, actor);
    actor->setArena(nullptr);
}

void Arena::tick(const std::vector<Key> &keys)
{
    _prevKeys = _currKeys;
    _currKeys = keys;

    for (Actor *actor : _actors)
    {
        Movable *movable = dynamic_cast<Movable *>(actor);
        if (movable)
        {
            movable->move();
        }
    }
}