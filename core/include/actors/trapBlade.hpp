#pragma once
#include "actors/fighter.hpp"
#include <cstdint>

namespace lulu
{
/**
 * @brief Intelligent trap enemy with state-based AI
 * 
 * BladeTrap is a stationary enemy that detects nearby players and charges
 * at them with a blade attack. It has three main states: idle (waiting),
 * charging (moving toward target), and returning (going back to origin).
 */
class BladeTrap final : public Fighter
{
public:
    /**
     * @brief AI states for BladeTrap behavior
     */
    enum class TrapState
    {
        IDLE,     // Waiting for player detection
        CHARGING, // Moving toward detected target
        RETURNING // Returning to original position
    };

private:
    // AI State Management
    TrapState _currentState;        // Current AI state
    std::uint8_t _idleFrames;       // Frames spent in idle state
    std::uint8_t _chargeFrames;     // Frames spent charging
    
    // Position and Movement
    pair _originalPosition;         // Starting position to return to
    pair _targetPosition;           // Current target position
    direction _chargeDirection;     // Direction of charge attack
    
    // Detection and Timing
    float _detectionRange;          // Range for player detection
    float _chargeSpeed;             // Speed multiplier during charge
    std::uint8_t _maxChargeFrames;  // Maximum frames to charge
    std::uint8_t _cooldownFrames;   // Frames to wait before next detection
    std::uint8_t _currentCooldown;  // Current cooldown counter
    
    // Damage and Combat
    bool _canDamage;                // Whether trap can currently damage
    std::uint8_t _damageFrames;     // Frames damage is active during charge
    
    /**
     * @brief Initialize all animation sequences for BladeTrap
     */
    void setupAnimations();
    
    /**
     * @brief Update AI state based on current conditions
     */
    void updateAI();
    
    /**
     * @brief Handle idle state behavior
     */
    void handleIdleState();
    
    /**
     * @brief Handle charging state behavior
     */
    void handleChargingState();
    
    /**
     * @brief Handle returning state behavior
     */
    void handleReturningState();
    
    /**
     * @brief Detect nearby players within range
     * @return Pointer to closest player, or nullptr if none found
     */
    Fighter* detectNearbyPlayer() const;
    
    /**
     * @brief Calculate direction to target position
     * @param target Target position
     * @return Direction enum value
     */
    direction calculateDirectionTo(const pair& target) const;
    
    /**
     * @brief Calculate distance to target position
     * @param target Target position
     * @return Distance as float
     */
    float calculateDistanceTo(const pair& target) const;
    
    /**
     * @brief Check if trap has reached target position
     * @param target Target position
     * @param tolerance Distance tolerance
     * @return True if at target position
     */
    bool hasReachedPosition(const pair& target, float tolerance = 5.0f) const;
    
    /**
     * @brief Move toward target position
     * @param target Target position
     * @param speedMultiplier Speed multiplier
     */
    void moveToward(const pair& target, float speedMultiplier = 1.0f);
    
    /**
     * @brief Start charge attack toward target
     * @param target Target position
     */
    void startCharge(const pair& target);
    
    /**
     * @brief Start return to original position
     */
    void startReturn();
    
    /**
     * @brief Reset trap to idle state
     */
    void resetToIdle();

public:
    /**
     * @brief Construct BladeTrap enemy
     * @param position Initial position (also original position)
     * @param size Trap size
     * @param speed Movement speed
     * @param hp Health points
     * @param damage Attack damage
     * @param detectionRange Range for player detection
     * @param chargeSpeed Speed multiplier during charge
     * @param arena Pointer to arena
     */
    BladeTrap(pair position, pair size, pair speed, float hp, float damage, 
              float detectionRange = 100.0f, float chargeSpeed = 2.0f, 
              Arena* arena = nullptr);
    
    /**
     * @brief Update BladeTrap position and AI
     * Override from Movable base class
     */
    void move() override;
    
    /**
     * @brief Handle collision responses
     * @param collisions Vector of collisions to resolve
     */
    void handleCollisions(const std::vector<collision>& collisions) override;
    
    /**
     * @brief Get current AI state
     * @return Current TrapState
     */
    [[nodiscard]] TrapState getCurrentState() const { return _currentState; }
    
    /**
     * @brief Get detection range
     * @return Detection range as float
     */
    [[nodiscard]] float getDetectionRange() const { return _detectionRange; }
    
    /**
     * @brief Get original position
     * @return Original position pair
     */
    [[nodiscard]] const pair& getOriginalPosition() const { return _originalPosition; }
    
    /**
     * @brief Check if trap can currently damage
     * @return True if damage is active
     */
    [[nodiscard]] bool canDamage() const { return _canDamage; }
    
    /**
     * @brief Set detection range
     * @param range New detection range
     */
    void setDetectionRange(float range) { _detectionRange = range; }
    
    /**
     * @brief Set charge speed multiplier
     * @param speed New charge speed multiplier
     */
    void setChargeSpeed(float speed) { _chargeSpeed = speed; }
};
} // namespace lulu