#include "actors/link.hpp"
#include "arena.hpp"
#include <cmath>

using namespace lulu;

// Constructor: Initialize Link with position, size, speed, health, damage, and arena reference
Link::Link(const pair position, const pair size, float speed, float hp, float damage, Arena *arena)
    : Fighter(position, size, {speed, speed}, hp, damage, arena), _attackFrame(0), _previousFrame(0),
      _animationSwitch(0)
{
    setupAnimation();
    _animation.set(S_STILL, D_DOWN); // Start facing down in still state
    _sprite = _animation.nextSprite();
}

// Setup all animation sequences for Link's different states and directions
void Link::setupAnimation()
{
    // Movement animation sprites for each direction
    const std::vector<std::string> up = {"assets/link/movement/link up 1.png",
                                         "assets/link/movement/link up 2.png"};
    const std::vector<std::string> down = {"assets/link/movement/link front 1.png",
                                           "assets/link/movement/link front 2.png"};
    const std::vector<std::string> left = {"assets/link/movement/link left 1.png",
                                           "assets/link/movement/link left 2.png"};
    const std::vector<std::string> right = {"assets/link/movement/link right 1.png",
                                            "assets/link/movement/link right 2.png"};

    // Attack animation sprites for all directions
    const std::vector<std::string> attackUp = {
        "assets/link/attack/link attack up 1.png", "assets/link/attack/link attack up 2.png",
        "assets/link/attack/link attack up 3.png", "assets/link/attack/link attack up 4.png"};
    const std::vector<std::string> attackDown = {
        "assets/link/attack/link attack down 1.png", "assets/link/attack/link attack down 2.png",
        "assets/link/attack/link attack down 3.png", "assets/link/attack/link attack down 4.png"};
    const std::vector<std::string> attackLeft = {
        "assets/link/attack/link attack left 1.png", "assets/link/attack/link attack left 2.png",
        "assets/link/attack/link attack left 3.png", "assets/link/attack/link attack left 4.png"};
    const std::vector<std::string> attackRight = {
        "assets/link/attack/link attack right 1.png", "assets/link/attack/link attack right 2.png",
        "assets/link/attack/link attack right 3.png", "assets/link/attack/link attack right 4.png"};

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

// Determine Link's current state based on input and attack status
state Link::updatedState() const
{
    // If currently in attack animation, maintain attack state
    if (_attackFrame > 0)
        return S_ATTACK;

    // Check if attack key was just pressed
    if (_arena && _arena->isKeyJustPressed(K_SPACE))
        return S_ATTACK;

    // Check if movement keys are pressed
    if (updatedDirection() != D_STILL)
        return S_MOVEMENT;

    // Default to still state
    return S_STILL;
}

// Get current directional input from arena
direction Link::updatedDirection() const
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

// Calculate movement vector based on direction and speed
pair Link::calculateMovement(direction dir) const
{
    pair movement;

    switch (dir)
    {
    case D_UP:
        movement = {0, -_speed.y};
        break;
    case D_DOWN:
        movement = {0, _speed.y};
        break;
    case D_LEFT:
        movement = {-_speed.x, 0};
        break;
    case D_RIGHT:
        movement = {_speed.x, 0};
        break;
    case D_UPLEFT:
        movement = {-_speed.x, -_speed.y};
        break;
    case D_UPRIGHT:
        movement = {_speed.x, -_speed.y};
        break;
    case D_DOWNLEFT:
        movement = {-_speed.x, _speed.y};
        break;
    case D_DOWNRIGHT:
        movement = {_speed.x, _speed.y};
        break;
    case D_STILL:
    default:
        movement = {0, 0};
        break;
    }

    // Normalize diagonal movement to prevent faster diagonal movement
    if (dir == D_UPLEFT || dir == D_UPRIGHT || dir == D_DOWNLEFT || dir == D_DOWNRIGHT)
    {
        const float diagonalFactor = 1.0f / std::sqrt(2.0f);
        movement = movement * diagonalFactor;
    }

    return movement;
}

// Initialize attack sequence
void Link::setupAttack()
{
    _attackFrame = 1; // Start attack sequence
    _previousFrame = _animation.currentFrame();
    _animation.set(S_ATTACK, _animation.currentDirection());
}

// Execute attack animation and damage calculation
void Link::performAttack()
{
    // Deal damage on frame 2 of attack animation (mid-swing)
    if (_attackFrame == 2)
    {
        // Find colliding fighters and damage them
        if (_arena)
        {
            const auto &collisions = _arena->collisions();
            auto it = collisions.find(this);
            if (it != collisions.end())
            {
                for (const auto &collision : it->second)
                {
                    if (auto *fighter = dynamic_cast<Fighter *>(collision.target))
                    {
                        fighter->takeDamage(_damage);
                    }
                }
            }
        }
    }

    // Advance to next sprite frame
    _sprite = _animation.nextSprite();

    // Adjust position based on sprite size changes during attack animation
    const auto spriteDimension = AnimationHandler::getSpriteDimension(_sprite);
    if (spriteDimension.has_value())
    {
        const pair spriteSize = spriteDimension.value();
        const pair sizeDiff = spriteSize - _size;
        adjustPositionForAttack(sizeDiff);
        _size = spriteSize;
    }

    ++_attackFrame;
}

// Clean up after attack animation completes
void Link::endAttack()
{
    _attackFrame = 0;

    // Return to movement animation state
    _animation.set(S_MOVEMENT, _animation.currentDirection());

    // Fast-forward animation to where it was before attack started
    while (_animation.currentFrame() < _previousFrame - 1)
        _animation.nextSprite();

    _sprite = _animation.nextSprite();

    // Readjust position and size back to normal movement sprites
    const auto spriteDimension = AnimationHandler::getSpriteDimension(_sprite);
    if (spriteDimension.has_value())
    {
        const pair spriteSize = spriteDimension.value();
        const pair sizeDiff = spriteSize - _size;
        adjustPositionForAttack(sizeDiff);
        _size = spriteSize;
    }
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
    const state newState = updatedState();
    const direction newDirection = updatedDirection();

    if (newState == S_STILL)
    {
        if (_animation.currentState() != S_STILL)
            _animation.set(S_STILL, _animation.currentDirection());
    }
    else if (newState == S_MOVEMENT)
    {
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
        if (_attackFrame == 0)
            setupAttack();

        performAttack();

        // End attack when animation sequence is complete
        if (_attackFrame > _animation.currentAnimation().size())
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
    if (_attackFrame > 0)
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