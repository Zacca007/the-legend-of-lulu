#include "actors/fighter.hpp"
#include "arena.hpp"
#include <algorithm>
#include <ranges>

using namespace lulu;

/**
 * @brief Reduce health points by damage amount
 */
void Fighter::takeDamage(const float damage)
{
    _hp -= damage;
}

/**
 * @brief Attack another fighter, dealing damage
 */
void Fighter::attack(Fighter &fighter) const
{
    fighter.takeDamage(_damage);
}

/**
 * @brief Check if fighter is still alive
 */
bool Fighter::isAlive() const
{
    return _hp > 0;
}

/**
 * @brief Check if input keys contain a specific key
 */
bool Fighter::hasKey(const std::vector<Key> &keys, Key key)
{
    return std::ranges::find(keys, key) != keys.end();
}

/**
 * @brief Check if a key was just pressed (not held from previous frame)
 */
bool Fighter::isKeyJustPressed(Key key) const
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
direction Fighter::getCurrentDirection() const
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

/**
 * @brief Calculate movement vector for given direction and speed
 */
pair Fighter::calculateMovement(direction dir, float speedMultiplier) const
{
    const pair effectiveSpeed = _speed * speedMultiplier;

    switch (dir)
    {
    case D_UP:
        return {0, -effectiveSpeed.y};
    case D_DOWN:
        return {0, effectiveSpeed.y};
    case D_LEFT:
        return {-effectiveSpeed.x, 0};
    case D_RIGHT:
        return {effectiveSpeed.x, 0};
    case D_UPLEFT:
    case D_UPRIGHT:
    case D_DOWNLEFT:
    case D_DOWNRIGHT: {
        // Calculate diagonal movement
        if (auto diagonal = effectiveSpeed.diagonal(); diagonal.has_value())
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

/**
 * @brief Keep actor within arena boundaries
 */
void Fighter::clampToArena()
{
    if (!_arena)
        return;

    const auto roomPos = _arena->pos();
    const auto roomSize = _arena->size();
    const auto roomEnd = roomPos + roomSize;

    _pos.x = std::clamp(_pos.x, roomPos.x, roomEnd.x - _size.x);
    _pos.y = std::clamp(_pos.y, roomPos.y, roomEnd.y - _size.y);
}

/**
 * @brief Get all colliding fighters from current collision map
 */
std::vector<Fighter *> Fighter::getCollidingFighters() const
{
    std::vector<Fighter *> collidingFighters;

    if (!_arena)
        return collidingFighters;

    const auto &collisionMap = _arena->collisions();
    auto it = collisionMap.find(std::map<Actor *, std::vector<collision>>::key_type(this));

    if (it != collisionMap.end())
    {
        for (const auto &[target, type] : it->second)
        {
            if (auto *fighter = dynamic_cast<Fighter *>(target))
            {
                collidingFighters.push_back(fighter);
            }
        }
    }

    return collidingFighters;
}

/**
 * @brief Check if actor is colliding with any static objects
 */
bool Fighter::isCollidingWithStatic() const
{
    if (!_arena)
        return false;

    const auto &collisionMap = _arena->collisions();
    auto it = collisionMap.find(std::map<Actor *, std::vector<collision>>::key_type(this));

    if (it != collisionMap.end())
    {
        for (const auto &[target, type] : it->second)
        {
            // Check if target is not a Movable (i.e., static)
            if (!dynamic_cast<Movable *>(target))
            {
                return true;
            }
        }
    }

    return false;
}

/**
 * @brief Deal damage to all colliding fighters
 */
void Fighter::damageCollidingFighters() const
{
    const auto collidingFighters = getCollidingFighters();
    for (auto *fighter : collidingFighters)
    {
        attack(*fighter);
    }
}