#pragma once
#include "animationHandler.hpp"
#include "types.hpp"

namespace lulu
{

class Movable
{
  protected:
    Vec2<float> speed_{};
    AnimationHandler movement_; // Animation handler for sprite management

    explicit Movable(Vec2<float> pos, bool enableAnimation = false);
    explicit Movable(const std::string &configPath);

    // internal movement methods
    [[nodiscard]] virtual State updatedState() const = 0;
    [[nodiscard]] virtual Direction updatedDirection() const = 0;
    [[nodiscard]] virtual Vec2<float> calculateMovement(Direction dir) const = 0;

  public:
    virtual ~Movable() = default;

    [[nodiscard]] const Vec2<float> &speed() const;
    virtual void move() = 0;
};

} // namespace lulu
