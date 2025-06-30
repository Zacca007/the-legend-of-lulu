#include "arena.hpp"
#include "actor.hpp"
using lulu::Actor;

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

lulu::collision Actor::checkCollision(const Actor *other) const
{
    if (other == nullptr)
        return C_NONE;
    if (other->arena() == nullptr)
        return C_NONE;

    const pair &otherPos = other->pos();
    const pair &otherSize = other->size();

    // Calcola i bordi di entrambi i rettangoli
    pair thisMin = _pos;
    pair thisMax = _pos + _size;
    pair otherMin = otherPos;
    pair otherMax = otherPos + otherSize;

    // Verifica se c'è una collisione
    if (thisMax.x <= otherMin.x || otherMax.x <= thisMin.x ||
        thisMax.y <= otherMin.y || otherMax.y <= thisMin.y)
    {
        return C_NONE;
    }

    // Calcola l'overlap su ogni asse
    float overlapX = std::min(thisMax.x - otherMin.x, otherMax.x - thisMin.x);
    float overlapY = std::min(thisMax.y - otherMin.y, otherMax.y - thisMin.y);

    // Il lato della collisione è determinato dall'overlap minore
    if (overlapX < overlapY)
    {
        // Collisione orizzontale
        pair thisCenter = _pos + _size / 2.0f;
        pair otherCenter = otherPos + otherSize / 2.0f;

        if (thisCenter.x < otherCenter.x)
            return C_RIGHT; // Questo oggetto collide dal lato destro
        else
            return C_LEFT; // Questo oggetto collide dal lato sinistro
    }
    else
    {
        // Collisione verticale
        pair thisCenter = _pos + _size / 2.0f;
        pair otherCenter = otherPos + otherSize / 2.0f;

        if (thisCenter.y < otherCenter.y)
            return C_BOTTOM; // Questo oggetto collide dal lato inferiore
        else
            return C_TOP; // Questo oggetto collide dal lato superiore
    }
}