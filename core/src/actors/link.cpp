#include "actors/link.hpp"
#include "arena.hpp"
#include <algorithm>

using namespace lulu;

void Link::move()
{
    // Parsing input
    bool w = false, a = false, s = false, d = false;
    for (const Key key : _arena->currKeys())
    {
        switch (key)
        {
        case K_W:
            w = true;
            break;
        case K_A:
            a = true;
            break;
        case K_S:
            s = true;
            break;
        case K_D:
            d = true;
            break;
        }
    }

    // Risolvi input conflittuali
    if (a && d)
    {
        a = false;
        d = false;
    }
    if (w && s)
    {
        w = false;
        s = false;
    }

    // Calcola movimento e aggiorna sprite
    const pair diag = _speed.diagonal().value();

    if (w && a)
    {
        _pos -= diag;
        updateSprite(_up);
    }
    else if (w && d)
    {
        _pos += {diag.x, -diag.y};
        updateSprite(_up);
    }
    else if (s && a)
    {
        _pos += {-diag.x, diag.y};
        updateSprite(_down);
    }
    else if (s && d)
    {
        _pos += diag;
        updateSprite(_down);
    }
    else if (w)
    {
        _pos.y -= _speed.y;
        updateSprite(_up);
    }
    else if (s)
    {
        _pos.y += _speed.y;
        updateSprite(_down);
    }
    else if (a)
    {
        _pos.x -= _speed.x;
        updateSprite(_left);
    }
    else if (d)
    {
        _pos.x += _speed.x;
        updateSprite(_right);
    }
}

void Link::updateSprite(const animations &anim)
{
    _sprite = _animationCounter ? anim.second : anim.first;
    _animationCounter = !_animationCounter;
}

void Link::handleCollisions(const std::vector<collision> &collisions)
{
    for (const auto &coll : collisions)
    {
        const auto &target = coll.target;
        const auto &tPos = target->pos();
        const auto &tSize = target->size();

        switch (coll.type)
        {
        case C_TOP:
            _pos.y = tPos.y + tSize.y;
            break;
        case C_BOTTOM:
            _pos.y = tPos.y - _size.y;
            break;
        case C_LEFT:
            _pos.x = tPos.x + tSize.x;
            break;
        case C_RIGHT:
            _pos.x = tPos.x - _size.x;
            break;
        default: break;
        }
    }

    // Mantieni Link dentro l'arena
    const auto roomPos = _arena->pos();
    const auto roomSize = _arena->size();
    const auto roomEnd = roomPos + roomSize;

    _pos.x = std::clamp(_pos.x, roomPos.x, roomEnd.x - _size.x);
    _pos.y = std::clamp(_pos.y, roomPos.y, roomEnd.y - _size.y);
}