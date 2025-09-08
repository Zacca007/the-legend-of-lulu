#include "actor.hpp"
#include "arena.hpp"
#include <algorithm>
#include <fstream>
#include <nlohmann/json.hpp>

namespace lulu
{
Actor::Actor(const Vec2<float> pos, const Vec2<float> size, const std::string &sprite)
    : pos_(pos), size_(size), sprite_(sprite), arena_(nullptr)
{
}

Actor::Actor(Vec2<float> pos, const std::string &configPath) : pos_(pos), arena_(nullptr)
{
    std::ifstream f(configPath);
    if (!f.is_open())
    {
        throw std::runtime_error("Could not open config file: " + configPath);
    }

    nlohmann::json j;
    f >> j;

    const auto actorJson = j.at("actor");

    sprite_ = actorJson.at("sprite").get<std::string>();

    float width = actorJson.at("size").at("width").get<float>();
    float height = actorJson.at("size").at("height").get<float>();
    size_ = Vec2{width, height};

    /*
    float x = actorJson.at("pos").at("x").get<float>();
    float y = actorJson.at("pos").at("y").get<float>();
    pos_ = Vec2{x, y};
    */
}

const Vec2<float> &Actor::pos() const
{
    return pos_;
}

const Vec2<float> &Actor::size() const
{
    return size_;
}

const std::string &Actor::sprite() const
{
    return sprite_;
}

Arena *Actor::arena() const
{
    return arena_;
}

void Actor::setArena(Arena *arena)
{
    if (arena_ == arena)
        return;

    // Lascia la vecchia arena (senza ownership)
    arena_ = arena;
}

Direction Actor::checkCollision(const Actor *other) const
{
    // Null check and arena validation
    if (!other)
        return D_NONE;

    // Get other actor's bounds
    const auto &[ox, oy] = other->pos();
    const auto &[ow, oh] = other->size();
    // Calculate bounding box corners
    const auto [x, y] = pos_ + size_;
    const Vec2 otherMax = {ox + ow, oy + oh};

    // AABB collision detection - check if boxes don't overlap
    if (x <= ox || otherMax.x <= pos_.x || y <= oy || otherMax.y <= pos_.y)
    {
        return D_NONE;
    }

    // Calculate penetration depths for each side
    const float leftDist = std::abs(x - ox);                // Distance from our right to their left
    const float rightDist = std::abs(otherMax.x - pos_.x);  // Distance from our left to their right
    const float topDist = std::abs(y - oy);                 // Distance from our bottom to their top
    const float bottomDist = std::abs(otherMax.y - pos_.y); // Distance from our top to their bottom

    // Find minimum penetration distances
    const float minH = std::min(leftDist, rightDist); // Minimum horizontal penetration
    const float minV = std::min(topDist, bottomDist); // Minimum vertical penetration

    // Calculate centers to determine collision direction
    const Vec2<float> thisCenter = pos_ + size_ / 2.0f;
    const Vec2<float> otherCenter = other->pos() + other->size() / 2.0f;

    // Return collision type based on minimum penetration
    if (minH <= minV)
        return thisCenter.x <= otherCenter.x ? D_RIGHT : D_LEFT;

    return thisCenter.y <= otherCenter.y ? D_DOWN : D_UP;
}

void Actor::handleCollision(Collision collision)
{
    const Actor *other = collision.target;

    const auto &otherPos = other->pos();
    const auto &otherSize = other->size();

    // Adjust position based on collision direction to prevent overlap
    switch (collision.collisionDirection)
    {
    case D_UP:
        pos_.y = otherPos.y + otherSize.y;
        break;
    case D_DOWN:
        pos_.y = otherPos.y - size_.y;
        break;
    case D_LEFT:
        pos_.x = otherPos.x + otherSize.x;
        break;
    case D_RIGHT:
        pos_.x = otherPos.x - size_.x;
        break;
    default:
        break;
    }
}
} // namespace lulu