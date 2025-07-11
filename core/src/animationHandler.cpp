#include "animationHandler.hpp"
#include <cstring>

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

std::uint32_t AnimationHandler::readBigEndian(std::ifstream& file)
{
    uint8_t bytes[4];
    file.read(reinterpret_cast<char*>(bytes), 4);
    return (static_cast<uint32_t>(bytes[0]) << 24) |
           (static_cast<uint32_t>(bytes[1]) << 16) |
           (static_cast<uint32_t>(bytes[2]) << 8)  |
           (static_cast<uint32_t>(bytes[3]));
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
    if (_animationSet.empty() || _animationSet[_currentState].empty() ||
        _animationSet[_currentState][_currentDirection].empty())
    {
        return "";
    }

    std::string sprite = _animationSet[_currentState][_currentDirection][_currentFrame];
    _currentFrame = (_currentFrame + 1) % _animationSet[_currentState][_currentDirection].size();
    return sprite;
}

std::optional<pair> AnimationHandler::getSpriteDimension(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file)
        return std::nullopt;

    constexpr uint8_t expected_signature[8] = { 0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A };
    uint8_t signature[8];
    file.read(reinterpret_cast<char*>(signature), 8);
    if (!file || std::memcmp(signature, expected_signature, 8) != 0)
        return std::nullopt;

    file.ignore(4); // Lunghezza chunk

    char chunkType[5] = {0};
    file.read(chunkType, 4);
    if (std::string(chunkType) != "IHDR")
        return std::nullopt;

    uint32_t width = readBigEndian(file);
    uint32_t height = readBigEndian(file);

    return pair{
        static_cast<float>(width),
        static_cast<float>(height)
    };
}