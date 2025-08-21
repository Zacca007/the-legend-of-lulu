#include "movable.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>

using json = nlohmann::json;

namespace lulu
{
Movable::Movable(const pair speed, const std::string &animationConfigPath, bool enableAnimation) : speed_(speed)
{
    if (enableAnimation)
    {
        movement_.enabled_ = true;
        if (!animationConfigPath.empty())
        {
            setupAnimation(animationConfigPath);
        }
    }
}

const pair &Movable::speed() const
{
    return speed_;
}

void Movable::setupAnimation(const std::string &configPath)
{
    try
    {
        loadAnimationsFromJson(configPath);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Warning: Failed to load animation config from " << configPath << ": " << e.what() << std::endl;
        std::cerr << "Character will use fallback animations or may not animate correctly." << std::endl;
    }
}

void Movable::loadAnimationsFromJson(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open animation configuration file: " + filepath);
    }

    json jsonData;
    try
    {
        file >> jsonData;
    }
    catch (const json::parse_error &e)
    {
        throw std::runtime_error("JSON parse error in file " + filepath + ": " + e.what());
    }

    if (!jsonData.contains("character"))
    {
        throw std::runtime_error("Invalid animation configuration: missing 'character' object");
    }

    const auto &character = jsonData["character"];

    // Set default state and direction if specified
    if (character.contains("default_state") && character.contains("default_direction"))
    {
        state defaultState = parseState(character["default_state"]);
        direction defaultDirection = parseDirection(character["default_direction"]);
        movement_.set(defaultState, defaultDirection);
    }

    // Load animations
    if (character.contains("animations"))
    {

        for (const auto &animations = character["animations"];
             const auto &[stateKey, stateAnimations] : animations.items())
        {
            state currentState = parseState(stateKey);

            for (const auto &[dirKey, spriteList] : stateAnimations.items())
            {
                direction currentDirection = parseDirection(dirKey);

                if (spriteList.is_array())
                {
                    std::vector<std::string> sprites;
                    for (const auto &sprite : spriteList)
                    {
                        if (sprite.is_string())
                        {
                            sprites.push_back(sprite.get<std::string>());
                        }
                    }

                    if (!sprites.empty())
                    {
                        movement_.addAnimation(currentState, currentDirection, sprites);
                    }
                }
            }
        }
    }

    std::cout << "Successfully loaded animations for: " << character.value("name", "Unknown Character") << std::endl;
}

state Movable::parseState(const std::string &stateStr)
{
    if (stateStr == "S_STILL")
        return S_STILL;
    if (stateStr == "S_MOVEMENT")
        return S_MOVEMENT;
    if (stateStr == "S_ATTACK")
        return S_ATTACK;
    if (stateStr == "D_HURT")
        return D_HURT;

    std::cerr << "Warning: Unknown state '" << stateStr << "', defaulting to S_STILL" << std::endl;
    return S_STILL;
}

direction Movable::parseDirection(const std::string &dirStr)
{
    if (dirStr == "D_STILL")
        return D_STILL;
    if (dirStr == "D_UP")
        return D_UP;
    if (dirStr == "D_DOWN")
        return D_DOWN;
    if (dirStr == "D_LEFT")
        return D_LEFT;
    if (dirStr == "D_RIGHT")
        return D_RIGHT;
    if (dirStr == "D_UPLEFT")
        return D_UPLEFT;
    if (dirStr == "D_UPRIGHT")
        return D_UPRIGHT;
    if (dirStr == "D_DOWNLEFT")
        return D_DOWNLEFT;
    if (dirStr == "D_DOWNRIGHT")
        return D_DOWNRIGHT;

    std::cerr << "Warning: Unknown direction '" << dirStr << "', defaulting to D_STILL" << std::endl;
    return D_STILL;
}

} // namespace lulu