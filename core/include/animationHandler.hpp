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
    AnimationHandler() : _currentDirection(D_STILL), _currentFrame(0), _currentState(S_STILL), _enabled(false)
    {
    }

    AnimationHandler(bool enableAnimation, direction initialDirection, state initialState)
        : _currentDirection(initialDirection), _currentFrame(0), _currentState(initialState), _enabled(enableAnimation)
    {
        if (!enableAnimation)
        {
            _currentState = S_STILL;
            _currentDirection = D_STILL;
        }
    }

    ~AnimationHandler() = default;

    [[nodiscard]] direction CurrentDirection() const
    {
        return _currentDirection;
    }
    [[nodiscard]] std::uint8_t CurrentFrame() const
    {
        return _currentFrame;
    }
    [[nodiscard]] state CurrentState() const
    {
        return _currentState;
    }
    [[nodiscard]] bool Enabled() const
    {
        return _enabled;
    }

    void enable()
    {
        _enabled = true;
    }

    void addAnimation(state state, direction direction, const std::vector<std::string> &animation)
    {
        _animationSet[state][direction] = animation;
    }

    void set(state newState, direction newDirection)
    {
        _currentState = newState;
        _currentDirection = newDirection;
        _currentFrame = 0;
    }

    std::string &nextSprite()
    {
        std::string &sprite = _animationSet[_currentState][_currentDirection][_currentFrame];
        _currentFrame = (_currentFrame + 1) % _animationSet[_currentState][_currentDirection].size();
        return sprite;
    }
};
} // namespace lulu