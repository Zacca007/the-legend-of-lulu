#include "actors/link.hpp"
#include "arena.hpp"
#include <ranges>

using namespace lulu;

// Constructor: Initialize Link with position, size, speed, health, damage, and arena reference
Link::Link(const pair position, const pair size, float speed, float hp, float damage, Arena *arena)
    : Fighter(position, size, {speed, speed}, hp, damage, arena), _isAttacking(false), _attackFrame(0),
      _animationSwitch(0)
{
    setupAnimations();
    _animation.set(S_STILL, D_DOWN); // Start facing down in still state
    _sprite = _animation.nextSprite();
}

// Setup all animation sequences for Link's different states and directions
void Link::setupAnimations()
{
    // Movement animation sprites for each direction
    const std::vector<std::string> up = {"core/assets/link/movement/link up 1.png",
                                         "core/assets/link/movement/link up 2.png"};
    const std::vector<std::string> down = {"core/assets/link/movement/link front 1.png",
                                           "core/assets/link/movement/link front 2.png"};
    const std::vector<std::string> left = {"core/assets/link/movement/link left 1.png",
                                           "core/assets/link/movement/link left 2.png"};
    const std::vector<std::string> right = {"core/assets/link/movement/link right 1.png",
                                            "core/assets/link/movement/link right 2.png"};

    // Attack animation sprites for all directions
    const std::vector<std::string> attackUp = {
        "core/assets/link/attack/link attack up 1.png", "core/assets/link/attack/link attack up 2.png",
        "core/assets/link/attack/link attack up 3.png", "core/assets/link/attack/link attack up 4.png"};
    const std::vector<std::string> attackDown = {
        "core/assets/link/attack/link attack down 1.png", "core/assets/link/attack/link attack down 2.png",
        "core/assets/link/attack/link attack down 3.png", "core/assets/link/attack/link attack down 4.png"};
    const std::vector<std::string> attackLeft = {
        "core/assets/link/attack/link attack left 1.png", "core/assets/link/attack/link attack left 2.png",
        "core/assets/link/attack/link attack left 3.png", "core/assets/link/attack/link attack left 4.png"};
    const std::vector<std::string> attackRight = {
        "core/assets/link/attack/link attack right 1.png", "core/assets/link/attack/link attack right 2.png",
        "core/assets/link/attack/link attack right 3.png", "core/assets/link/attack/link attack right 4.png"};

    // Register movement animations for all 8 directions
    _animation.addAnimation(S_MOVEMENT, D_UP, up);
    _animation.addAnimation(S_MOVEMENT, D_UPLEFT, up);
    _animation.addAnimation(S_MOVEMENT, D_UPRIGHT, up);
    _animation.addAnimation(S_MOVEMENT, D_DOWN, down);
    _animation.addAnimation(S_MOVEMENT, D_DOWNLEFT, down);
    _animation.addAnimation(S_MOVEMENT, D_DOWNRIGHT, down);
    _animation.addAnimation(S_MOVEMENT, D_LEFT, left);
    _animation.addAnimation(S_MOVEMENT, D_RIGHT, right);

    // Register idle/still animations
    _animation.addAnimation(S_STILL, D_UP, up);
    _animation.addAnimation(S_STILL, D_UPLEFT, up);
    _animation.addAnimation(S_STILL, D_UPRIGHT, up);
    _animation.addAnimation(S_STILL, D_DOWN, down);
    _animation.addAnimation(S_STILL, D_DOWNLEFT, down);
    _animation.addAnimation(S_STILL, D_DOWNRIGHT, down);
    _animation.addAnimation(S_STILL, D_LEFT, left);
    _animation.addAnimation(S_STILL, D_RIGHT, right);

    // Register attack animations for all directions
    _animation.addAnimation(S_ATTACK, D_UP, attackUp);
    _animation.addAnimation(S_ATTACK, D_UPLEFT, attackUp);
    _animation.addAnimation(S_ATTACK, D_UPRIGHT, attackUp);
    _animation.addAnimation(S_ATTACK, D_DOWN, attackDown);
    _animation.addAnimation(S_ATTACK, D_DOWNLEFT, attackDown);
    _animation.addAnimation(S_ATTACK, D_DOWNRIGHT, attackDown);
    _animation.addAnimation(S_ATTACK, D_LEFT, attackLeft);
    _animation.addAnimation(S_ATTACK, D_RIGHT, attackRight);
}

// INPUT HANDLING METHODS - specific to Link as player-controlled character

/**
 * @brief Check if input keys contain a specific key
 */
bool Link::hasKey(const std::vector<Key> &keys, Key key)
{
    return std::ranges::find(keys, key) != keys.end();
}

/**
 * @brief Check if a key was just pressed (not held from previous frame)
 */
bool Link::isKeyJustPressed(Key key) const
{
    if (!_arena)
        return false;

    const auto &currKeys = _arena->currKeys();
    const auto &prevKeys = _arena->prevKeys();

    return hasKey(currKeys, key) && !hasKey(prevKeys, key);
}

/**
 * @brief Get current directional input from arena
 */
