#pragma once
#include "types.hpp"

namespace lulu
{
    class IMovable
    {
    protected:
        pair _speed;
        virtual void move();
        virtual ~IMovable() = default;
    };
}