#pragma once
#include "actor.hpp"
#include "movable.hpp"
#include "types.hpp"

namespace lulu
{
/**
 * @brief Base class for combat-capable actors
 * Fighter extends Actor and Movable to provide combat functionality
 * including health points, damage dealing, attack mechanics.
 */
class Fighter : public Actor, public Movable
{
  protected:
    float _hp;     // Current health points
    float _damage; // Damage dealt per attack
    bool _isAttacking;             // Whether currently performing attack
    bool _isHurt;

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
    Fighter(const pair position, const pair size, const pair speed, const float hp, const float damage,
            Arena *arena = nullptr, const std::string &sprite = "")
        : Actor(position, size, arena, sprite), Movable(speed, true), _hp(hp), _damage(damage), _isAttacking(false),
          _isHurt(false)
    {
    }

    /**
     * @brief Get all colliding fighters from current collision map
     * @return Vector of Fighter pointers that this actor is colliding with
     */
    [[nodiscard]] std::vector<Fighter *> getCollidingFighters() const;

    /**
     * @brief Check if actor is colliding with any static objects
     * @return True if colliding with non-movable actors
     */
    [[nodiscard]] bool isCollidingWithStatic() const;

    /**
     * @brief Deal damage to all colliding fighters
     */
    void damageCollidingFighters() const;

    /**
 * @brief Execute attack frame and handle damage
 */
    virtual void performAttack() = 0;

    /**
     * @brief End attack sequence and restore normal state
     */
    virtual void endAttack() = 0;

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

    /**
     * @brief Get current health points
     * @return Current HP
     */
    [[nodiscard]] float getHP() const
    {
        return _hp;
    }

    /**
     * @brief Get damage value
     * @return Damage per attack
     */
    [[nodiscard]] float getDamage() const
    {
        return _damage;
    }
};
} // namespace lulu