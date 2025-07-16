#include "actors/link.hpp"
#include "arena.hpp"
#include <cmath>

using namespace lulu;

// Constructor: Initialize Link with position, size, speed, health, damage, and arena reference
Link::Link(const pair position, const pair size, const float speed, const float hp, float damage, Arena *arena)
    : Fighter(position, size, {speed, speed}, hp, damage, arena), attackFrame_(0), previousFrame_(0),
      animationSwitch_(0)
{
    setupAnimation();
    movement_.set(S_STILL, D_DOWN); // Start facing down in still state
    sprite_ = movement_.nextSprite();
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
    movement_.addAnimation(S_MOVEMENT, D_UP, up);
    movement_.addAnimation(S_MOVEMENT, D_UPLEFT, up);
    movement_.addAnimation(S_MOVEMENT, D_UPRIGHT, up);
    movement_.addAnimation(S_MOVEMENT, D_DOWN, down);
    movement_.addAnimation(S_MOVEMENT, D_DOWNLEFT, down);
    movement_.addAnimation(S_MOVEMENT, D_DOWNRIGHT, down);
    movement_.addAnimation(S_MOVEMENT, D_LEFT, left);
    movement_.addAnimation(S_MOVEMENT, D_RIGHT, right);

    // Register idle/still animations
    movement_.addAnimation(S_STILL, D_UP, up);
    movement_.addAnimation(S_STILL, D_UPLEFT, up);
    movement_.addAnimation(S_STILL, D_UPRIGHT, up);
    movement_.addAnimation(S_STILL, D_DOWN, down);
    movement_.addAnimation(S_STILL, D_DOWNLEFT, down);
    movement_.addAnimation(S_STILL, D_DOWNRIGHT, down);
    movement_.addAnimation(S_STILL, D_LEFT, left);
    movement_.addAnimation(S_STILL, D_RIGHT, right);

    // Register attack animations for all directions
    movement_.addAnimation(S_ATTACK, D_UP, attackUp);
    movement_.addAnimation(S_ATTACK, D_UPLEFT, attackUp);
    movement_.addAnimation(S_ATTACK, D_UPRIGHT, attackUp);
    movement_.addAnimation(S_ATTACK, D_DOWN, attackDown);
    movement_.addAnimation(S_ATTACK, D_DOWNLEFT, attackDown);
    movement_.addAnimation(S_ATTACK, D_DOWNRIGHT, attackDown);
    movement_.addAnimation(S_ATTACK, D_LEFT, attackLeft);
    movement_.addAnimation(S_ATTACK, D_RIGHT, attackRight);
}

