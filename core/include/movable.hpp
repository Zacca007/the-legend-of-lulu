#pragma once
#include "types.hpp"

namespace lulu
{
    class Movable
    {
    protected:
        pair _speed;

        // Costruttore protected: può essere chiamato solo dalle classi derivate
        Movable(pair speed) : _speed(speed) {}

    public:
        virtual ~Movable() = default;

        virtual void move() = 0; // Metodo virtuale puro (astratto)

        const pair &speed() const { return _speed; }
    };
}