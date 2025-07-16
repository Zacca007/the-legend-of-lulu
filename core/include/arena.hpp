#pragma once
#include "types.hpp"
#include <map>
#include <vector>

namespace lulu
{
class Actor;
class Movable;

class Arena
{
  private:
    // attributes
    pair _pos;                                             // Arena position
    pair _size;                                            // Arena size
    std::vector<Key> _currKeys;                            // Current frame input
    std::vector<Key> _prevKeys;                            // Previous frame input
    std::vector<Actor *> _actors;                          // All actors in the arena
    std::map<Actor *, std::vector<collision>> _collisions; // Collision data per actor

    // internal methods for collisions
    void detectCollisionsFor(Actor *actor);
    void detectCollisions();

  public:
    Arena(pair position, pair size);

    [[nodiscard]] const pair &pos() const;
    [[nodiscard]] const pair &size() const;
    [[nodiscard]] const std::vector<Key> &currKeys() const;
    [[nodiscard]] const std::vector<Key> &prevKeys() const;
    [[nodiscard]] const std::vector<Actor *> &actors() const;
    [[nodiscard]] const std::map<Actor *, std::vector<collision>> &collisions() const;

    // main methods
    void spawn(Actor *actor);
    void kill(Actor *actor);
    void tick(const std::vector<Key> &keys = {});

    // utility methods
    static bool hasKey(const std::vector<Key> &keys, Key key);
    bool isKeyJustPressed(Key key) const;
};
} // namespace lulu