// Determine Link's current state based on input and attack status
state Link::updatedState() const
{
    // If currently in attack animation, maintain attack state
    if (movement_.currentState()==S_ATTACK)
        return S_ATTACK;

    // Check if attack key was just pressed
    if (arena_ && arena_->isKeyJustPressed(K_SPACE))
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
    if (!arena_)
        return D_STILL;

    bool w = false, a = false, s = false, d = false;

    // Check which directional keys are currently pressed
    for (const Key key : arena_->currKeys())
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
pair Link::calculateMovement(const direction dir) const
{
    pair movement;

    switch (dir)
    {
    case D_UP:
        movement = {0, -speed_.y};
        break;
    case D_DOWN:
        movement = {0, speed_.y};
        break;
    case D_LEFT:
        movement = {-speed_.x, 0};
        break;
    case D_RIGHT:
        movement = {speed_.x, 0};
        break;
    case D_UPLEFT:
        movement = {-speed_.x, -speed_.y};
        break;
    case D_UPRIGHT:
        movement = {speed_.x, -speed_.y};
        break;
    case D_DOWNLEFT:
        movement = {-speed_.x, speed_.y};
        break;
    case D_DOWNRIGHT:
        movement = {speed_.x, speed_.y};
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
    attackFrame_ = 1; // Start attack sequence
    previousFrame_ = movement_.currentFrame();
    movement_.set(S_ATTACK, movement_.currentDirection());
}

// Execute attack animation and damage calculation
void Link::performAttack()
{
    // Deal damage on frame 2 of attack animation (mid-swing)
    if (attackFrame_ == 2)
    {
        // Find colliding fighters and damage them
        if (arena_)
        {
            const auto &collisions = arena_->collisions();
            if (const auto it = collisions.find(this); it != collisions.end())
            {
                for (const auto &[target, type] : it->second)
                {
                    if (auto *fighter = dynamic_cast<Fighter *>(target))
                    {
                        fighter->takeDamage(damage_);
                    }
                }
            }
        }
    }

    // Advance to next sprite frame
    sprite_ = movement_.nextSprite();

    // Adjust position based on sprite size changes during attack animation
    if (const auto spriteDimension = AnimationHandler::getSpriteDimension(sprite_); spriteDimension.has_value())
    {
        const pair spriteSize = spriteDimension.value();
        const pair sizeDiff = spriteSize - size_;
        adjustPositionForAttack(sizeDiff);
        size_ = spriteSize;
    }

    ++attackFrame_;
}

// Clean up after attack animation completes
void Link::endAttack()
{
    attackFrame_ = 0;

    // Return to movement animation state
    movement_.set(S_MOVEMENT, movement_.currentDirection());

    // Fast-forward animation to where it was before attack started
    while (movement_.currentFrame() < previousFrame_ - 1)
        movement_.nextSprite();

    sprite_ = movement_.nextSprite();

    // Readjust position and size back to normal movement sprites
    if (const auto spriteDimension = AnimationHandler::getSpriteDimension(sprite_); spriteDimension.has_value())
    {
        const pair spriteSize = spriteDimension.value();
        const pair sizeDiff = spriteSize - size_;
        adjustPositionForAttack(sizeDiff);
        size_ = spriteSize;
    }
}

// Handle sprite size changes during attack animations
void Link::adjustPositionForAttack(const pair &sizeDiff)
{
    switch (movement_.currentDirection())
    {
    case D_LEFT:
        pos_.x -= sizeDiff.x;
        // Fall through
    case D_UP:
    case D_UPLEFT:
    case D_UPRIGHT:
        pos_.y -= sizeDiff.y;
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
        if (movement_.currentState() != S_STILL)
            movement_.set(S_STILL, movement_.currentDirection());
    }
    else if (newState == S_MOVEMENT)
    {
        if (newDirection != D_STILL)
        {
            pos_ += calculateMovement(newDirection);

            if (movement_.currentDirection() != newDirection)
            {
                movement_.set(S_MOVEMENT, newDirection);
                movement_.nextSprite();
                animationSwitch_ = 0;
            }

            // Update sprite every 4 frames for smooth movement animation
            if (animationSwitch_++ % 4 == 0)
                sprite_ = movement_.nextSprite();
        }
    }
    else if (newState == S_ATTACK)
    {
        // Initialize attack if just starting
        if (attackFrame_ == 0)
            setupAttack();

        performAttack();

        // End attack when animation sequence is complete
        if (attackFrame_ > movement_.currentAnimation().size())
        {
            endAttack();
            animationSwitch_ = 0;
        }
    }
}

// Handle collisions with walls and other objects
void Link::handleCollisions(const std::vector<collision> &collisions)
{
    // Don't handle collisions during attack to prevent interrupting attack animation
    if (attackFrame_ > 0)
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
            pos_.y = tPos.y + tSize.y;
            break;
        case C_BOTTOM:
            pos_.y = tPos.y - size_.y;
            break;
        case C_LEFT:
            pos_.x = tPos.x + tSize.x;
            break;
        case C_RIGHT:
            pos_.x = tPos.x - size_.x;
            break;
        default:
            break;
        }
    }

    // Keep Link within arena boundaries
    clampToArena();
}