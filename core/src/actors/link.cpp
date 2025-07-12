#include "actors/link.hpp"
#include "arena.hpp"
#include <algorithm>
#include <ranges>

using namespace lulu;

Link::Link(const pair position, const pair size, float speed, float hp, float damage, Arena *arena)
    : Fighter(position, size, {speed, speed}, hp, damage, arena), _isAttacking(false), _attackFrame(0)
{
    setupAnimations();
    _animation.set(S_STILL, D_DOWN);
    _sprite = _animation.nextSprite();
}

void Link::setupAnimations()
{
    const std::vector<std::string> up = {"core/assets/link/movement/link up 1.png",
                                         "core/assets/link/movement/link up 2.png"};
    const std::vector<std::string> down = {"core/assets/link/movement/link front 1.png",
                                           "core/assets/link/movement/link front 2.png"};
    const std::vector<std::string> left = {"core/assets/link/movement/link left 1.png",
                                           "core/assets/link/movement/link left 2.png"};
    const std::vector<std::string> right = {"core/assets/link/movement/link right 1.png",
                                            "core/assets/link/movement/link right 2.png"};



    // Animazioni di attacco - tutte le direzioni
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

    // Animazioni movimento
    _animation.addAnimation(S_MOVEMENT, D_UP, up);
    _animation.addAnimation(S_MOVEMENT, D_UPLEFT, up);
    _animation.addAnimation(S_MOVEMENT, D_UPRIGHT, up);
    _animation.addAnimation(S_MOVEMENT, D_DOWN, down);
    _animation.addAnimation(S_MOVEMENT, D_DOWNLEFT, down);
    _animation.addAnimation(S_MOVEMENT, D_DOWNRIGHT, down);
    _animation.addAnimation(S_MOVEMENT, D_LEFT, left);
    _animation.addAnimation(S_MOVEMENT, D_RIGHT, right);

    _animation.addAnimation(S_STILL, D_UP, up);
    _animation.addAnimation(S_STILL, D_UPLEFT, up);
    _animation.addAnimation(S_STILL, D_UPRIGHT, up);
    _animation.addAnimation(S_STILL, D_DOWN, down);
    _animation.addAnimation(S_STILL, D_DOWNLEFT, down);
    _animation.addAnimation(S_STILL, D_DOWNRIGHT, down);
    _animation.addAnimation(S_STILL, D_LEFT, left);
    _animation.addAnimation(S_STILL, D_RIGHT, right);

    // Animazioni attacco
    _animation.addAnimation(S_ATTACK, D_DOWN, attackDown);
    _animation.addAnimation(S_ATTACK, D_DOWNLEFT, attackDown);
    _animation.addAnimation(S_ATTACK, D_DOWNRIGHT, attackDown);
    _animation.addAnimation(S_ATTACK, D_LEFT, attackLeft);
    _animation.addAnimation(S_ATTACK, D_RIGHT, attackRight);
    // TODO: ADD D_UP ATTACKS
}

state Link::updateState() const
{
    if (_isAttacking)
        return S_ATTACK;

    for (const Key key : _arena->currKeys())
    {
        switch (key)
        {
        case K_SPACE:
            return S_ATTACK;
        case K_W:
        case K_UP:
        case K_A:
        case K_LEFT:
        case K_S:
        case K_DOWN:
        case K_D:
        case K_RIGHT:
            return S_MOVEMENT;
        default:
            break;
        }
    }
    return S_STILL;
}

direction Link::updateDirection() const
{
    bool w = false, a = false, s = false, d = false;

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

    // Risolvi input conflittuali
    if (a && d)
        a = d = false;
    if (w && s)
        w = s = false;

    // Determina la direzione
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

    return _animation.currentDirection();
}

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
    return {0, 0};
}

void Link::setupAttack()
{
    _isAttacking = true;
    _attackFrame = 0;
    _previousFrame = _animation.currentFrame();
    _animation.set(S_ATTACK, _animation.currentDirection());
}

void Link::performAttack()
{
    if (_attackFrame == 2)
    {
        const auto &collisionMap = _arena->collisions();
        auto it = collisionMap.find(this);

        if (it != collisionMap.end())
        {
            for (const auto &coll : it->second)
            {
                if (auto *fighter = dynamic_cast<Fighter *>(coll.target))
                {
                    attack(*fighter);
                }
            }
        }
    }

    _sprite = _animation.nextSprite();

    if (const auto spriteDimOpt = AnimationHandler::getSpriteDimension(_sprite))
    {
        const pair spriteSize = spriteDimOpt.value();
        const pair sizeDiff = spriteSize - _size;

        switch (_animation.currentDirection())
        {
        case D_LEFT:
            _pos.x -= sizeDiff.x;

        case D_UP:
        case D_UPLEFT:
        case D_UPRIGHT:
            // TODO: handle vertical adjustment
            break;
        default:
            break;
        }

        _size = spriteSize;
    }

    ++_attackFrame;
}

void Link::endAttack()
{
    _isAttacking = false;
    _attackFrame = 0;

    _animation.set(S_MOVEMENT, _animation.currentDirection());

    while (_animation.currentFrame() < _previousFrame - 1)
        _animation.nextSprite();

    _sprite = _animation.nextSprite();
}

void Link::move()
{
    static long long int animationSwitch = 0;
    if (const state newState = updateState(); newState == S_STILL)
    {
        if (_animation.currentState() != S_STILL)
            _animation.set(S_STILL, _animation.currentDirection());
    }

    else if (newState == S_MOVEMENT)
    {
        const direction newDirection = updateDirection();
        _pos += calculateMovement(newDirection);

        if (_animation.currentDirection() != newDirection && newDirection != D_STILL)
            _animation.set(S_MOVEMENT, newDirection);

        if (animationSwitch++ % 3 == 0)
        _sprite = _animation.nextSprite();
    }

    else if (newState == S_ATTACK)
    {
        if (!_attackFrame)
            setupAttack();

        performAttack();

        if (_attackFrame == _animation.currentAnimation().size())
            endAttack();
    }
}

void Link::handleCollisions(const std::vector<collision> &collisions)
{
    if (_isAttacking)
        return; // Non gestire collisioni durante l'attacco

    for (const auto &coll : collisions)
    {
        const auto &target = coll.target;
        const auto &tPos = target->pos();
        const auto &tSize = target->size();

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

    // Mantieni Link dentro l'arena
    const auto roomPos = _arena->pos();
    const auto roomSize = _arena->size();
    const auto roomEnd = roomPos + roomSize;

    _pos.x = std::clamp(_pos.x, roomPos.x, roomEnd.x - _size.x);
    _pos.y = std::clamp(_pos.y, roomPos.y, roomEnd.y - _size.y);
}