#pragma once
#include "fighter.hpp"

namespace lulu
{
class Link final : public Fighter
{
    [[nodiscard]] State updatedState() const override;
    [[nodiscard]] Direction updatedDirection() const override;
    [[nodiscard]] Vec2<float> calculateMovement(Direction dir) const override;

    // TODO: implement these things
    void setupAttack() override;
    void performAttack() override;
    void endAttack() override;
    void adjustPositionForAttack(const Vec2<float> &sizeDiff);

  public:
    // Costruttore con parsing JSON
    explicit Link(Vec2<float> pos, const std::string &configPath = "assets/link/link.json");

    void move() override;
};
} // namespace lulu