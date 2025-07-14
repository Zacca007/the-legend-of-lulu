#include "actors/link.hpp"
#include "arena.hpp"
#include <algorithm>
#include <ranges>

using namespace lulu;

// Constructor: Initialize Link with position, size, speed, health, damage, and arena reference
Link::Link(const pair position, const pair size, float speed, float hp, float damage, Arena *arena)
    : Fighter(position, size, {speed, speed}, hp, damage, arena), _isAttacking(false), _attackFrame(0)
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
    _animation.addAnimation(S_MOVEMENT, D_UPLEFT, up);      // Diagonal up-left uses up sprites
    _animation.addAnimation(S_MOVEMENT, D_UPRIGHT, up);     // Diagonal up-right uses up sprites
    _animation.addAnimation(S_MOVEMENT, D_DOWN, down);
    _animation.addAnimation(S_MOVEMENT, D_DOWNLEFT, down);  // Diagonal down-left uses down sprites
    _animation.addAnimation(S_MOVEMENT, D_DOWNRIGHT, down); // Diagonal down-right uses down sprites
    _animation.addAnimation(S_MOVEMENT, D_LEFT, left);
    _animation.addAnimation(S_MOVEMENT, D_RIGHT, right);

    // Register idle/still animations (same sprites as movement, just not cycling)
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
    _animation.addAnimation(S_ATTACK, D_UPLEFT, attackUp);      // Diagonal attacks use corresponding directional sprites
    _animation.addAnimation(S_ATTACK, D_UPRIGHT, attackUp);
    _animation.addAnimation(S_ATTACK, D_DOWN, attackDown);
    _animation.addAnimation(S_ATTACK, D_DOWNLEFT, attackDown);
    _animation.addAnimation(S_ATTACK, D_DOWNRIGHT, attackDown);
    _animation.addAnimation(S_ATTACK, D_LEFT, attackLeft);
    _animation.addAnimation(S_ATTACK, D_RIGHT, attackRight);
    // TODO: ADD D_UP ATTACKS (note from original code)
}

// Determine Link's current state based on input and attack status
state Link::updateState() const
{
    // If currently attacking, maintain attack state
    if (_isAttacking)
        return S_ATTACK;

    // Get current and previous frame key states
    std::vector<Key> currKeys = _arena->currKeys();
    std::vector<Key> prevKeys = _arena->prevKeys();

    // Check if spacebar is pressed in current frame
    auto currIt = std::ranges::find(currKeys, K_SPACE);
    auto prevIt = std::ranges::find(prevKeys, K_SPACE);

    // If spacebar was held down from previous frame, remove it from current keys
    // (prevents continuous attack while holding spacebar)
    if (currIt != currKeys.end() && prevIt != prevKeys.end())
    {
        currKeys.erase(currIt);
    }
    // If spacebar is pressed this frame but wasn't pressed last frame, start attack
    else if (currIt != currKeys.end() && prevIt == prevKeys.end())
    {
        return S_ATTACK;
    }

    // If not attacking and movement keys are pressed, return movement state
    if (!currKeys.empty())
        return S_MOVEMENT;

    // Default to still state
    return S_STILL;
}

// Calculate Link's facing direction based on current input
direction Link::updateDirection() const
{
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

    // No directional input
    return D_STILL;
}

// Calculate movement vector based on direction
pair Link::calculateMovement(direction dir) const
{
    switch (dir)
    {
    case D_UP:
        return {0, -_speed.y};
    case D_DOWN:
        return {0, _speed.y};
    case D_LEFT:
        return {-_speed.x, 0};
    case D_RIGHT:
        return {_speed.x, 0};
    case D_UPLEFT:
    case D_UPRIGHT:
    case D_DOWNLEFT:
    case D_DOWNRIGHT: {
        // Calculate diagonal movement (reduced speed to maintain consistent movement distance)
        if (auto diagonal = _speed.diagonal(); diagonal.has_value())
        {
            const auto [x, y] = diagonal.value();
            switch (dir)
            {
            case D_UPLEFT:
                return {-x, -y};
            case D_UPRIGHT:
                return {x, -y};
            case D_DOWNLEFT:
                return {-x, y};
            case D_DOWNRIGHT:
                return {x, y};
            default:
                break;
            }
        }
    }
    break;

    case D_STILL:
    default:
        break;
    }
    return {0, 0}; // No movement
}

// Initialize attack sequence
void Link::setupAttack()
{
    _isAttacking = true;
    _attackFrame = 0;
    _previousFrame = _animation.currentFrame(); // Store current frame to return to after attack
    _animation.set(S_ATTACK, _animation.currentDirection()); // Start attack animation in current direction
}

// Execute attack animation and damage calculation
void Link::performAttack()
{
    // Deal damage on frame 2 of attack animation (mid-swing)
    if (_attackFrame == 2)
    {
        const auto &collisionMap = _arena->collisions();
        auto it = collisionMap.find(this);

        // Check if Link is colliding with any targets
        if (it != collisionMap.end())
        {
            for (const auto &[target, type] : it->second)
            {
                // If target is a Fighter, deal damage to it
                if (auto *fighter = dynamic_cast<Fighter *>(target))
                {
                    attack(*fighter);
                }
            }
        }
    }

    // Advance to next sprite frame
    _sprite = _animation.nextSprite();

    // Adjust position based on sprite size changes during attack animation
    const pair spriteSize = AnimationHandler::getSpriteDimension(_sprite).value();
    const pair sizeDiff = spriteSize - _size;

    // Adjust position for different attack directions to maintain visual consistency
    switch (_animation.currentDirection())
    {
    case D_LEFT:
        _pos.x -= sizeDiff.x; // Adjust for left attack sprite expansion
        // Fall through to also adjust Y position

    case D_UP:
    case D_UPLEFT:
    case D_UPRIGHT:
        _pos.y -= sizeDiff.y; // Adjust for upward attack sprite expansion
        break;
    default:
        break;
    }

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

    _size = spriteSize;
}

// Main movement and animation update function
void Link::move()
{
    static long long int animationSwitch = 0; // Counter for animation frame timing

    const state newState = updateState();

    if (newState == S_STILL)
    {
        // Switch to still animation if not already in still state
        if (_animation.currentState() != S_STILL)
            _animation.set(S_STILL, _animation.currentDirection());
    }
    else if (newState == S_MOVEMENT)
    {
        const direction newDirection = updateDirection();
        _pos += calculateMovement(newDirection); // Update position

        if (newDirection != D_STILL)
        {
            // Update animation direction if direction changed
            if (_animation.currentDirection() != newDirection)
                _animation.set(S_MOVEMENT, newDirection);

            // Update sprite every 3 frames for smooth movement animation
            if (animationSwitch++ % 3 == 0)
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
            animationSwitch = 0; // Reset animation counter
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
            _pos.y = tPos.y + tSize.y; // Push Link down
            break;
        case C_BOTTOM:
            _pos.y = tPos.y - _size.y; // Push Link up
            break;
        case C_LEFT:
            _pos.x = tPos.x + tSize.x; // Push Link right
            break;
        case C_RIGHT:
            _pos.x = tPos.x - _size.x; // Push Link left
            break;
        default:
            break;
        }
    }

    // Keep Link within arena boundaries
    const auto roomPos = _arena->pos();
    const auto roomSize = _arena->size();
    const auto roomEnd = roomPos + roomSize;

    // Clamp position to stay within arena bounds
    _pos.x = std::clamp(_pos.x, roomPos.x, roomEnd.x - _size.x);
    _pos.y = std::clamp(_pos.y, roomPos.y, roomEnd.y - _size.y);
}