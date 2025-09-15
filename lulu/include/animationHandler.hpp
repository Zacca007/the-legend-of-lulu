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
    Direction movementDirection_; // Current facing direction
    std::uint8_t frame_;          // Current frame in animation sequence
    State state_;                 // Current animation state
    std::map<State, std::map<Direction, std::vector<std::string>>>
        animationSet_; // Animation storage: state -> direction -> sprite sequence

    // Internal helpers
    static std::uint32_t readBigEndian(std::ifstream &file);

  public:
    // attributes
    bool enabled_;

    // Constructors
    AnimationHandler();
    AnimationHandler(bool enableAnimation, Direction initialDirection, State initialState);
    ~AnimationHandler() = default;

    // Getters
    [[nodiscard]] Direction currentDirection() const;
    [[nodiscard]] std::uint8_t currentFrame() const;
    [[nodiscard]] State currentState() const;

    // Animation control
    [[nodiscard]] const std::vector<std::string> &currentAnimation() const;
    void addAnimation(State state, Direction direction, const std::vector<std::string> &animation);
    void set(State newState, Direction newDirection);
    std::string nextSprite();

    // Utilities
    static std::optional<Vec2<float>> getSpriteDimension(const std::string &filepath);
};

} // namespace lulu