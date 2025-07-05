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
        case K_UP:
            w = true;
            break;

        case K_A:
        case K_LEFT:
            a = true;
            break;

        case K_S:
        case K_DOWN:
            s = true;
            break;

        case K_D:
        case K_RIGHT:
            d = true;
            break;

        default:
            break;
        }
    }

    // Risolvi input conflittuali
    if (a && d)
        a = d = false;
    if (w && s)
        w = s = false;

    // Determina la direzione
    if (w && a)
        _direction = D_UPLEFT;
    else if (w && d)
        _direction = D_UPRIGHT;
    else if (s && a)
        _direction = D_DOWNLEFT;
    else if (s && d)
        _direction = D_DOWNRIGHT;
    else if (w)
        _direction = D_UP;
    else if (s)
        _direction = D_DOWN;
    else if (a)
        _direction = D_LEFT;
    else if (d)
        _direction = D_RIGHT;
    else
        _direction = D_STILL;

    pair diagonal = _speed.diagonal().value();
    switch (_direction)
    {
    case D_UPLEFT:
        _pos -= diagonal;
        updateSprite(_up);
        break;
    case D_UPRIGHT:
        _pos += {diagonal.x, -diagonal.y};
        updateSprite(_up);
        break;
    case D_DOWNLEFT:
        _pos += {-diagonal.x, diagonal.y};
        updateSprite(_down);
        break;
    case D_DOWNRIGHT:
        _pos += diagonal;
        updateSprite(_down);
        break;
    case D_UP:
        _pos.y -= _speed.y;
        updateSprite(_up);
        break;
    case D_DOWN:
        _pos.y += _speed.y;
        updateSprite(_down);
        break;
    case D_LEFT:
        _pos.x -= _speed.x;
        updateSprite(_left);
        break;
    case D_RIGHT:
        _pos.x += _speed.x;
        updateSprite(_right);
        break;
    case D_STILL:
        // Non fare nulla
        break;
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
        default:
            break;
        }
    }

    // Mantieni Link dentro l'arena
    const auto roomPos = _arena->pos();
    const auto roomSize = _arena->size();
    const auto roomEnd = roomPos + roomSize;

    _pos.x = std::clamp(_pos.x, roomPos.x, roomEnd.x - _size.x);
    _pos.y = std::clamp(_pos.y, roomPos.y, roomEnd.y - _size.y);
}