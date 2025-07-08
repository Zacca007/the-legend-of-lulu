#pragma once
#include "types.hpp"
#include <cstdint>
#include <map>
#include <vector>

namespace lulu
{
class AnimationHandler
{
private:
    direction _currentDirection;
    std::uint8_t _currentFrame;
    state _currentState;
    bool _enabled;
    std::map<state, std::map<direction, std::vector<std::string>>> _animationSet;

public:
    AnimationHandler();
    AnimationHandler(bool enableAnimation, direction initialDirection, state initialState);
    ~AnimationHandler() = default;

    [[nodiscard]] direction currentDirection() const;
    [[nodiscard]] std::uint8_t currentFrame() const;
    [[nodiscard]] state currentState() const;
    [[nodiscard]] bool enabled() const;

    void enable();
    void addAnimation(state state, direction direction, const std::vector<std::string> &animation);
    void set(state newState, direction newDirection);
    std::string nextSprite();
};
} // namespace lulu