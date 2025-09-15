#pragma once
#include "actor.hpp"

namespace lulu
{
class Door final : public Actor
{
    Vec2<float> spawn_;
    std::string destination_;
    bool changeMusic_;

  public:
    Door(Vec2<float> pos, Vec2<float> size, Vec2<float> spawn, const std::string &destination, bool changeMusic)
        : Actor(pos, size), spawn_(spawn), destination_(destination), changeMusic_(changeMusic) {}

    [[nodiscard]] Vec2<float> spawn() const{ return spawn_; }
    [[nodiscard]] std::string destination() const { return destination_; }
    [[nodiscard]] bool changeMusic() const { return changeMusic_; }
};
} // namespace lulu
