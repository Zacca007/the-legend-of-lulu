#include "actors/trapBlade.hpp"
#include "actors/link.hpp"
#include "arena.hpp"
#include <algorithm>
#include <cmath>

namespace lulu
{

BladeTrap::BladeTrap(pair position, pair size, pair speed, float hp, float damage, 
                     float detectionRange, float chargeSpeed, Arena* arena)
    : Fighter(position, size, speed, hp, damage, arena, "assets/sprites/bladetrap_idle.png"),
      _currentState(TrapState::IDLE),
      _idleFrames(0),
      _chargeFrames(0),
      _originalPosition(position),
      _targetPosition(position),
      _chargeDirection(D_STILL),
      _detectionRange(detectionRange),
      _chargeSpeed(chargeSpeed),
      _maxChargeFrames(60), // 2 seconds at 30 FPS
      _cooldownFrames(30),  // 1 second at 30 FPS
      _currentCooldown(0),
      _canDamage(false),
      _damageFrames(20)     // Damage active for ~0.67 seconds
{
    setupAnimations();
}

void BladeTrap::setupAnimations()
{
    // Idle animations - same sprite for all directions
    std::vector<std::string> idleSprites = {"assets/sprites/bladetrap_idle.png"};
    _animation.addAnimation(S_STILL, D_STILL, idleSprites);
    _animation.addAnimation(S_STILL, D_UP, idleSprites);
    _animation.addAnimation(S_STILL, D_DOWN, idleSprites);
    _animation.addAnimation(S_STILL, D_LEFT, idleSprites);
    _animation.addAnimation(S_STILL, D_RIGHT, idleSprites);
    
    // Charging animations - direction-specific sprites
    std::vector<std::string> chargeUp = {"assets/sprites/bladetrap_charge_up.png"};
    std::vector<std::string> chargeDown = {"assets/sprites/bladetrap_charge_down.png"};
    std::vector<std::string> chargeLeft = {"assets/sprites/bladetrap_charge_left.png"};
    std::vector<std::string> chargeRight = {"assets/sprites/bladetrap_charge_right.png"};
    
    _animation.addAnimation(S_MOVEMENT, D_UP, chargeUp);
    _animation.addAnimation(S_MOVEMENT, D_DOWN, chargeDown);
    _animation.addAnimation(S_MOVEMENT, D_LEFT, chargeLeft);
    _animation.addAnimation(S_MOVEMENT, D_RIGHT, chargeRight);
    
    // Diagonal movements use closest cardinal direction sprite
    _animation.addAnimation(S_MOVEMENT, D_UPLEFT, chargeLeft);
    _animation.addAnimation(S_MOVEMENT, D_UPRIGHT, chargeRight);
    _animation.addAnimation(S_MOVEMENT, D_DOWNLEFT, chargeLeft);
    _animation.addAnimation(S_MOVEMENT, D_DOWNRIGHT, chargeRight);
    
    // Set initial animation state
    _animation.set(S_STILL, D_STILL);
}

void BladeTrap::move()
{
    updateAI();
    
    // Handle state-specific behavior
    switch (_currentState)
    {
        case TrapState::IDLE:
            handleIdleState();
            break;
        case TrapState::CHARGING:
            handleChargingState();
            break;
        case TrapState::RETURNING:
            handleReturningState();
            break;
    }
    
    // Update sprite to current animation frame
    _sprite = _animation.nextSprite();
}

void BladeTrap::updateAI()
{
    // Update cooldown
    if (_currentCooldown > 0)
    {
        _currentCooldown--;
    }
    
    // Update damage state
    if (_canDamage && _chargeFrames > _damageFrames)
    {
        _canDamage = false;
    }
}

void BladeTrap::handleIdleState()
{
    _idleFrames++;
    
    // Only detect players if not on cooldown
    if (_currentCooldown == 0)
    {
        Fighter* target = detectNearbyPlayer();
        if (target != nullptr)
        {
            startCharge(target->pos());
        }
    }
    
    // Ensure still animation is playing
    if (_animation.currentState() != S_STILL)
    {
        _animation.set(S_STILL, D_STILL);
    }
}

void BladeTrap::handleChargingState()
{
    _chargeFrames++;
    
    // Check if charge duration exceeded
    if (_chargeFrames >= _maxChargeFrames)
    {
        startReturn();
        return;
    }
    
    // Move toward target
    moveToward(_targetPosition, _chargeSpeed);
    
    // Check if reached target or hit static obstacle
    if (hasReachedPosition(_targetPosition, 10.0f) || isCollidingWithStatic())
    {
        startReturn();
    }
    
    // Damage nearby fighters if in damage phase
    if (_canDamage)
    {
        damageCollidingFighters();
    }
}

void BladeTrap::handleReturningState()
{
    // Move back to original position
    moveToward(_originalPosition, 1.0f);
    
    // Check if reached original position
    if (hasReachedPosition(_originalPosition, 5.0f))
    {
        resetToIdle();
    }
}

Fighter* BladeTrap::detectNearbyPlayer() const
{
    if (!_arena) return nullptr;
    
    Fighter* closestPlayer = nullptr;
    float closestDistance = _detectionRange;
    
    // Check all actors in arena
    for (Actor* actor : _arena->actors())
    {
        // Try to cast to Fighter
        auto* fighter = dynamic_cast<Fighter*>(actor);
        if (!fighter || fighter == this) continue;
        
        // Check if it's the player (Link)
        Link* link = dynamic_cast<Link*>(fighter);
        if (link && link->isAlive())
        {
            float distance = calculateDistanceTo(link->pos());
            if (distance < closestDistance)
            {
                closestDistance = distance;
                closestPlayer = fighter;
            }
        }
    }
    
    return closestPlayer;
}

direction BladeTrap::calculateDirectionTo(const pair& target) const
{
    pair diff = target - _pos;
    
    // Determine primary direction based on larger component
    if (std::abs(diff.x) > std::abs(diff.y))
    {
        // Horizontal movement primary
        if (diff.x > 0)
        {
            return diff.y > 0 ? D_DOWNRIGHT : diff.y < 0 ? D_UPRIGHT : D_RIGHT;
        }
        return diff.y > 0 ? D_DOWNLEFT : diff.y < 0 ? D_UPLEFT : D_LEFT;
    }
    // Vertical movement primary
    if (diff.y > 0)
    {
        return diff.x > 0 ? D_DOWNRIGHT : diff.x < 0 ? D_DOWNLEFT : D_DOWN;
    }
    return diff.x > 0 ? D_UPRIGHT : diff.x < 0 ? D_UPLEFT : D_UP;
}

float BladeTrap::calculateDistanceTo(const pair& target) const
{
    auto [x, y] = target - _pos;
    return std::sqrt(x * x + y * y);
}

bool BladeTrap::hasReachedPosition(const pair& target, float tolerance) const
{
    return calculateDistanceTo(target) <= tolerance;
}

void BladeTrap::moveToward(const pair& target, float speedMultiplier)
{
    direction dir = calculateDirectionTo(target);
    pair movement = calculateMovement(dir, speedMultiplier);
    
    // Update position
    _pos += movement;
    
    // Clamp to arena bounds
    clampToArena();
    
    // Update animation
    if (dir != D_STILL)
    {
        _animation.set(S_MOVEMENT, dir);
    }
}

void BladeTrap::startCharge(const pair& target)
{
    _currentState = TrapState::CHARGING;
    _targetPosition = target;
    _chargeDirection = calculateDirectionTo(target);
    _chargeFrames = 0;
    _canDamage = true;
    
    // Set charging animation
    _animation.set(S_MOVEMENT, _chargeDirection);
}

void BladeTrap::startReturn()
{
    _currentState = TrapState::RETURNING;
    _canDamage = false;
    _chargeFrames = 0;
    
    // Set movement animation toward original position
    direction returnDir = calculateDirectionTo(_originalPosition);
    _animation.set(S_MOVEMENT, returnDir);
}

void BladeTrap::resetToIdle()
{
    _currentState = TrapState::IDLE;
    _pos = _originalPosition;
    _idleFrames = 0;
    _chargeFrames = 0;
    _canDamage = false;
    _currentCooldown = _cooldownFrames;
    
    // Set idle animation
    _animation.set(S_STILL, D_STILL);
}

void BladeTrap::handleCollisions(const std::vector<collision>& collisions)
{
    for (const auto& col : collisions)
    {
        // If charging and hit a static object, start returning
        if (_currentState == TrapState::CHARGING && col.target)
        {
            // Check if colliding with static object (non-movable)
            if (dynamic_cast<Movable*>(col.target) == nullptr)
            {
                startReturn();
                break;
            }
        }
        
        // If colliding with player during charge, deal damage
        if (_currentState == TrapState::CHARGING && _canDamage)
        {
            auto* fighter = dynamic_cast<Fighter*>(col.target);
            if (fighter && dynamic_cast<Link*>(fighter))
            {
                attack(*fighter);
                // Continue charging after hit
            }
        }
    }
}

} // namespace lulu