#pragma once
#include "actor.hpp"

namespace lulu
{
class Door final : public Actor
{
    Vec2<float> spawn_;
    std::string destination_;

  public:
    Door(const Vec2<float> pos, const Vec2<float> size, const Vec2<float> spawn, const std::string &destination)
        : Actor(pos, size), spawn_(spawn), destination_(destination) {}

    [[nodiscard]] Vec2<float> spawn() const { return spawn_; }
    [[nodiscard]] std::string destination() const { return destination_; }
};

} // namespace lulu
