#pragma once
#include "types.hpp"
#include <vector>
#include <map>

namespace lulu
{
    class Actor;
    class Movable;
    class Arena
    {
    private:
        pair _pos;
        pair _size;
        int _count; // Contatore dei tick
        std::vector<Key> _currKeys;
        std::vector<Key> _prevKeys;
        std::vector<Actor *> _actors;
        std::map<Actor *, std::vector<Actor *>> _collisions; // Mappa attore -> lista collisioni

        void detectCollisions();

    public:
        // Costruttori
        Arena(pair position, pair size) : _pos(position), _size(size), _count(0) {}

        // Getters
        const pair &pos() const { return _pos; }
        const pair &size() const { return _size; }
        const std::vector<Key> &currKeys() const { return _currKeys; }
        const std::vector<Key> &prevKeys() const { return _prevKeys; }
        const std::vector<Actor *> &actors() const { return _actors; }
        const std::map<Actor *, std::vector<Actor *>> &collisions() const { return _collisions; }

        
        void spawn(Actor *actor);
        void kill(Actor *actor);
        void tick(const std::vector<Key> &keys = {});
    };
} // namespace lulu
