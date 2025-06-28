#include "actors/link.hpp"
#include "arena.hpp"
#include <algorithm>

using lulu::Link;

void Link::move()
{
    const std::vector<Key> &keys = _arena->currKeys();
    const pair &roomPos = _arena->pos();
    const pair &roomSize = _arena->size();
    pair fullRoom = roomPos + roomSize;

    for (Key key : keys)
    {
        switch (key)
        {
        case K_RIGHT:
            if (_pos.x + _size.x + _speed.x <= fullRoom.x)
                _pos.x += _speed.x;
            else
                _pos.x = fullRoom.x - _size.x;
            break;

        case K_LEFT:
            if (_pos.x - _speed.x >= roomPos.x)
                _pos.x -= _speed.x;
            else
                _pos.x = roomPos.x;
            break;

        case K_DOWN:
            if (_pos.y + _size.y + _speed.y <= fullRoom.y)
                _pos.y += _speed.y;
            else
                _pos.y = fullRoom.y - _size.y;
            break;

        case K_UP:
            if (_pos.y - _speed.y >= roomPos.y)
                _pos.y -= _speed.y;
            else
                _pos.y = roomPos.y;
            break;

        default:
            break;
        }
    }
}