#pragma once
#include "types.hpp"
#include <cstdint>
#include <fstream>
#include <map>
#include <vector>

namespace lulu
{
/**
 * @brief Handles sprite animations for game actors
 * * AnimationHandler manages sprite sequences for different states and directions,
 * providing smooth animation transitions and sprite dimension reading from PNG files.
 */
class AnimationHandler
{
    direction _currentDirection; // Current facing direction
    std::uint8_t _currentFrame;  // Current frame in animation sequence
    state _currentState;         // Current animation state
    bool _enabled;               // Whether animation is enabled
 // Animation storage: state -> direction -> sprite sequence
    std::map<state, std::map<direction, std::vector<std::string>>> _animationSet;
 /**
     * @brief Read a 32-bit big-endian integer from file
     * @param file Input file stream
     * @return 32-bit integer value
     */
    static std::uint32_t readBigEndian(std::ifstream &file);

  public:
    /**
     * @brief Default constructor - creates disabled animation handler
     */
    AnimationHandler();
 /**
     * @brief Construct animation handler with initial state
     * @param enableAnimation Whether to enable animation system
     * @param initialDirection Initial facing direction
     * @param initialState Initial animation state
     */
    AnimationHandler(bool enableAnimation, direction initialDirection, state initialState);
 ~AnimationHandler() = default;

    // Getters for animation properties
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
 /**
     * @brief Get current animation sprite sequence
     * @return Reference to current animation vector
     */
    [[nodiscard]] const std::vector<std::string> &currentAnimation() const
    {
        return _animationSet.at(_currentState).at(_currentDirection);
    }

    /**
     * @brief Enable the animation system
     */
    void enable();
 /**
     * @brief Add animation sequence for a state/direction combination
     * @param state Animation state
     * @param direction Facing direction
     * @param animation Vector of sprite file paths
     */
    void addAnimation(state state, direction direction, const std::vector<std::string> &animation);
 /**
     * @brief Set current animation state and direction
     * @param newState New animation state
     * @param newDirection New facing direction
     */
    void set(state newState, direction newDirection);

    /**
     * @brief Get next sprite in current animation sequence
     * @return Path to next sprite file
     */
    std::string nextSprite();

    /**
     * @brief Get dimensions of a PNG sprite file
     * @param filepath Path to PNG file
     * @return Optional pair with width/height, nullopt if file invalid
     */
    static std::optional<pair> getSpriteDimension(const std::string &filepath);
};
}