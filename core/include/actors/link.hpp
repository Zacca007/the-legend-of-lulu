#pragma once
#include "actors/fighter.hpp"

namespace lulu
{
class Link final : public Fighter
{
private:
    bool _isAttacking;
    std::uint8_t _attackFrame;
    std::uint8_t _previousFrame{};

    void setupAnimations();
    [[nodiscard]] state updateState() const;
    [[nodiscard]] direction updateDirection() const;
    [[nodiscard]] pair calculateMovement(direction dir) const;
    void setupAttack();
    void performAttack();
    void endAttack();
    //TODO: void handleAttackCollisions();

public:
    Link( pair position,  pair size, float speed, float hp, float damage, Arena *arena = nullptr);
    void move() override;
    void handleCollisions(const std::vector<collision> &collisions) override;
    [[nodiscard]] bool isAttacking() const { return _isAttacking; }
};
} // namespace lulu