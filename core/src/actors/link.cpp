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

    // Determina la direzione e movimento
    direction newDirection = D_STILL;

    if (w && a)
        newDirection = D_UPLEFT;
    else if (w && d)
        newDirection = D_UPRIGHT;
    else if (s && a)
        newDirection = D_DOWNLEFT;
    else if (s && d)
        newDirection = D_DOWNRIGHT;
    else if (w)
        newDirection = D_UP;
    else if (s)
        newDirection = D_DOWN;
    else if (a)
        newDirection = D_LEFT;
    else if (d)
        newDirection = D_RIGHT;

    // Movimento basato sulla direzione
    if (newDirection != D_STILL)
    {
        pair movement{0, 0};

        switch (newDirection)
        {
        case D_UPLEFT:
        case D_UPRIGHT:
        case D_DOWNLEFT:
        case D_DOWNRIGHT:
            {
                auto diagonal = _speed.diagonal();
                if (diagonal.has_value())
                {
                    pair diag = diagonal.value();
                    switch (newDirection)
                    {
                    case D_UPLEFT:
                        movement = {-diag.x, -diag.y};
                        break;
                    case D_UPRIGHT:
                        movement = {diag.x, -diag.y};
                        break;
                    case D_DOWNLEFT:
                        movement = {-diag.x, diag.y};
                        break;
                    case D_DOWNRIGHT:
                        movement = {diag.x, diag.y};
                        break;
                    }
                }
            }
            break;
        case D_UP:
            movement = {0, -_speed.y};
            break;
        case D_DOWN:
            movement = {0, _speed.y};
            break;
        case D_LEFT:
            movement = {-_speed.x, 0};
            break;
        case D_RIGHT:
            movement = {_speed.x, 0};
            break;
        }

        _pos += movement;
            // _sprite = _animation.getCurrentSprite();
    }
    if (newDirection != D_STILL)
    {
        if (newDirection != _animation.CurrentDirection())
            _animation.set(S_MOVEMENT, newDirection);
        _sprite = _animation.nextSprite();
    }
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