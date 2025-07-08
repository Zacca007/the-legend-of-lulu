#include "animationHandler.hpp"

using namespace lulu;

AnimationHandler::AnimationHandler()
    : _currentDirection(D_STILL), _currentFrame(0), _currentState(S_STILL), _enabled(false)
{
}

AnimationHandler::AnimationHandler(bool enableAnimation, direction initialDirection, state initialState)
    : _currentDirection(initialDirection), _currentFrame(0), _currentState(initialState), _enabled(enableAnimation)
{
    if (!enableAnimation)
    {
        _currentState = S_STILL;
        _currentDirection = D_STILL;
    }
}

direction AnimationHandler::currentDirection() const
{
    return _currentDirection;
}

std::uint8_t AnimationHandler::currentFrame() const
{
    return _currentFrame;
}

state AnimationHandler::currentState() const
{
    return _currentState;
}

bool AnimationHandler::enabled() const
{
    return _enabled;
}

void AnimationHandler::enable()
{
    _enabled = true;
}

void AnimationHandler::addAnimation(state state, direction direction, const std::vector<std::string> &animation)
{
    _animationSet[state][direction] = animation;
}

void AnimationHandler::set(state newState, direction newDirection)
{
    _currentState = newState;
    _currentDirection = newDirection;
    _currentFrame = 0;
}

std::string AnimationHandler::nextSprite()
{
    if (_animationSet.empty() ||
        _animationSet[_currentState].empty() ||
        _animationSet[_currentState][_currentDirection].empty())
    {
        return "";
    }

    std::string sprite = _animationSet[_currentState][_currentDirection][_currentFrame];
    _currentFrame = (_currentFrame + 1) % _animationSet[_currentState][_currentDirection].size();
    return sprite;
}