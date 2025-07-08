#include "actors/link.hpp"
#include "arena.hpp"
#include <algorithm>

using namespace lulu;

Link::Link(const pair position, const pair size, float speed, float hp, float damage, Arena *arena)
    : Fighter(position, size, {speed, speed}, hp, damage, arena)
{
    setupAnimations();
    _animation.set(S_MOVEMENT, D_UP);
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
    const std::vector<std::string> attackDown = {
        "core/assets/link/attack/link attack down 1.png", "core/assets/link/attack/link attack down 2.png",
        "core/assets/link/attack/link attack down 3.png", "core/assets/link/attack/link attack down 4.png"};
    const std::vector<std::string> attackLeft = {
        "core/assets/link/attack/link attack left 1.png", "core/assets/link/attack/link attack left 2.png",
        "core/assets/link/attack/link attack left 3.png", "core/assets/link/attack/link attack left 4.png"};
    const std::vector<std::string> attackRight = {
        "core/assets/link/attack/link attack right 1.png", "core/assets/link/attack/link attack right 2.png",
        "core/assets/link/attack/link attack right 3.png", "core/assets/link/attack/link attack right 4.png"};

    // Animazioni verso l'alto
    _animation.addAnimation(S_MOVEMENT, D_UP, up);
    _animation.addAnimation(S_MOVEMENT, D_UPLEFT, up);
    _animation.addAnimation(S_MOVEMENT, D_UPRIGHT, up);

    // Animazioni verso il basso
    _animation.addAnimation(S_MOVEMENT, D_DOWN, down);
    _animation.addAnimation(S_MOVEMENT, D_DOWNLEFT, down);
    _animation.addAnimation(S_MOVEMENT, D_DOWNRIGHT, down);

    // Animazioni laterali
    _animation.addAnimation(S_MOVEMENT, D_LEFT, left);
    _animation.addAnimation(S_MOVEMENT, D_RIGHT, right);

    _animation.addAnimation(S_ATTACK, D_DOWN, attackDown);
    _animation.addAnimation(S_ATTACK, D_DOWNLEFT, attackDown);
    _animation.addAnimation(S_ATTACK, D_DOWNRIGHT, attackDown);

    _animation.addAnimation(S_ATTACK, D_LEFT, attackLeft);
    _animation.addAnimation(S_ATTACK, D_RIGHT, attackRight);
}

direction Link::parseInput() const
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

    return D_STILL;
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
        auto diagonal = _speed.diagonal();
        if (diagonal.has_value())
        {
            const pair diag = diagonal.value();
            switch (dir)
            {
            case D_UPLEFT:
                return {-diag.x, -diag.y};
            case D_UPRIGHT:
                return {diag.x, -diag.y};
            case D_DOWNLEFT:
                return {-diag.x, diag.y};
            case D_DOWNRIGHT:
                return {diag.x, diag.y};
            default:
                break;
            }
        }
    }
    break;
    default:
        break;
    }
    return {0, 0};
}

void Link::move()
{
    const direction newDirection = parseInput();

    if (newDirection != D_STILL)
    {
        const pair movement = calculateMovement(newDirection);
        _pos += movement;

        // Aggiorna animazione solo se la direzione è cambiata
        if (newDirection != _animation.currentDirection())
        {
            _animation.set(S_MOVEMENT, newDirection);
        }
        _sprite = _animation.nextSprite();
    }
}

void Link::handleCollisions(const std::vector<collision> &collisions)
{
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