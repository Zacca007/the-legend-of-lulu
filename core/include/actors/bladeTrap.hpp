#pragma once
#include "actors/fighter.hpp"

namespace lulu
{
/**
 * @brief Trap enemy that attacks in straight lines
 * BladeTrap stays stationary until a target enters its axis (X or Y),
 * then charges at high speed in that direction until hitting a wall or static object.
 * Returns to starting position at slower speed.
 */
class BladeTrap final : public Fighter
{
public:
    enum class State {
        IDLE,       // Waiting for target to enter axis
        CHARGING,   // Moving toward target at high speed
        RETURNING   // Moving back to starting position at low speed
    };

private:
    pair _homePosition;      // Original position to return to
    State _currentState;     // Current behavior state
    direction _chargeDirection; // Direction of current charge
    float _chargeSpeed;      // Speed multiplier when charging
    float _returnSpeed;      // Speed multiplier when returning
    float _detectionRange;   // Range within which to detect targets

    /**
     * @brief Check if any target is in our axis and within detection range
     * @return Direction to charge, or D_STILL if no valid target
     */
    [[nodiscard]] direction detectTargetInAxis() const;

    /**
     * @brief Check if we should stop charging (hit wall or static object)
     * @return True if should stop charging
     */
    [[nodiscard]] bool shouldStopCharging() const;

    /**
     * @brief Check if we've returned close enough to home position
     * @return True if close enough to home position
     */
    [[nodiscard]] bool hasReachedHome() const;

    /**
     * @brief Update behavior based on current state
     */
    void updateBehavior();

public:
    /**
     * @brief Construct BladeTrap
     * @param position Initial position (becomes home position)
     * @param size Trap size
     * @param baseSpeed Base movement speed
     * @param hp Health points
     * @param damage Attack damage
     * @param chargeSpeed Speed multiplier when charging (default 3.0)
     * @param returnSpeed Speed multiplier when returning (default 0.5)
     * @param detectionRange Detection range for targets (default 200.0)
     * @param arena Pointer to arena
     * @param sprite Sprite file path
     */
    BladeTrap(pair position, pair size, float baseSpeed, float hp, float damage,
              float chargeSpeed = 3.0f, float returnSpeed = 0.5f, float detectionRange = 200.0f,
              Arena *arena = nullptr, const std::string &sprite = "");

    /**
     * @brief Update BladeTrap's position and behavior
     * Override from Movable base class
     */
    void move() override;

    /**
     * @brief Handle collision responses
     * @param collisions Vector of collisions to resolve
     */
    void handleCollisions(const std::vector<collision> &collisions) override;

    /**
     * @brief Get current state
     * @return Current behavior state
     */
    [[nodiscard]] State getCurrentState() const { return _currentState; }

    /**
     * @brief Check if trap is currently dangerous (charging)
     * @return True if in charging state
     */
    [[nodiscard]] bool isCharging() const { return _currentState == State::CHARGING; }
};
}