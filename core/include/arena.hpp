#pragma once
#include "types.hpp"
#include <vector>

namespace lulu
{
    class Arena
    {
    private:
        //attrs
        pair _pos;
        pair _size;
        std::vector<Key> _currKeys;
        std::vector<Key> _prevKeys;

    public:
        // constructors
        Arena(pair position, pair size) : _pos(position), _size(size) {};

        // getters
        const pair &pos() { return _pos; }
        const pair &size() { return _size; }
        const std::vector<Key> &currKeys() { return _currKeys; }
        const std::vector<Key> &prevKeys() { return _prevKeys; }
    };
} // namespace lulu
