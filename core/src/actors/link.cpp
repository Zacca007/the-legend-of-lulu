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

    // Salva la posizione precedente
    pair oldPos = _pos;

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
        _pos.y -= _speed.y; // Corretto: era *pos.y -= *speed.y;
    else if (a)
        _pos.x -= _speed.x; // Corretto: era *pos.x -= *speed.x;
    else if (s)
        _pos.y += _speed.y; // Corretto: era *pos.y += *speed.y;
    else if (d)
        _pos.x += _speed.x; // Corretto: era *pos.x += *speed.x;

    // Controlla collisioni con altri attori
    if (_arena)
    {
        // Controlla se la nuova posizione causa collisioni
        const std::vector<Actor *> &actors = _arena->actors();

        for (Actor *other : actors)
        {
            if (other != this) // Non controllare collisioni con se stesso
            {
                collision collType = this->checkCollision(other);
                if (collType != C_NONE)
                {

                    // Gestisci la collisione in base al tipo
                    switch (collType)
                    {
                    case C_RIGHT:
                        // Collisione dal lato destro, impedisci movimento verso destra
                        if (_pos.x > oldPos.x)
                            _pos.x = oldPos.x;
                        break;
                    case C_LEFT:
                        // Collisione dal lato sinistro, impedisci movimento verso sinistra
                        if (_pos.x < oldPos.x)
                            _pos.x = oldPos.x;
                        break;
                    case C_BOTTOM:
                        // Collisione dal lato inferiore, impedisci movimento verso il basso
                        if (_pos.y > oldPos.y)
                            _pos.y = oldPos.y;
                        break;
                    case C_TOP:
                        // Collisione dal lato superiore, impedisci movimento verso l'alto
                        if (_pos.y < oldPos.y)
                            _pos.y = oldPos.y;
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        // Se c'è ancora una collisione (casi complessi), ripristina completamente la posizione
        for (Actor *other : actors)
        {
            if (other != this && this->checkCollision(other) != C_NONE)
            {
                _pos = oldPos;
                break;
            }
        }
    }

    // Gestisci i confini della stanza
    if (_pos.x + _size.x >= fullRoom.x)
        _pos.x = fullRoom.x - _size.x; // Corretto: era *pos.x = fullRoom.x - *size.x;
    if (_pos.y + _size.y >= fullRoom.y)
        _pos.y = fullRoom.y - _size.y; // Corretto: era *pos.y = fullRoom.y - *size.y;
    if (_pos.x <= roomPos.x)
        _pos.x = roomPos.x;
    if (_pos.y <= roomPos.y)
        _pos.y = roomPos.y;
}