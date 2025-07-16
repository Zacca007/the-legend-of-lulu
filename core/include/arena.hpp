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
    pair pos_;                                             // Arena position
    pair size_;                                            // Arena size
    std::vector<Key> currKeys_;                            // Current frame input
    std::vector<Key> prevKeys_;                            // Previous frame input
    std::vector<Actor *> actors_;                          // All actors in the arena
    std::map<Actor *, std::vector<collision>> collisions_; // Collision data per actor

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