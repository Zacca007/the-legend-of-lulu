#pragma once
#include "actor.hpp"
#include "movable.hpp"
#include "types.hpp"

namespace lulu
{
class Fighter : public Actor, public Movable
{
  protected:
    float hp_;     // Current health points
    float damage_; // Damage dealt per attack

    virtual void setupAttack() = 0;
    virtual void performAttack() = 0;
    virtual void endAttack() = 0;

    //TODO: bisogna aggiungere metodi in questa classe o nella figlia link, per le collisioni con muri e attori statici

    Fighter(Vec2<float> position, Vec2<float> size, Vec2<float> speed, float hp, float damage,
            const std::string &sprite = "");
    explicit Fighter(Vec2<float> pos, const std::string &configPath);

  public:
    [[nodiscard]] bool isAlive() const;
    [[nodiscard]] float hp() const;
    [[nodiscard]] float damage() const;

    void takeDamage(float damage);
    void attack(Fighter &fighter) const;
};
} // namespace lulu