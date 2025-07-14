#pragma once
#include "actor.hpp"
#include "movable.hpp"
#include "types.hpp"

namespace lulu
{
/**
 * @brief Base class for combat-capable actors
 * * Fighter extends Actor and Movable to provide combat functionality
 * including health points, damage dealing, and attack mechanics.
 */
class Fighter : public Actor, public Movable
{
  protected:
    float _hp;     // Current health points
    float _damage; // Damage dealt per attack

    /**
     * @brief Protected constructor for Fighter
     * @param position Initial position
     * @param size Actor size
     * @param speed Movement speed
     * @param hp Initial health points
     * @param damage Damage per attack
     * @param arena Pointer to arena
     * @param sprite Sprite file path
     */
    Fighter(const pair position, const pair size, const pair speed, const float hp, const float damage,            Arena *arena = nullptr, const std::string &sprite = "")
        : Actor(position, size, arena, sprite), Movable(speed, true), _hp(hp), _damage(damage)
    {
    }

  public:
    /**
     * @brief Reduce health points by damage amount
     * @param damage Amount of damage to take
     */
    void takeDamage(float damage);
 /**
     * @brief Attack another fighter
     * @param fighter Target fighter to attack
     */
    void attack(Fighter &fighter) const;

    /**
     * @brief Check if fighter is still alive
     * @return True if HP > 0, false otherwise
     */
    [[nodiscard]] bool isAlive() const;
};
}