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
  private:
    // Attributes
    direction _currentDirection; // Current facing direction
    std::uint8_t _currentFrame;  // Current frame in animation sequence
    state _currentState;         // Current animation state
    std::map<state, std::map<direction, std::vector<std::string>>>
        _animationSet; // Animation storage: state -> direction -> sprite sequence

    // Internal helpers
    static std::uint32_t readBigEndian(std::ifstream &file);

  public:
    // attributes
    bool _enabled;

    // Constructors
    AnimationHandler();
    AnimationHandler(bool enableAnimation, direction initialDirection, state initialState);
    ~AnimationHandler() = default;

    // Getters
    [[nodiscard]] direction currentDirection() const;
    [[nodiscard]] std::uint8_t currentFrame() const;
    [[nodiscard]] state currentState() const;

    // Animation control
    [[nodiscard]] const std::vector<std::string> &currentAnimation() const;
    void addAnimation(state state, direction direction, const std::vector<std::string> &animation);
    void set(state newState, direction newDirection);
    std::string nextSprite();

    // Utilities
    static std::optional<pair> getSpriteDimension(const std::string &filepath);
};

} // namespace lulu
