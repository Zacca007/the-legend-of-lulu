#pragma once
#include "types.hpp"
#include <string>

namespace lulu
{
    class Arena;

    class Actor
    {
    protected:
        pair _pos;
        pair _size;
        Arena *_arena;
        std::string _sprite;

    public:
        Actor(pair position, pair size, Arena *arena = nullptr, const std::string &sprite = "");
        virtual ~Actor() = default;

        const pair &pos() const { return _pos; }
        const pair &size() const { return _size; }
        const Arena *arena() const { return _arena; }

        void setArena(Arena *arena);
        collision checkCollision(const Actor *other) const;
    };
}
