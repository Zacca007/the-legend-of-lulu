#include "arena.hpp"
#include "actor.hpp"
using lulu::Arena, lulu::Actor;

void Arena::spawn(Actor *actor)
{
    actor->setArena(this);
    _actors.push_back(actor);
}

void Arena::kill(Actor *actor)
{
    std::erase(_actors, actor);
    actor->setArena(nullptr);
}