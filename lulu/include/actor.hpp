#pragma once
#include "types.hpp"
#include <string>

namespace lulu
{
class Arena;
class Actor
{
  protected:
    Vec2<float> pos_, size_{};
    std::string sprite_;
    Arena *arena_;

    void keepInsideArena();

  public:
    Actor(Vec2<float> pos, Vec2<float> size, const std::string &sprite = "");
    explicit Actor(Vec2<float> pos, const std::string &configPath);
    virtual ~Actor() = default;

    [[nodiscard]] const Vec2<float> &pos() const;
    [[nodiscard]] const Vec2<float> &size() const;
    [[nodiscard]] const std::string &sprite() const;
    [[nodiscard]] Arena *arena() const;

    void setArena(Arena *arena);

    Direction checkCollision(const Actor* other) const;
    virtual void handleCollision(Collision collision);
};
} // namespace lulu
