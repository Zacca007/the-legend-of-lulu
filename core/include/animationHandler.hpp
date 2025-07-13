#pragma once
#include "types.hpp"
#include <cstdint>
#include <fstream>
#include <map>
#include <vector>

namespace lulu
{
class AnimationHandler

{
    direction _currentDirection;
    std::uint8_t _currentFrame;
    state _currentState;
    bool _enabled;
    std::map<state, std::map<direction, std::vector<std::string>>> _animationSet;
    static std::uint32_t readBigEndian(std::ifstream &file);

  public:
    AnimationHandler();
    AnimationHandler(bool enableAnimation, direction initialDirection, state initialState);
    ~AnimationHandler() = default;

    [[nodiscard]] direction currentDirection() const
    {
        return _currentDirection;
    }

    [[nodiscard]] std::uint8_t currentFrame() const
    {
        return _currentFrame;
    }

    [[nodiscard]] state currentState() const
    {
        return _currentState;
    }

    [[nodiscard]] bool enabled() const
    {
        return _enabled;
    }

    [[nodiscard]] const std::vector<std::string> &currentAnimation() const
    {
        return _animationSet.at(_currentState).at(_currentDirection);
    }

    void enable();
    void addAnimation(state state, direction direction, const std::vector<std::string> &animation);
    void set(state newState, direction newDirection);
    std::string nextSprite();
    static std::optional<pair> getSpriteDimension(const std::string &filepath);
};
} // namespace lulu