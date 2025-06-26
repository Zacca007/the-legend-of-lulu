#pragma once
#include "types.hpp"
#include <vector>

namespace lulu
{
    class Actor;
    class Arena
    {
    private:
        // attrs
        pair _pos;
        pair _size;
        std::vector<Key> _currKeys;
        std::vector<Key> _prevKeys;
        std::vector<Actor *> _actors;

    public:
        // constructors
        Arena(pair position, pair size) : _pos(position), _size(size) {};

        // getters
        const pair &pos() const { return _pos; }
        const pair &size() const { return _size; }
        const std::vector<Key> &currKeys() const { return _currKeys; }
        const std::vector<Key> &prevKeys() const { return _prevKeys; }
        const std::vector<Actor *> &actors() const { return _actors; }

        void spawn(Actor *actor);
        void kill(Actor *actor);
    };
} // namespace lulu
