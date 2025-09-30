#include "movable.hpp"
#include <nlohmann/json.hpp>

namespace lulu
{
    Movable::Movable(const Vec2<float> speed, bool enableAnimation) : speed_(speed)
    {
        if (enableAnimation)
            movement_.enabled_ = true;
    }

    Movable::Movable(const std::string& configPath)
    {
        std::ifstream f(configPath);
        if (!f.is_open())
        {
            throw std::runtime_error("Could not open config file: " + configPath);
        }

        nlohmann::json j;
        f >> j;

        // Parsing sezione "movable"
        if (j.contains("movable"))
        {
            const auto& movableJson = j["movable"];

            // Speed
            if (movableJson.contains("speed"))
            {
                const auto& speedJson = movableJson["speed"];
                float x = speedJson["x"].get<float>();
                float y = speedJson["y"].get<float>();
                speed_ = Vec2{x, y};
            }

            // Enable animation
            bool enableAnimation = false;
            if (movableJson.contains("enableAnimation"))
            {
                enableAnimation = movableJson["enableAnimation"].get<bool>();
            }

            if (enableAnimation)
            {
                movement_.enabled_ = true;
            }
        }
    }

    const Vec2<float>& Movable::speed() const
    {
        return speed_;
    }
} // namespace lulu
