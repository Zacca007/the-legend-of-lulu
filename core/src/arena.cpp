#include "arena.hpp"
#include "actor.hpp"
using lulu::Arena, lulu::Actor;

void Arena::spawn(Actor *actor)
{
    actor->setArena(this);
    
    //cycle is preventing the recursion to add the actor two times
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