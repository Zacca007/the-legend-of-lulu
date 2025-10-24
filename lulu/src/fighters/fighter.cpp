#include "fighters/fighter.hpp"

#include <nlohmann/json.hpp>

namespace lulu
{
    Fighter::Fighter(const Vec2<float> position, const Vec2<float> size, const Vec2<float> speed, const float hp,
                     const float damage, const std::string& sprite)
        : Actor(position, size, sprite), Movable(speed, true), hp_(hp), damage_(damage)
    {
    }

    Fighter::Fighter(Vec2<float> pos, const std::string& configPath) : Actor(pos, configPath), Movable(configPath)
    {
        std::ifstream f(configPath);
        if (!f.is_open())
        {
            throw std::runtime_error("Could not open config file: " + configPath);
        }

        nlohmann::json j;
        f >> j;

        // Parsing sezione "fighter"
        if (j.contains("fighter"))
        {
            const auto& fighterJson = j["fighter"];

            if (fighterJson.contains("hp"))
            {
                hp_ = fighterJson["hp"].get<float>();
            }

            if (fighterJson.contains("damage"))
            {
                damage_ = fighterJson["damage"].get<float>();
            }
        }
    }

    void Fighter::recoil(const Direction collisionDirection)
    {
        switch (collisionDirection)
        {
        case D_UP:
        case D_UPLEFT:
        case D_UPRIGHT:
            pos_.y += speed_.y * 2;
            break;

        case D_DOWN:
        case D_DOWNLEFT:
        case D_DOWNRIGHT:
            pos_.y -= speed_.y * 2;
            break;

        case D_LEFT:
            pos_.x += speed_.x * 2;
            break;
        case D_RIGHT:
            pos_.x -= speed_.x * 2;
            break;
        default:
            break;
        }
    }

    void Fighter::takeDamage(const float damage)
    {
        hp_ -= damage;
    }

    void Fighter::attack(Fighter* fighter) const
    {
        fighter->takeDamage(damage_);
    }

    bool Fighter::isAlive() const
    {
        return hp_ > 0;
    }

    float Fighter::hp() const
    {
        return hp_;
    }

    float Fighter::damage() const
    {
        return damage_;
    }
} // namespace lulu
