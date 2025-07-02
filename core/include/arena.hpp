#pragma once
#include "types.hpp"
#include <vector>
#include <map>

namespace lulu {
    class Actor;
    class Movable;

    class Arena {
    private:
        pair _pos;
        pair _size;
        std::vector<Key> _currKeys;
        std::vector<Key> _prevKeys;
        std::vector<Actor *> _actors;
        std::map<Actor *, std::vector<collision>> _collisions;

        void detectCollisionsFor(Actor *actor);
        void detectCollisions();

    public:
        Arena(const pair position, const pair size) : _pos(position), _size(size) {}

        [[nodiscard]] const pair &pos() const { return _pos; }
        [[nodiscard]] const pair &size() const { return _size; }
        [[nodiscard]] const std::vector<Key> &currKeys() const { return _currKeys; }
        [[nodiscard]] const std::vector<Key> &prevKeys() const { return _prevKeys; }
        [[nodiscard]] const std::vector<Actor *> &actors() const { return _actors; }
        [[nodiscard]] const std::map<Actor *, std::vector<collision>> &collisions() const { return _collisions; }

        void spawn(Actor *actor);
        void kill(Actor *actor);
        void tick(const std::vector<Key> &keys = {});
    };
} // namespace lulu
