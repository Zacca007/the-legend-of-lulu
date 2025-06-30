#include "actors/link.hpp"
#include "arena.hpp"
#include <algorithm>
using lulu::Link;

void Link::move()
{
    pair roomPos = _arena->pos();
    pair roomSize = _arena->size();
    pair fullRoom = roomPos + roomSize;

    bool w = false, a = false, s = false, d = false;
    for (Key key : _arena->currKeys())
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
        default:
            break;
        }
    }

    pair diagonal = _speed.diagonal(); // equals speed.x and speed.y
    if (w && a)
        _pos -= diagonal;
    else if (s && d)
        _pos += diagonal;
    else if (w && d)
    {
        _pos.x += diagonal.x;
        _pos.y -= diagonal.y;
    }
    else if (s && a)
    {
        _pos.x -= diagonal.x;
        _pos.y += diagonal.y;
    }
    else if (w)
        _pos.y -= _speed.y;
    else if (a)
        _pos.x -= _speed.x;
    else if (s)
        _pos.y += _speed.y;
    else if (d)
        _pos.x += _speed.x;

    for (const Actor *other : _arena->actors())
    {
        if (other != this)
        {
            collision coll = checkCollision(other);
            switch (coll)
            {
            case C_TOP:
                _pos.y = other->pos().y + other->size().y;
                break;

            case C_BOTTOM:
                _pos.y = other->pos().y - _size.y;
                break;

            case C_LEFT:
                _pos.x = other->pos().x + other->size().x;
                break;
            case C_RIGHT:
                _pos.x = other->pos().x - _size.x;

            default:
                break;
            }
        }
    }

    // Gestisci i confini della stanza
    if (_pos.x + _size.x >= fullRoom.x)
        _pos.x = fullRoom.x - _size.x;
    if (_pos.y + _size.y >= fullRoom.y)
        _pos.y = fullRoom.y - _size.y;
    if (_pos.x <= roomPos.x)
        _pos.x = roomPos.x;
    if (_pos.y <= roomPos.y)
        _pos.y = roomPos.y;
}