direction Link::getCurrentDirection() const
{
    if (!_arena)
        return D_STILL;

    bool w = false, a = false, s = false, d = false;

    // Check which directional keys are currently pressed
    for (const Key key : _arena->currKeys())
    {
        switch (key)
        {
        case K_W:
        case K_UP:
            w = true;
            break;
        case K_A:
        case K_LEFT:
            a = true;
            break;
        case K_S:
        case K_DOWN:
            s = true;
            break;
        case K_D:
        case K_RIGHT:
            d = true;
            break;
        default:
            break;
        }
    }

    // Resolve conflicting inputs (opposite directions cancel out)
    if (a && d)
        a = d = false;
    if (w && s)
        w = s = false;

    // Determine final direction (diagonal directions have priority)
    if (w && a)
        return D_UPLEFT;
    if (w && d)
        return D_UPRIGHT;
    if (s && a)
        return D_DOWNLEFT;
    if (s && d)
        return D_DOWNRIGHT;
    if (w)
        return D_UP;
    if (s)
        return D_DOWN;
    if (a)
        return D_LEFT;
    if (d)
        return D_RIGHT;

    return D_STILL;
}

// Determine Link's current state based on input and attack status
state Link::updateState() const
{
    // If currently attacking, maintain attack state
    if (_isAttacking)
        return S_ATTACK;

    // Check if attack key was just pressed
    if (isKeyJustPressed(K_SPACE))
        return S_ATTACK;

    // Check if movement keys are pressed
    if (getCurrentDirection() != D_STILL)
        return S_MOVEMENT;

    // Default to still state
    return S_STILL;
}

// Initialize attack sequence
void Link::setupAttack()
{
    _isAttacking = true;
    _attackFrame = 0;
    _previousFrame = _animation.currentFrame();
    _animation.set(S_ATTACK, _animation.currentDirection());
}

// Execute attack animation and damage calculation
void Link::performAttack()
{
    // Deal damage on frame 2 of attack animation (mid-swing)
    if (_attackFrame == 2)
    {
        damageCollidingFighters();
    }

    // Advance to next sprite frame
    _sprite = _animation.nextSprite();

    // Adjust position based on sprite size changes during attack animation
    const pair spriteSize = AnimationHandler::getSpriteDimension(_sprite).value();
    const pair sizeDiff = spriteSize - _size;
    adjustPositionForAttack(sizeDiff);

    _size = spriteSize;
    ++_attackFrame;
}

// Clean up after attack animation completes
void Link::endAttack()
{
    _isAttacking = false;
    _attackFrame = 0;

    // Return to movement animation state
    _animation.set(S_MOVEMENT, _animation.currentDirection());

    // Fast-forward animation to where it was before attack started
    while (_animation.currentFrame() < _previousFrame - 1)
        _animation.nextSprite();

    _sprite = _animation.nextSprite();

    // Readjust position and size back to normal movement sprites
    const pair spriteSize = AnimationHandler::getSpriteDimension(_sprite).value();
    const pair sizeDiff = spriteSize - _size;
    adjustPositionForAttack(sizeDiff);

    _size = spriteSize;
}

// Handle sprite size changes during attack animations
void Link::adjustPositionForAttack(const pair &sizeDiff)
{
    switch (_animation.currentDirection())
    {
    case D_LEFT:
        _pos.x -= sizeDiff.x;
        // Fall through
    case D_UP:
    case D_UPLEFT:
    case D_UPRIGHT:
        _pos.y -= sizeDiff.y;
        break;
    default:
        break;
    }
}

// Main movement and animation update function
void Link::move()
{
    if (const state newState = updateState(); newState == S_STILL)
    {
        if (_animation.currentState() != S_STILL)
            _animation.set(S_STILL, _animation.currentDirection());
    }
    else if (newState == S_MOVEMENT)
    {
        const direction newDirection = getCurrentDirection();

        if (newDirection != D_STILL)
        {
            _pos += calculateMovement(newDirection);

            if (_animation.currentDirection() != newDirection)
            {
                _animation.set(S_MOVEMENT, newDirection);
                _animation.nextSprite();
                _animationSwitch = 0;
            }

            // Update sprite every 4 frames for smooth movement animation
            if (_animationSwitch++ % 4 == 0)
                _sprite = _animation.nextSprite();
        }
    }
    else if (newState == S_ATTACK)
    {
        // Initialize attack if just starting
        if (!_attackFrame)
            setupAttack();

        performAttack();

        // End attack when animation sequence is complete
        if (_attackFrame == _animation.currentAnimation().size())
        {
            endAttack();
            _animationSwitch = 0;
        }
    }
}

// Handle collisions with walls and other objects
void Link::handleCollisions(const std::vector<collision> &collisions)
{
    // Don't handle collisions during attack to prevent interrupting attack animation
    if (_isAttacking)
        return;

    // Process each collision
    for (const auto &coll : collisions)
    {
        const auto &target = coll.target;
        const auto &tPos = target->pos();
        const auto &tSize = target->size();

        // Adjust Link's position based on collision side
        switch (coll.type)
        {
        case C_TOP:
            _pos.y = tPos.y + tSize.y;
            break;
        case C_BOTTOM:
            _pos.y = tPos.y - _size.y;
            break;
        case C_LEFT:
            _pos.x = tPos.x + tSize.x;
            break;
        case C_RIGHT:
            _pos.x = tPos.x - _size.x;
            break;
        default:
            break;
        }
    }

    // Keep Link within arena boundaries
    clampToArena();
}