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
    direction direction_; // Current facing direction
    std::uint8_t frame_;  // Current frame in animation sequence
    state state_;         // Current animation state
    std::map<state, std::map<direction, std::vector<std::string>>>
        animationSet_; // Animation storage: state -> direction -> sprite sequence

    // Internal helpers
    static std::uint32_t readBigEndian(std::ifstream &file);

  public:
    // attributes
    bool enabled_;

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
