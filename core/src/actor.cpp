#include "arena.hpp"
#include "actor.hpp"
using lulu::Arena, lulu::Actor;

Actor::Actor(pair position, pair size, Arena *arena, const std::string &sprite)
    : _pos(position), _size(size), _arena(arena), _sprite(sprite)
{
    if (_arena != nullptr)
        _arena->spawn(this);
}

void Actor::setArena(Arena *arena)
{
    if (_arena == arena)
        return;

    if (_arena != nullptr)
    {
        Arena *oldArena = _arena;
        _arena = nullptr;
        oldArena->kill(this);
    }

    _arena = arena;

    if (arena != nullptr)
        _arena->spawn(this);
}